#include "Engine.hpp"

#include <SDL/SDL.h>
#include <GL/gl.h>

#include <math.h>
#include <iostream>

using namespace std;

void Engine::setup()
{
  camera.target_x = camera.target_y  = camera.target_z = 0.0f;

	// load the scene
  scene = ReadOBJFile("assets/Island_001.obj");

	// load textures into video memory
	glEnable(GL_TEXTURE_2D);
	for(unsigned int mtl_i = 0; mtl_i < scene->u32MaterialsCount; mtl_i++)
	{
		// get pointer to image data
		IMAGE_DATA* image = scene->pMaterials[mtl_i].pDiffuse;
		if(!image)
			continue;

		//get number of channels in the SDL surface
		PIXEL_FORMAT p_format = image->PixelFormat;
		GLenum ext_format, int_format;
		switch(p_format)
		{
			case PIXELFORMAT_R8G8B8:
				ext_format = GL_RGB;
				int_format = 3;
			break;
			case PIXELFORMAT_R8G8B8A8:
				ext_format = GL_RGBA;
				int_format = 4;
			break;
		}

		// Request an OpenGL unassigned GLuint to identify this texture
		GLuint handle;
		glGenTextures(1, &handle);
		image->pUserData = (void*)(long)handle; //! WARNING

		// Bind the texture object to the current block
		glBindTexture(GL_TEXTURE_2D, handle);

		// Set the textures environnement parameters
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			//Sample RGB, multiply by previous texunit result
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);   //Modulate RGB with RGB
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
			//Sample ALPHA, multiply by previous texunit result
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);  //Modulate ALPHA with ALPHA
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);

		// Set the textures other properties
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Finally: convert the image to a texture
		glTexImage2D(GL_TEXTURE_2D, 0, int_format, image->u32Width, image->u32Height, 0,
			ext_format, GL_UNSIGNED_BYTE, image->pu8Pixels);

		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// print, for debug purposes
	//print();
}

void Engine::render()
{
	// clear and reset
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// move camera
	glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glRotatef(camera.rotation, 0.0f, 1.0f, 0.0f);
	glTranslatef(camera.target_x, camera.target_y, camera.target_z);

	// move to object positions...

	// for each object
	for(unsigned int obj_i = 0; obj_i < scene->u32ObjectsCount; obj_i++)
	{
		// cache the current object
		OBJECT const& obj = scene->pObjects[obj_i];

		// get this object's material
		IMAGE_DATA* image = NULL;
		if(obj.u32Material < scene->u32MaterialsCount) // check if there's a material
		{
			// cache material reference
			MATERIAL& m = scene->pMaterials[obj.u32Material];

			glMaterialfv(GL_FRONT, GL_AMBIENT, m.pfAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m.pfDiffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m.pfSpecular);
			glMaterialfv(GL_FRONT, GL_EMISSION, m.pfEmission);
			glMaterialf(GL_FRONT, GL_SHININESS, m.fShininess);

			// activate texture
			image = m.pDiffuse;
			if(image) //! WARNING
				glBindTexture(GL_TEXTURE_2D, (GLuint)(long)m.pDiffuse->pUserData);
		}

		// draw this object's faces
		glBegin(GL_TRIANGLES);
			// for each triangle in this object
			for(unsigned int tri_i = obj.u32FirstFace, tri_cnt = 0;
          tri_cnt < obj.u32FacesCount; tri_i++, tri_cnt++)
			{
				// cache the current triangle
				FACE const& tri = scene->pFaces[tri_i];

				// cache normals
				VERTEX* n[3] = { &(scene->pNormals[tri.pu32Normals[0]]),
													&(scene->pNormals[tri.pu32Normals[1]]),
													&(scene->pNormals[tri.pu32Normals[2]]) };

				// cache texture coordinates
				TEXTURE_COORDS* tc[3] = { &(scene->pUV[tri.pu32UV[0]]),
													&(scene->pUV[tri.pu32UV[1]]),
													&(scene->pUV[tri.pu32UV[2]]) };

				// cache vertices
				VERTEX* v[3] = { &(scene->pVertices[tri.pu32Vertices[0]]),
											&(scene->pVertices[tri.pu32Vertices[1]]),
											&(scene->pVertices[tri.pu32Vertices[2]]) };

				// first vertex
				glNormal3fv(&(n[0]->fX));
				if(image)
          glTexCoord2fv(&(tc[0]->fU));
				glVertex3fv(&(v[0]->fX));

				// second vertex
				glNormal3fv(&(n[1]->fX));
				if(image)
          glTexCoord2fv(&(tc[1]->fU));
				glVertex3fv(&(v[1]->fX));

				// third vertex
				glNormal3fv(&(n[2]->fX));
				if(image)
          glTexCoord2fv(&(tc[2]->fU));
				glVertex3fv(&(v[2]->fX));
			}
			// finished drawing the triangles
		glEnd();

		// deactivate texture
		if(image)
			glBindTexture(GL_TEXTURE_2D, 0);
	}

	glPopMatrix();
}

