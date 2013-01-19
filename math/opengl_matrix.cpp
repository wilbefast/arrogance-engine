/*
Arrogance Engine: a simple SDL/OpenGL game engine for Desktop and Android.
Copyright (C) 2012 William James Dyce

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "opengl_matrix.hpp"

using namespace std;

//! ----------------------------------------------------------------------------
//! WORKSPACE VARIABLES
//! ----------------------------------------------------------------------------
static GLfloat a[16];
static glM44 m;

//! ----------------------------------------------------------------------------
//! DEBUG
//! ----------------------------------------------------------------------------
void printGLMatrix(GLenum which_matrix)
{
  // use this function for debugging OpenGL ;)
  glGetFloatv(which_matrix, a);

  m.importArray(a);
  cout << m << endl;
}

glM44 const* getGLMatrix(GLenum which_matrix)
{
  glGetFloatv(which_matrix, a);
  m.importArray(a);
  return &m;
}

//! ----------------------------------------------------------------------------
//! RENDER
//! ----------------------------------------------------------------------------

void applyTransform(glM44 const& transform)
{
  transform.exportArray(a);
  glMultMatrixf(a);
}

//! ----------------------------------------------------------------------------
//! RESET
//! ----------------------------------------------------------------------------

void setTranslation(glM44& transform, fV3 trans)
{
  transform[3].x = trans.x;
  transform[3].y = trans.y;
  transform[3].z = trans.z;
}

//! ----------------------------------------------------------------------------
//! COMPOSE
//! ----------------------------------------------------------------------------

void addTranslation(glM44& transform, fV3 trans)
{
  transform[3].x += trans.x;
  transform[3].y += trans.y;
  transform[3].z += trans.z;
}

void addRotationX(glM44& transform, float rotation)
{
  transform *= glM44::xRotMatrix(rotation);
}

void addRotationY(glM44& transform, float rotation)
{
  transform *= glM44::yRotMatrix(rotation);
}

void addRotationZ(glM44& transform, float rotation)
{
  transform *= glM44::zRotMatrix(rotation);
}

void addScale(glM44& transform, fV3 scale)
{
  transform[0][0] *= scale.x;
  transform[1][1] *= scale.y;
  transform[2][2] *= scale.z;
}
