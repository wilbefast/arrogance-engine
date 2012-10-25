#include <stdio.h>
#include <string.h>

#include "TGALoader.hpp"

//*********************************************************************************

typedef enum
{
	TGATYPE_NOIMAGE = 0,
	TGATYPE_UNCOMPRESSEDCOLORMAPPED = 1,
	TGATYPE_UNCOMPRESSEDRGB = 2,
	TGATYPE_UNCOMPRESSEDBW = 3,
	TGATYPE_RLECOLORMAPPED = 9,
	TGATYPE_RLERGB = 10,
	TGATYPE_COMPRESSEDBW = 11,
	TGATYPE_COMPRESSEDCOLORMAPPED = 32,
	TGATYPE_COMPRESSEDCOLORMAPPED2 = 33,
} TGA_TYPE;

#pragma pack(push)
#pragma pack(1)

typedef struct TGAHEADERstr
{
   char cIDLength;
   char cColorMapType;
   char cDataTypeCode;
   short s16ColorMapOrigin;
   short s16ColorMapLength;
   char cColorMapDepth;
   short s16XOrigin;
   short s16YOrigin;
   unsigned short u16Width;
   unsigned short u16Height;
   char cBitsPerPixel;
   char cImageDescriptor;
} TGAHEADER;

#pragma pack(pop)

//*******************************************************************************
// Get valid file extensions count
//
unsigned int GetFileExtensionsCount(void)
{
	return(1);
}

//*******************************************************************************
// Get valid file extensions
//
char *GetFileExtension(unsigned int u32Index)
{
	static char *psExtensions[1] = {"TGA"};

	if (u32Index < GetFileExtensionsCount())
		return(psExtensions[u32Index]);
	else
		return(NULL);
}

//*******************************************************************************
// Retrieve readable format versions count
//
unsigned int GetReadVersionCount(void)
{
	return(2);
}

//*******************************************************************************
// Retrieve readable format versions
//
const char *GetReadVersion(unsigned int u32VersionID)
{
	static char *psReadVersions[] = {"Standard Targa", "Extended Targa"};

	if (u32VersionID < GetReadVersionCount())
		return(psReadVersions[u32VersionID]);
	else
		return(NULL);
}

//*******************************************************************************
// File type checking
//
bool IsFileOfType(const char *sFileName, bool bDeepCheck)
{
  ///! FIXME
  return true;


	const char *sExtension;
	unsigned int i;
	FILE *pFile;
	char pcSignature[32];

	if (!sFileName)
		return(false);

	if (bDeepCheck)
	{
		//--- Try to read extended TARGA specification

		pFile = fopen(sFileName, "rb");
		if (pFile)
		{
			fseek(pFile, -18, SEEK_END);
			fread(pcSignature, 1, 18, pFile);
			fclose(pFile);

			//--- Check file signature
			if (!strcmp(pcSignature, "trueVISION-XFILE."))
				return(true);
		}
	}

	//--- Check for file extension (also used for deep-checking old TGA files with no extended information)

	sExtension = sFileName+strlen(sFileName)-1;
	while ((sExtension > sFileName) && (*sExtension != '.'))
		sExtension--;
	if (*sExtension == '.')
		sExtension++;

	for (i=0; i<GetFileExtensionsCount(); ++i)
	{
		if (strcmp(sExtension, GetFileExtension(i)) == 0)
			return(true);
	}

	return(false);
}

//*******************************************************************************
// Decompress RLE buffer
//
void RLEDecompress(unsigned char *pu8RLEBuffer, FILE *pFile, unsigned int u32PixelsCount, unsigned int u32PixelDepth)
{
	unsigned int i;
	unsigned char u8Ctrl, *pu8Ref;

	//--- Parse pixels

	while (u32PixelsCount)
	{
		//--- Read control byte

		fread(&u8Ctrl, 1, 1, pFile);

		if (u8Ctrl & 0x80)
		{
			//--- RLE packet to be decompressed

			u8Ctrl &= 0x7F;
			pu8Ref = pu8RLEBuffer;
			fread(pu8RLEBuffer, 1, u32PixelDepth, pFile);
			pu8RLEBuffer += u32PixelDepth;

			for (i=0; i<u8Ctrl; ++i)
			{
				memcpy(pu8RLEBuffer, pu8Ref, u32PixelDepth);
				pu8RLEBuffer += u32PixelDepth;
			}

			u32PixelsCount -= (u8Ctrl+1);
		}
		else
		{
			//--- RAW packet following

			++u8Ctrl;
			fread(pu8RLEBuffer, 1, u32PixelDepth*u8Ctrl, pFile);
			u32PixelsCount -= u8Ctrl;
			pu8RLEBuffer += u32PixelDepth*u8Ctrl;
		}
	}
}

