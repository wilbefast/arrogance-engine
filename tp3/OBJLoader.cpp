#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OBJLoader.hpp"
#include "TGALoader.hpp"

//*********************************************************************************

typedef enum
{
	TAGNAME_COMMENT = 0,
	TAGNAME_MATLIB,
	TAGNAME_VERTEX,
	TAGNAME_FACE,
	TAGNAME_NORMAL,
	TAGNAME_TEXTURECOORDS,
	TAGNAME_OBJECT,
	TAGNAME_MATERIAL,
	TAGNAME_AMBIENT,
	TAGNAME_DIFFUSE,
	TAGNAME_SPECULAR,
	TAGNAME_EMISSION,
	TAGNAME_TEXTURE,
	TAGNAME_OPACITY,
	TAGNAME_SHININESS,
	TAGNAME_ILLUMINATION,
	TAGNAME_UNKNOWN,

	TAGNAME_MAXCOUNT
} TAG_NAME;

//*********************************************************************************

static const char *TokenizeFileLine(const char *sLine, TAG_NAME *pTag);
static void ExtractParameters(TAG_NAME Tag, const char *pcArgs, void *pData);

static void ParseMaterialLibrary(SCENE *pS, const char *sSceneFile, const char *pcMaterialLib, bool bParseOnly);

