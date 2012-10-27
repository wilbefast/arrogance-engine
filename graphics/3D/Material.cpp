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

#include <iostream>
#include <fstream>
#include <sstream>

#include "Material.hpp"

using namespace std;

//! CREATION, DESTRUCTION

Material::Material(Colour a, Colour d, Colour s, Colour e, float shine) :
id(0),
ambient(a),
diffuse(d),
specular(s),
emissive(e),
shininess(shine),
use_texture(false),
texture()
{
  if(shine < 0)
    shininess = 0;
  else if(shine > 128)
    shininess = 128;
}


//! DRAWING

void Material::activate()
{
  // material
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.front());
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular.front());
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.front());
  glMaterialfv(GL_FRONT, GL_EMISSION, emissive.front());
  glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);

  // UV map
  if(use_texture)
  {
    static bool first = true;
    if(first) std::cout << "WARN - Material textures not working: FIXME!\n";
    first = false;
    //glBindTexture(GL_TEXTURE_2D, texture.getHandle());
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glTexCoordPointer(2, GL_FLOAT, 0, &texture_coordinates.front());
  }
}

void Material::deactivate()
{
  if(use_texture)
  {
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}