//*******************************************************************************
// Read colormapped TGA file
//
bool ReadColorMapped(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	//TODO
	return(false);
}

//*******************************************************************************
// Read colormapped RLE TGA file
//
bool ReadColorMappedRLE(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	//TODO
	return(false);
}

//*******************************************************************************
// Decompress RGB16 RLE
//
bool DecompressRGB16RLE(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int u32Stride, i, j, k;
	unsigned short *pu16RLEBuffer;
	unsigned char *pu8Ptr;

	//--- Check alpha channel

	if (pHeader->cImageDescriptor & 0xF)
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[4*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8A8;
		u32Stride = pContext->u32Width*4;
	}
	else
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8;
		u32Stride = pContext->u32Width*3;
	}

	if (!pContext->pu8Pixels)
		return(false);

	//--- Allocate decompression buffer

	pu16RLEBuffer = new unsigned short[pContext->u32Width*pContext->u32Height];
	if (!pu16RLEBuffer)
	{
		delete [] pContext->pu8Pixels;
		pContext->pu8Pixels = NULL;
		return(false);
	}

	//--- Decompress data

	RLEDecompress((unsigned char *)pu16RLEBuffer, pFile, pContext->u32Width*pContext->u32Height, 2);

	//--- Reformat data

	for (j=0; j<pContext->u32Height; ++j)
	{
		pu8Ptr = pContext->pu8Pixels + (pContext->u32Height-1-j)*u32Stride;
		for (i=0; i<pContext->u32Width; ++i)
		{
			k = i*j;
			*(pu8Ptr++) = (unsigned char)((((pu16RLEBuffer[k] & 0x7C00)>>10)*255)/31);
			*(pu8Ptr++) = (unsigned char)((((pu16RLEBuffer[k] & 0x3E0)>>5)*255)/31);
			*(pu8Ptr++) = (unsigned char)(((pu16RLEBuffer[k] & 0x1F)*255)/31);

			if (pHeader->cImageDescriptor & 0xF)
				*(pu8Ptr++) = ((pu16RLEBuffer[k] & 0x8000) == 0x8000)?255:0;
		}
	}

	//--- Destroy work buffer

	delete [] pu16RLEBuffer;

	return(true);
}

//*******************************************************************************
// Decompress RGB24 RLE
//
bool DecompressRGB24RLE(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int u32Stride, i, j, u32PixelDepth;
	unsigned char *pu8Src1, *pu8Src2;

	//--- Allocate image buffer

	pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
	pContext->PixelFormat = PIXELFORMAT_R8G8B8;
	u32PixelDepth = 3;

	if (!pContext->pu8Pixels)
		return(false);

	//--- RLE decompression from file

	RLEDecompress(pContext->pu8Pixels, pFile, pContext->u32Width*pContext->u32Height, u32PixelDepth);

	//--- Swap bytes and image to get proper format

	u32Stride = pContext->u32Width*u32PixelDepth;

	for (j=0; j<pContext->u32Height>>1; ++j)
	{
		pu8Src1 = pContext->pu8Pixels + j*u32Stride;
		pu8Src2 = pContext->pu8Pixels + (pContext->u32Height-1-j)*u32Stride;

		for (i=0; i<pContext->u32Width*u32PixelDepth; i+=u32PixelDepth)
		{
			pu8Src1[i] ^= pu8Src2[i+2];
			pu8Src2[i+2] ^= pu8Src1[i];
			pu8Src1[i] ^= pu8Src2[i+2];
		}
	}

	//--- Flip image

	if (pContext->u32Height & 1)
	{
		j = (pContext->u32Height>>1)+1;

		pu8Src1 = pContext->pu8Pixels + j*u32Stride;
		pu8Src2 = pContext->pu8Pixels + (pContext->u32Height-1-j)*u32Stride;

		for (i=0; i<pContext->u32Width*u32PixelDepth; i+=u32PixelDepth)
		{
			pu8Src1[i] ^= pu8Src2[i+2];
			pu8Src2[i+2] ^= pu8Src1[i];
			pu8Src1[i] ^= pu8Src2[i+2];
		}
	}

	return(true);
}