//*********************************************************************************
// Read OBJ file from disk
//
SCENE *ReadOBJFile(const char *sFileName)
{
	FILE *pScene;
	char pcLine[512], pcMaterialLib[MAX_PATH], pcMaterialName[64];
	const char *pcArgs;
	TAG_NAME Tag;
	SCENE *pS = NULL;

	//--- Check parameters

	if (!sFileName)
		return(NULL);

	//--- Open scene file

	pScene = fopen(sFileName, "r");
	if (!pScene)
		return(NULL);

	//--- Create scene object

	pS = new SCENE;
	if (!pS)
		goto Exit;

	pS->pObjects = NULL;
	pS->u32ObjectsCount = 0;

	pS->pVertices = NULL;
	pS->u32VerticesCount = 0;

	pS->pNormals = NULL;
	pS->u32NormalsCount = 0;

	pS->pUV = NULL;
	pS->u32UVCount = 0;

	pS->pFaces = NULL;
	pS->u32FacesCount = 0;

	pS->pMaterials = NULL;
	pS->u32MaterialsCount = 0;

	//--- Parse scene file, first pass

	while (fgets(pcLine, sizeof(pcLine), pScene))
	{
		//--- Parse file line

		pcArgs = TokenizeFileLine(pcLine, &Tag);

		//--- Process file line

		switch (Tag)
		{
		case TAGNAME_VERTEX:
			//--- Count vertex

			++pS->u32VerticesCount;
			break;

		case TAGNAME_NORMAL:
			//--- Count normal

			++pS->u32NormalsCount;
			break;

		case TAGNAME_TEXTURECOORDS:
			//--- Count UV pair

			++pS->u32UVCount;
			break;

		case TAGNAME_FACE:
			//--- Count face

			++pS->u32FacesCount;
			break;

		case TAGNAME_OBJECT:
			//--- Count object

			++pS->u32ObjectsCount;
			break;

		case TAGNAME_MATLIB:
			//--- Parse materials library

			ExtractParameters(Tag, pcArgs, pcMaterialLib);
			ParseMaterialLibrary(pS, sFileName, pcMaterialLib, true);
			break;
		}
	}

	if (!pS->u32VerticesCount || !pS->u32FacesCount)
	{
		ReleaseScene(pS);
		pS = NULL;
		goto Exit;
	}

	//--- Allocate scene buffers

	pS->pVertices = new VERTEX[pS->u32VerticesCount + pS->u32NormalsCount];
	if (!pS->pVertices)
	{
		ReleaseScene(pS);
		pS = NULL;
		goto Exit;
	}

	pS->pNormals = pS->pVertices + pS->u32VerticesCount;

	if (pS->u32UVCount)
	{
		pS->pUV = new TEXTURE_COORDS[pS->u32UVCount];
		if (!pS->pUV)
		{
			ReleaseScene(pS);
			pS = NULL;
			goto Exit;
		}
	}

	pS->pFaces = new FACE[pS->u32FacesCount];
	if (!pS->pFaces)
	{
		ReleaseScene(pS);
		pS = NULL;
		goto Exit;
	}

	pS->pMaterials = new MATERIAL[pS->u32MaterialsCount];
	if (!pS->pMaterials)
	{
		ReleaseScene(pS);
		pS = NULL;
		goto Exit;
	}
	memset(pS->pMaterials, 0, sizeof(MATERIAL)*pS->u32MaterialsCount);

	pS->pObjects = new OBJECT[pS->u32ObjectsCount+1];
	if (!pS->pObjects)
	{
		ReleaseScene(pS);
		pS = NULL;
		goto Exit;
	}

	pS->pObjects[0].u32FirstFace = 0;
	pS->pObjects[0].u32FacesCount = 0;
	pS->pObjects[0].u32Material = 0xFFFFFFFF;

	//--- Reparse scene file

	pS->u32VerticesCount = pS->u32NormalsCount = pS->u32UVCount = pS->u32FacesCount = pS->u32ObjectsCount = pS->u32MaterialsCount = 0;

	fseek(pScene, 0, SEEK_SET);
	while (fgets(pcLine, sizeof(pcLine), pScene))
	{
		//--- Parse file line

		pcArgs = TokenizeFileLine(pcLine, &Tag);
		//--- Process file line

		switch (Tag)
		{
		case TAGNAME_VERTEX:
			//--- Retrieve vertex position

			ExtractParameters(Tag, pcArgs, &pS->pVertices[pS->u32VerticesCount++]);
			break;

		case TAGNAME_NORMAL:
			//--- Retrieve vertex normal

			ExtractParameters(Tag, pcArgs, &pS->pNormals[pS->u32NormalsCount++]);
			break;

		case TAGNAME_TEXTURECOORDS:
			//--- Retrieve UV coordinates

			ExtractParameters(Tag, pcArgs, &pS->pUV[pS->u32UVCount++]);
			break;

		case TAGNAME_FACE:
			//--- Retrieve face description

			ExtractParameters(Tag, pcArgs, &pS->pFaces[pS->u32FacesCount++]);
			(pS->pObjects[pS->u32ObjectsCount].u32FacesCount)++;
			break;

		case TAGNAME_OBJECT:
			//--- Switch to next object

			pS->u32ObjectsCount++;
			pS->pObjects[pS->u32ObjectsCount].u32FirstFace = pS->u32FacesCount;
			pS->pObjects[pS->u32ObjectsCount].u32FacesCount = 0;

			//--- Retrieve material parameters

			ExtractParameters(Tag, pcArgs, pcMaterialName);

			//--- Look for material

			pS->pObjects[pS->u32ObjectsCount].u32Material = 0;
			for (; pS->pObjects[pS->u32ObjectsCount].u32Material<pS->u32MaterialsCount; pS->pObjects[pS->u32ObjectsCount].u32Material++)
			{
				if (!strcmp(pS->pMaterials[pS->pObjects[pS->u32ObjectsCount].u32Material].pcName, pcMaterialName))
					break;
			}

			if (pS->pObjects[pS->u32ObjectsCount].u32Material >= pS->u32MaterialsCount)
				pS->pObjects[pS->u32ObjectsCount].u32Material = 0xFFFFFFFF;
			break;

		case TAGNAME_MATLIB:
			//--- Parse materials library
			ExtractParameters(Tag, pcArgs, pcMaterialLib);
			ParseMaterialLibrary(pS, sFileName, pcMaterialLib, false);
			break;
		}
	}

	pS->u32ObjectsCount++;

Exit:

	//--- Close scene file

	fclose(pScene);

	return(pS);
}