void Engine::moveCamera(fV3 amount)
{
  camera.target_x += amount.x;
  camera.target_y += amount.y;
  camera.target_z += amount.z;
}

void Engine::turnCamera(float amount)
{
  camera.rotation += amount;
}


void Engine::keydown(int key)
{
	switch(key)
	{
		// up
		case SDLK_SPACE:
			camera.target_y -= 0.1f;
		break;

		// down
		case SDLK_LCTRL:
			camera.target_y += 0.1f;
		break;

		// forward
		case SDLK_UP:
			camera.target_z += 0.5f;
		break;

		// back
		case SDLK_DOWN:
			camera.target_z -= 0.5f;
		break;

		// left
		case SDLK_LEFT:
			camera.target_x += 0.3f;
		break;

		// right
		case SDLK_RIGHT:
			camera.target_x -= 0.3f;
		break;

    case SDLK_LALT:
      camera.rotation++;
    break;


		default:
		break;
	}
}

void Engine::print(ostream& out) const
{
  for(unsigned int obj_i = 0; obj_i < scene->u32ObjectsCount; obj_i++)
	{
		// cache the current object
		OBJECT const& obj = scene->pObjects[obj_i];
		out << "g" << endl;

    // print the group's vertices
    for(unsigned int tri_i = obj.u32FirstFace, tri_cnt = 0;
    tri_cnt < obj.u32FacesCount; tri_i++, tri_cnt++)
    {
      // cache the current triangle
      FACE const& tri = scene->pFaces[tri_i];
      for(size_t v_i = 0; v_i < 3; v_i++)
      {
        out << "v ";
        VERTEX const& v = scene->pVertices[tri.pu32Vertices[v_i]];
        out << v.fX << ' ' << v.fY << ' ' << v.fZ << endl;
      }
    }

    // print the group's normals
    for(unsigned int tri_i = obj.u32FirstFace, tri_cnt = 0;
    tri_cnt < obj.u32FacesCount; tri_i++, tri_cnt++)
    {
      // cache the current triangle
      FACE const& tri = scene->pFaces[tri_i];
      for(size_t v_i = 0; v_i < 3; v_i++)
      {
        out << "vn ";
        VERTEX const& v = scene->pNormals[tri.pu32Normals[v_i]];
        out << v.fX << ' ' << v.fY << ' ' << v.fZ << endl;
      }
    }

    // print the group's faces
    for(unsigned int tri_i = obj.u32FirstFace, tri_cnt = 0;
    tri_cnt < obj.u32FacesCount; tri_i++, tri_cnt++)
    {
      // cache the current triangle
      FACE const& tri = scene->pFaces[tri_i];
      out << "f ";
      for(size_t v_i = 0; v_i < 3; v_i++)
       out << tri.pu32Vertices[v_i]+1 << '/'
            << tri.pu32UV[v_i]+1 << '/'
            << tri.pu32Normals[v_i]+1 << ' ';
      out << endl;
    }

	}
}
