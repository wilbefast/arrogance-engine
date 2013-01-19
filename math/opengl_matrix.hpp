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

#ifndef OPENGL_MATRIX_HPP_INCLUDED
#define OPENGL_MATRIX_HPP_INCLUDED

#include "../opengl.h"
#include "V3.hpp"
#include "M44.hpp"

typedef M44<GLfloat> glM44;


// debug
void printGLMatrix(GLenum which_matrix);
glM44 const* getGLMatrix(GLenum which_matrix);

// render
void applyTransform(glM44 const& transform);

// reset
void setTranslation(glM44& transform, fV3 trans);

// compose
void addTranslation(glM44& transform, fV3 trans);
void addRotationX(glM44& transform, float angle);
void addRotationY(glM44& transform, float angle);
void addRotationZ(glM44& transform, float angle);
void addScale(glM44& transform, fV3 scale);

#endif // OPENGL_MATRIX_HPP_INCLUDED