//*********************************************************************************
// Parse materials library file
//
static void ParseMaterialLibrary(SCENE *pS, const char *sSceneFile, const char *sMatLib, bool bParseOnly)
{
	FILE *pMat;
	char pcLine[512], pcPath[MAX_PATH], *pcPtr;
	const char *pcArgs;
	TAG_NAME Tag;

	if (!sMatLib)
		return;
  const char* pccPtr = sMatLib;
  while ((pccPtr < sMatLib+strlen(sMatLib)) && (*(pccPtr-1) != '/'))
    ++pccPtr;

	//--- Extract base path

	strcpy(pcPath, sSceneFile);
	pcPtr = pcPath;

	while ((pcPtr < pcPath+strlen(pcPath)) && (*(pcPtr-1) != '/'))
		++pcPtr;
  strcpy(pcPtr, pccPtr);

	//--- Open scene file
	pMat = fopen(pcPath, "r");
	if (!pMat)
	  return;

	//--- Parse scene file, first pass
	while (fgets(pcLine, sizeof(pcLine), pMat))
	{

		//--- Parse file line

		pcArgs = TokenizeFileLine(pcLine, &Tag);

		//--- Process file line

		switch (Tag)
		{
		case TAGNAME_MATERIAL:
			//--- Retrieve new material name
			if (!bParseOnly)
			{
				ExtractParameters(Tag, pcArgs, pS->pMaterials[pS->u32MaterialsCount].pcName);

				pS->pMaterials[pS->u32MaterialsCount].pfAmbient[0] = pS->pMaterials[pS->u32MaterialsCount].pfAmbient[1] = pS->pMaterials[pS->u32MaterialsCount].pfAmbient[2] = pS->pMaterials[pS->u32MaterialsCount].pfAmbient[3] = 0;
				pS->pMaterials[pS->u32MaterialsCount].pfDiffuse[0] = pS->pMaterials[pS->u32MaterialsCount].pfDiffuse[1] = pS->pMaterials[pS->u32MaterialsCount].pfDiffuse[2] = 0;
				pS->pMaterials[pS->u32MaterialsCount].pfDiffuse[3] = 1;
				pS->pMaterials[pS->u32MaterialsCount].pfSpecular[0] = pS->pMaterials[pS->u32MaterialsCount].pfSpecular[1] = pS->pMaterials[pS->u32MaterialsCount].pfSpecular[2] = pS->pMaterials[pS->u32MaterialsCount].pfSpecular[3] = 0;
				pS->pMaterials[pS->u32MaterialsCount].pfEmission[0] = pS->pMaterials[pS->u32MaterialsCount].pfEmission[1] = pS->pMaterials[pS->u32MaterialsCount].pfEmission[2] = pS->pMaterials[pS->u32MaterialsCount].pfEmission[3] = 0;
				pS->pMaterials[pS->u32MaterialsCount].fShininess = 128;
				pS->pMaterials[pS->u32MaterialsCount].pcTextureFile[0] = 0;
				pS->pMaterials[pS->u32MaterialsCount].pDiffuse = NULL;
				pS->pMaterials[pS->u32MaterialsCount].u32RenderFlags = 0;
			}

			pS->u32MaterialsCount++;
			break;

		case TAGNAME_OPACITY:
			//--- Retrieve opacity properties

			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, &pS->pMaterials[pS->u32MaterialsCount-1].pfDiffuse[3]);
			break;

		case TAGNAME_SHININESS:
			//--- Retrieve shininess properties

			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, &pS->pMaterials[pS->u32MaterialsCount-1].fShininess);
			if (pS->pMaterials[pS->u32MaterialsCount-1].fShininess < 1)
				pS->pMaterials[pS->u32MaterialsCount-1].fShininess = 1;
			else if (pS->pMaterials[pS->u32MaterialsCount-1].fShininess > 128)
				pS->pMaterials[pS->u32MaterialsCount-1].fShininess = 128;
			break;

		case TAGNAME_ILLUMINATION:
			//--- Retrieve lighting properties

			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, &pS->pMaterials[pS->u32MaterialsCount-1].u32RenderFlags);
			break;

		case TAGNAME_AMBIENT:
			//--- Retrieve ambient properties

			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, pS->pMaterials[pS->u32MaterialsCount-1].pfAmbient);
			break;

		case TAGNAME_DIFFUSE:
			//--- Retrieve diffuse properties

			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, pS->pMaterials[pS->u32MaterialsCount-1].pfDiffuse);
			break;

		case TAGNAME_SPECULAR:
			//--- Retrieve specular properties

			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, pS->pMaterials[pS->u32MaterialsCount-1].pfSpecular);
			break;

		case TAGNAME_EMISSION:
			//--- Retrieve emission properties

			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, pS->pMaterials[pS->u32MaterialsCount-1].pfEmission);
			break;

		case TAGNAME_TEXTURE:
			//--- Retrieve texture filename
			if (bParseOnly)
				break;

			ExtractParameters(Tag, pcArgs, pS->pMaterials[pS->u32MaterialsCount-1].pcTextureFile);

			//--- Extract base path

			strcpy(pcPath, sSceneFile);
			pcPtr = pcPath;

      while ((pcPtr < pcPath+strlen(pcPath)) && (*(pcPtr-1) != '/'))
        ++pcPtr;
			strcpy(pcPtr, pS->pMaterials[pS->u32MaterialsCount-1].pcTextureFile);
			//--- Load texture file

			pS->pMaterials[pS->u32MaterialsCount-1].pDiffuse = ReadTGA(pcPath);
			break;
		}
	}

	//-- Close library file

	fclose(pMat);
}

