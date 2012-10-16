#ifndef __TGALOADER_H
#define __TGALOADER_H

#pragma once

//*******************************************************************************

typedef enum
{
	PIXELFORMAT_R8G8B8 = 0,					// 3*8 bits RGB
	PIXELFORMAT_R8G8B8A8,					// 4*8 bits RGBA

	PIXELFORMAT_MAXCOUNT
} PIXEL_FORMAT;

typedef struct IMAGE_DATAstr
{
	PIXEL_FORMAT PixelFormat;					// Image pixel format
	unsigned char *pu8Pixels;					// Image pixels

	unsigned int u32Width, u32Height;			// Image dimensions

	void *pUserData;							// User data - free to use
} IMAGE_DATA;

//*******************************************************************************

IMAGE_DATA *ReadTGA(const char *sFileName);
void ReleaseTGA(IMAGE_DATA *pContext);

#endif