//*******************************************************************************
// Decompress RGB32 RLE
//
bool DecompressRGB32RLE(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int u32Stride, i, j, u32PixelDepth;
	unsigned char *pu8Src1, *pu8Src2;

	//--- Check alpha channel

	if (pHeader->cImageDescriptor & 0xF)
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[4*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8A8;
		u32PixelDepth = 4;
	}
	else
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8;
		u32PixelDepth = 3;
	}

	if (!pContext->pu8Pixels)
		return(false);

	//--- RLE decompression from file

	RLEDecompress(pContext->pu8Pixels, pFile, pContext->u32Width*pContext->u32Height, u32PixelDepth);

	//--- Swap bytes and image to get proper format

	u32Stride = pContext->u32Width*u32PixelDepth;

	for (j=0; j<pContext->u32Height>>1; ++j)
	{
		pu8Src1 = pContext->pu8Pixels + j*u32Stride;
		pu8Src2 = pContext->pu8Pixels + (pContext->u32Height-1-j)*u32Stride;

		for (i=0; i<pContext->u32Width*u32PixelDepth; i+=u32PixelDepth)
		{
			pu8Src1[i] ^= pu8Src2[i+2];
			pu8Src2[i+2] ^= pu8Src1[i];
			pu8Src1[i] ^= pu8Src2[i+2];
		}
	}

	//--- Flip image

	if (pContext->u32Height & 1)
	{
		j = (pContext->u32Height>>1)+1;

		pu8Src1 = pContext->pu8Pixels + j*u32Stride;
		pu8Src2 = pContext->pu8Pixels + (pContext->u32Height-1-j)*u32Stride;

		for (i=0; i<pContext->u32Width*u32PixelDepth; i+=u32PixelDepth)
		{
			pu8Src1[i] ^= pu8Src2[i+2];
			pu8Src2[i+2] ^= pu8Src1[i];
			pu8Src1[i] ^= pu8Src2[i+2];
		}
	}

	return(true);
}

//*******************************************************************************
// Read RGB RLE TGA file
//
bool ReadRGBRLE(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	pContext->u32Width = (unsigned int)pHeader->u16Width;
	pContext->u32Height = (unsigned int)pHeader->u16Height;

	//--- Skip image identification field

	if (pHeader->cIDLength)
		fseek(pFile, pHeader->cIDLength, SEEK_CUR);

	//--- Skip color map

	if (pHeader->cColorMapType)
		fseek(pFile, (pHeader->s16ColorMapLength*pHeader->cColorMapDepth)>>3, SEEK_CUR);

	//--- Read pixels data

	switch (pHeader->cBitsPerPixel)
	{
	case 16:
		//--- 16-bit image format

		return(DecompressRGB16RLE(pFile, pHeader, pContext));

	case 24:
		//--- 24-bit image format

		return(DecompressRGB24RLE(pFile, pHeader, pContext));

	case 32:
		//--- 32-bit image format

		return(DecompressRGB32RLE(pFile, pHeader, pContext));
	}

	return(false);
}

//*******************************************************************************
// Read B&W RLE TGA file
//
bool ReadBWRLE(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int i;
	unsigned char *pu8RLEBuffer;

	pContext->u32Width = (unsigned int)pHeader->u16Width;
	pContext->u32Height = (unsigned int)pHeader->u16Height;

	//--- Skip image identification field

	if (pHeader->cIDLength)
		fseek(pFile, pHeader->cIDLength, SEEK_CUR);

	//--- Skip color map

	if (pHeader->cColorMapType)
		fseek(pFile, (pHeader->s16ColorMapLength*pHeader->cColorMapDepth)>>3, SEEK_CUR);

	//--- Read pixels data

	switch (pHeader->cBitsPerPixel)
	{
	case 8:
		//--- 8-bit RLE B&W image format

		pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
		if (!pContext->pu8Pixels)
			return(false);

		//--- Allocate decompression buffer

		pu8RLEBuffer = new unsigned char[pContext->u32Width*pContext->u32Height];
		if (!pu8RLEBuffer)
		{
			delete [] pContext->pu8Pixels;
			pContext->pu8Pixels = NULL;
			return(false);
		}

		pContext->PixelFormat = PIXELFORMAT_R8G8B8;

		//--- Decompress data

		RLEDecompress(pu8RLEBuffer, pFile, pContext->u32Width*pContext->u32Height, 1);

		for (i=0; i<pContext->u32Width*pContext->u32Height; ++i)
			pContext->pu8Pixels[3*i] = pContext->pu8Pixels[3*i+1] = pContext->pu8Pixels[3*i+2] = pu8RLEBuffer[i];

		//--- Release decompression buffer

		delete [] pu8RLEBuffer;

		return(true);
	}

	return(false);
}

