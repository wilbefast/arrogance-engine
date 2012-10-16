#ifndef __OBJLOADER_H
#define __OBJLOADER_H

#pragma once

//*********************************************************************************

#include "TGALoader.hpp"

//*********************************************************************************

#define RENDERFLAGS_LIGHTING	1L
#define RENDERFLAGS_FOG			1L<<1

#define MAX_PATH 128

typedef struct MATERIALstr
{
	char pcName[64];									// Material's name
	float pfAmbient[4];									// Ambient coefficients
	float pfDiffuse[4];									// Diffuse coefficients
	float pfSpecular[4];								// Specular coefficients
	float pfEmission[4];								// Emission coefficients
	float fShininess;									// Specular shininess exponent
	char pcTextureFile[MAX_PATH];						// Diffuse texture
	IMAGE_DATA *pDiffuse;								// Diffuse texture data
	unsigned int u32RenderFlags;						// Render mode
} MATERIAL;

typedef struct VERTEXstr
{
	float fX, fY, fZ;									// Vertex position
} VERTEX;

typedef struct TEXTURE_COORDSstr
{
	float fU, fV;										// Vertex'texture coordinates
} TEXTURE_COORDS;

typedef struct FACEstr
{
	unsigned int pu32Vertices[3];						// Vertex'indices
	unsigned int pu32UV[3];								// Texture coordinates indices
	unsigned int pu32Normals[3];						// Normals indices
} FACE;

typedef struct OBJECTstr
{
	unsigned int u32Material;							// Material index in global materials table
	unsigned int u32FirstFace;							// First face index
	unsigned int u32FacesCount;							// Total number of faces for object
} OBJECT;

typedef struct SCENEstr
{
	OBJECT *pObjects;									// Objects list
	unsigned int u32ObjectsCount;						// Total objects count

	VERTEX *pVertices;									// Global vertices table
	unsigned int u32VerticesCount;						// Total number of vertices

	VERTEX *pNormals;									// Global normals table
	unsigned int u32NormalsCount;						// Total number of normals

	TEXTURE_COORDS *pUV;								// Global UV table
	unsigned int u32UVCount;							// Total number of UV pairs

	FACE *pFaces;										// Faces'indices table
	unsigned int u32FacesCount;							// Total number of faces

	MATERIAL *pMaterials;								// Global materials table
	unsigned int u32MaterialsCount;						// Total number of materials
} SCENE;

//*********************************************************************************

SCENE *ReadOBJFile(const char *sFileName);
void ReleaseScene(SCENE *pScene);

#endif