//*********************************************************************************
// Parse file line and extract parameters
//
static const char *TokenizeFileLine(const char *sLine, TAG_NAME *pTag)
{
	char pcTag[64];
	const char *pcPtr;
	static char *psTags[] = {"#", "v", "vt", "vn", "f", "mtllib", "usemtl", "newmtl", "Ns", "d", "Ka", "Kd", "Ks", "Ke", "map_Kd", "illum", NULL};
	static TAG_NAME pTags[] = {TAGNAME_COMMENT, TAGNAME_VERTEX, TAGNAME_TEXTURECOORDS, TAGNAME_NORMAL, TAGNAME_FACE, TAGNAME_MATLIB, TAGNAME_OBJECT, TAGNAME_MATERIAL, TAGNAME_SHININESS, TAGNAME_OPACITY, TAGNAME_AMBIENT, TAGNAME_DIFFUSE, TAGNAME_SPECULAR, TAGNAME_EMISSION, TAGNAME_TEXTURE, TAGNAME_ILLUMINATION, TAGNAME_UNKNOWN};
	int s32TagLength, i;

	//--- Skip leading spaces

	pcPtr = sLine;
	while (*pcPtr && ((*pcPtr == ' ') || (*pcPtr == '\t')) && (*pcPtr != '\n') && (*pcPtr != '\r'))
		++pcPtr;

	//--- Copy tag

	s32TagLength = 0;
	while (*pcPtr && (*pcPtr != '\r') && (*pcPtr != '\n') && (*pcPtr != ' ') && (*pcPtr != '\t') && (s32TagLength < sizeof(pcTag)-1))
		pcTag[s32TagLength++] = *(pcPtr++);
	pcTag[s32TagLength++] = 0;

	//--- Identify tag

	i = 0;
	while (psTags[i] && strcmp(psTags[i], pcTag))
		++i;

	*pTag = pTags[i];

	//--- Skip empty spaces

	while (*pcPtr && ((*pcPtr == ' ') || (*pcPtr == '\t')) && (*pcPtr != '\n') && (*pcPtr != '\r'))
		++pcPtr;

	return(pcPtr);
}