//*******************************************************************************
// Read RGB16 image
//
bool ReadRGB16(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int i, j, u32Stride;
	unsigned char *pu8Ptr;
	unsigned short u16Pixel;

	//--- Check alpha channel

	if (pHeader->cImageDescriptor & 0xF)
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[4*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8A8;
		u32Stride = pContext->u32Width*4;
	}
	else
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8;
		u32Stride = pContext->u32Width*3;
	}

	if (!pContext->pu8Pixels)
		return(false);

	//--- Read image data

	for (j=0; j<pContext->u32Height; ++j)
	{
		pu8Ptr = pContext->pu8Pixels + (pContext->u32Height-1-j)*u32Stride;
		for (i=0; i<pContext->u32Width; ++i)
		{
			fread(&u16Pixel, sizeof(u16Pixel), 1, pFile);

			*(pu8Ptr++) = (unsigned char)((((u16Pixel & 0x7C00)>>10)*255)/31);
			*(pu8Ptr++) = (unsigned char)((((u16Pixel & 0x3E0)>>5)*255)/31);
			*(pu8Ptr++) = (unsigned char)(((u16Pixel & 0x1F)*255)/31);

			if (pHeader->cImageDescriptor & 0xF)
				*(pu8Ptr++) = ((u16Pixel & 0x8000) == 0x8000)?255:0;
		}
	}

	return(true);
}

//*******************************************************************************
// Read RGB24 image
//
bool ReadRGB24(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int i, j;
	unsigned char *pu8Ptr;

	//--- Allocate image buffer

	pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
	if (!pContext->pu8Pixels)
		return(false);

	pContext->PixelFormat = PIXELFORMAT_R8G8B8;

	//--- Read image data

	for (j=0; j<pContext->u32Height; ++j)
	{
		pu8Ptr = pContext->pu8Pixels + (pContext->u32Height-1-j)*3*pContext->u32Width;
		for (i=0; i<pContext->u32Width; ++i)
		{
			fread(&pu8Ptr[2], 1, 1, pFile);
			fread(&pu8Ptr[1], 1, 1, pFile);
			fread(&pu8Ptr[0], 1, 1, pFile);

			pu8Ptr += 3;
		}
	}
	return(true);
}

//*******************************************************************************
// Read RGB32 image
//
bool ReadRGB32(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int i, j, u32Stride;
	unsigned char *pu8Ptr;

	//--- Check alpha channel

	if (pHeader->cImageDescriptor & 0xF)
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[4*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8A8;
		u32Stride = 4*pContext->u32Width;
	}
	else
	{
		//--- Allocate image buffer

		pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
		pContext->PixelFormat = PIXELFORMAT_R8G8B8;
		u32Stride = 3*pContext->u32Width;
	}

	if (!pContext->pu8Pixels)
		return(false);

	//--- Read image data

	for (j=0; j<pContext->u32Height; ++j)
	{
		pu8Ptr = pContext->pu8Pixels + (pContext->u32Height-1-j)*u32Stride;
		for (i=0; i<pContext->u32Width; ++i)
		{
			fread(&pu8Ptr[2], 1, 1, pFile);
			fread(&pu8Ptr[1], 1, 1, pFile);
			fread(&pu8Ptr[0], 1, 1, pFile);

			if (pHeader->cImageDescriptor & 0xF)
			{
				fread(&pu8Ptr[3], 1, 1, pFile);
				pu8Ptr += 4;
			}
			else
			{
				fseek(pFile, 1, SEEK_CUR);
				pu8Ptr += 3;
			}
		}
	}
	return(true);
}

//*******************************************************************************
// Read RGB TGA file
//
bool ReadRGB(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	pContext->u32Width = (unsigned int)pHeader->u16Width;
	pContext->u32Height = (unsigned int)pHeader->u16Height;

	//--- Skip image identification field

	if (pHeader->cIDLength)
		fseek(pFile, pHeader->cIDLength, SEEK_CUR);

	//--- Skip color map

	if (pHeader->cColorMapType)
		fseek(pFile, (pHeader->s16ColorMapLength*pHeader->cColorMapDepth)>>3, SEEK_CUR);

	//--- Read pixels data

	switch (pHeader->cBitsPerPixel)
	{
	case 16:
		//--- 16-bit image format

		return(ReadRGB16(pFile, pHeader, pContext));

	case 24:
		//--- 24-bit image format

		return(ReadRGB24(pFile, pHeader, pContext));

	case 32:
		//--- 32-bit image format

		return(ReadRGB32(pFile, pHeader, pContext));
	}

	return(false);
}

