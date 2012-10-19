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

#include "../debug/assert.h"          // for ASSERT macro
#include "../io/file.hpp"               // for ASSET_PATH

using namespace std;

/* CREATION, DESTRUCTION */

Material::Material(Colour a, Colour d, Colour s, Colour e, float shine) :
ambient(a),
diffuse(d),
specular(s),
emission(e),
shininess(shine),
use_texture(false),
texture()
{
  if(shine < 0)
    shininess = 0;
  else if(shine > 128)
    shininess = 128;
}

int Material::load_mtl(const char* filename)
{
  // open the file
  ifstream in(filename, ios::in);
  ASSERT(in, "'Material::load_mtl' opening material file");

  // read each line
  string line;
  while (getline(in, line))
  {
    // lop off the first two characters, used to determine how we read the line
    if(line.size() < 2 || line[0] == '#')
      continue;
    string key(line.substr(0, 2));
    istringstream s(line.substr(2));

    // ambient
    if(key == "Ka")
      ambient = Colour(s);
    // diffuse
    else if(key == "Kd")
      diffuse = Colour(s);
    // specular
    else if(key == "Ks")
      specular = Colour(s);
    // emission
    else if(key == "Ke")
      emission = Colour(s);
    // shininess
    else if(key == "Ns")
      s >> shininess;
    // transparency / alpha
    else if (key == "d " || key == "Tr")
      s >> ambient.a;

    // texture
    else if(key == "ma" && line.substr(0, 6) == "map_Kd")
    {
      // parse the texture filename
      string tex_file = line.substr(7).insert(0, ASSET_PATH);
      tex_file = tex_file.substr(0, tex_file.find_first_of('\r'));
      // load the texture file
      ASSERT(texture.load(tex_file.c_str()) == EXIT_SUCCESS,
        "'Material::load_mtl' loading associated texture file");
      use_texture = true;
    }
  }

  // close the file even though the destructor does this for us - I have OCD :D
  in.close();

  // all clear!
  return EXIT_SUCCESS;
}


/* DRAWING */

void Material::activate()
{
  // material
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,emission.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
  // UV map
  /*if(use_texture)
  {
    glBindTexture(GL_TEXTURE_2D, texture.getHandle());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &texture_coordinates.front());
  }*/
}

void Material::deactivate()
{
  /*
  if(use_texture)
  {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  */
}