//*********************************************************************************
// Parse file line and extract parameters
//
static void ExtractParameters(TAG_NAME Tag, const char *pcArgs, void *pData)
{
	FACE *pF;
	unsigned int i;

	switch (Tag)
	{
	case TAGNAME_MATLIB:
		//--- Material library file

		sscanf(pcArgs, "%s", pData);
		break;

	case TAGNAME_VERTEX:
		//--- Read vertex data

		sscanf(pcArgs, "%f %f %f", &(((VERTEX *)pData)->fX), &(((VERTEX *)pData)->fY), &(((VERTEX *)pData)->fZ));
		break;

	case TAGNAME_FACE:
		//--- Face declaration

		pF = (FACE *)pData;

		for (i=0; i<3; ++i)
		{
			pF->pu32Vertices[i] = pF->pu32UV[i] = pF->pu32Normals[i] = 0;

			//--- Read vertex index

			while (*pcArgs && ((*pcArgs == ' ') || (*pcArgs == '\t')))
				++pcArgs;

			pF->pu32Vertices[i] = atoi(pcArgs);
			if (pF->pu32Vertices[i])
				pF->pu32Vertices[i]--;

			//--- Read optional texture coordinates index

			while (*pcArgs && (*pcArgs != '/') && (*pcArgs != '\t') && (*pcArgs != ' '))
				++pcArgs;

			if (*pcArgs == '/')
			{
				pF->pu32UV[i] = atoi(++pcArgs);
				if (pF->pu32UV[i])
					pF->pu32UV[i]--;
				else
					pF->pu32UV[i] = pF->pu32Vertices[i];
			}
			else
				pF->pu32UV[i] = pF->pu32Vertices[i];

			//--- Read optional normal index

			while (*pcArgs && (*pcArgs != '/') && (*pcArgs != '\t') && (*pcArgs != ' '))
				++pcArgs;

			if (*pcArgs == '/')
			{
				pF->pu32Normals[i] = atoi(++pcArgs);
				if (pF->pu32Normals[i])
					pF->pu32Normals[i]--;
				else
					pF->pu32Normals[i] = pF->pu32Vertices[i];
			}
			else
				pF->pu32Normals[i] = pF->pu32Vertices[i];

			while (*pcArgs && (*pcArgs != ' ') && (*pcArgs != '\t'))
				++pcArgs;
		}
		break;

	case TAGNAME_NORMAL:
		//--- Vertex normal declaration

		sscanf(pcArgs, "%f %f %f", &(((VERTEX *)pData)->fX), &(((VERTEX *)pData)->fY), &(((VERTEX *)pData)->fZ));
		break;

	case TAGNAME_TEXTURECOORDS:
		//--- Texture coordinates declaration

		((TEXTURE_COORDS *)pData)->fV = 0;
		sscanf(pcArgs, "%f %f", &(((TEXTURE_COORDS *)pData)->fU), &(((TEXTURE_COORDS *)pData)->fV));
		break;

	case TAGNAME_OBJECT:
		//--- Retrieve associated material

		sscanf(pcArgs, "%s", pData);
		break;

	case TAGNAME_OPACITY:
		//--- Read material properties

		sscanf(pcArgs, "%f", (float *)pData);
		break;

	case TAGNAME_SHININESS:
		//--- Read material properties

		sscanf(pcArgs, "%f", (float *)pData);
		break;

	case TAGNAME_ILLUMINATION:
		//--- Read ilumination status

		switch (atoi(pcArgs))
		{
		case 0:
			*((unsigned int *)pData) = 0;
			break;

		case 1:
			*((unsigned int *)pData) = RENDERFLAGS_LIGHTING;
			break;

		default:
			*((unsigned int *)pData) = RENDERFLAGS_LIGHTING | RENDERFLAGS_FOG;
			break;
		}
		break;

	case TAGNAME_AMBIENT:
	case TAGNAME_DIFFUSE:
	case TAGNAME_SPECULAR:
	case TAGNAME_EMISSION:
		//--- Read material properties

		sscanf(pcArgs, "%f %f %f", (float *)pData, (float *)pData+1, (float *)pData+2);
		((float *)pData)[3] = 1;
		break;

	case TAGNAME_MATERIAL:
		//--- Read material filename

		sscanf(pcArgs, "%s", pData);
		break;

	case TAGNAME_TEXTURE:
		//--- Read texture filename

		sscanf(pcArgs, "%s", pData);
		break;
	}
}

//*********************************************************************************
// Release scene contents
//
void ReleaseScene(SCENE *pScene)
{
	unsigned int i;

	if (!pScene)
		return;

	if (pScene->pObjects)
		delete [] pScene->pObjects;

	if (pScene->pVertices)
		delete [] pScene->pVertices;

	if (pScene->pUV)
		delete [] pScene->pUV;

	if (pScene->pFaces)
		delete [] pScene->pFaces;

	if (pScene->pMaterials)
	{
		for (i=0; i<pScene->u32MaterialsCount; ++i)
			ReleaseTGA(pScene->pMaterials[i].pDiffuse);

		delete [] pScene->pMaterials;
	}
}