//*******************************************************************************
// Read B&W TGA file
//
bool ReadBW(FILE *pFile, TGAHEADER *pHeader, IMAGE_DATA *pContext)
{
	unsigned int i, j;
	unsigned char *pu8Ptr;

	pContext->u32Width = (unsigned int)pHeader->u16Width;
	pContext->u32Height = (unsigned int)pHeader->u16Height;

	//--- Skip image identification field

	if (pHeader->cIDLength)
		fseek(pFile, pHeader->cIDLength, SEEK_CUR);

	//--- Skip color map

	if (pHeader->cColorMapType)
		fseek(pFile, (pHeader->s16ColorMapLength*pHeader->cColorMapDepth)>>3, SEEK_CUR);

	//--- Read pixels data

	switch (pHeader->cBitsPerPixel)
	{
	case 8:
		//--- 8-bit B&W image format

		pContext->pu8Pixels = new unsigned char[3*pContext->u32Width*pContext->u32Height];
		if (!pContext->pu8Pixels)
			return(false);

		pContext->PixelFormat = PIXELFORMAT_R8G8B8;

		for (j=0; j<pContext->u32Height; ++j)
		{
			pu8Ptr = pContext->pu8Pixels + (pContext->u32Height-1-j)*3*pContext->u32Width;
			for (i=0; i<pContext->u32Width; ++i)
			{
				fread(&pu8Ptr[0], 1, 1, pFile);
				pu8Ptr[1] = pu8Ptr[2] = pu8Ptr[0];

				pu8Ptr += 3;
			}
		}
		return(true);
	}

	return(false);
}

//*******************************************************************************
// Read TGA file
//
IMAGE_DATA *ReadTGA(const char *sFileName)
{
	FILE *pFile;
	TGAHEADER Header;
	bool bRes = false;
	IMAGE_DATA *pContext;

	//--- Check filename

	if (!sFileName)
		return(NULL);

	//--- Check file type

	if (!IsFileOfType(sFileName, true))
		return(NULL);
	//--- Create image container

	pContext = new IMAGE_DATA;
	if (!pContext)
		return(NULL);

	//--- Init image context

	pContext->pu8Pixels = NULL;
	pContext->u32Width = 0;
	pContext->u32Height = 0;
	pContext->PixelFormat = PIXELFORMAT_R8G8B8;

	//--- Open file

	pFile = fopen(sFileName, "rb");
	if (!pFile)
	{
		delete pContext;
		return(NULL);
	}

	//--- Read file header

	fread(&Header, sizeof(Header), 1, pFile);
	switch (Header.cDataTypeCode)
	{
	case TGATYPE_NOIMAGE:
		//--- Nothing to be loaded
		break;

	case TGATYPE_UNCOMPRESSEDCOLORMAPPED:
		//--- Uncompressed colormapped

		bRes = ReadColorMapped(pFile, &Header, pContext);
		break;

	case TGATYPE_UNCOMPRESSEDRGB:
		//--- Uncompressed RGB

		bRes = ReadRGB(pFile, &Header, pContext);
		break;

	case TGATYPE_UNCOMPRESSEDBW:
		//--- Uncompressed B&W

		bRes = ReadBW(pFile, &Header, pContext);
		break;

	case TGATYPE_RLECOLORMAPPED:
		//--- RLE colormapped

		bRes = ReadColorMappedRLE(pFile, &Header, pContext);
		break;

	case TGATYPE_RLERGB:
		//--- RLE RGB

		bRes = ReadRGBRLE(pFile, &Header, pContext);
		break;

	case TGATYPE_COMPRESSEDBW:
		//--- RLE B&W

		bRes = ReadBWRLE(pFile, &Header, pContext);
		break;

	case TGATYPE_COMPRESSEDCOLORMAPPED:
	case TGATYPE_COMPRESSEDCOLORMAPPED2:
		//--- Not implemented
		break;
	}

	//--- Close file

	fclose(pFile);
	return(pContext);
}

//*******************************************************************************
// Free up file loading buffers
//
void ReleaseTGA(IMAGE_DATA *pContext)
{
	if (!pContext)
		return;

	//--- Free image buffer

	if (pContext->pu8Pixels)
	{
		delete [] pContext->pu8Pixels;
		pContext->pu8Pixels = NULL;
	}

	delete pContext;
}

