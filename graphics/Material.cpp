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

#include "../assert.hpp"

/* CREATION, DESTRUCTION */

Material::Material(Colour a, Colour d, Colour s, Colour e, float shine) :
ambient(a), diffuse(d), specular(s), emission(e), shininess(shine)
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
  ASSERT(in, "'MeshManager::load_mtl' opening material file");

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
  }

  // close the file even though the destructor does this for us - I have OCD :D
  in.close();

  // all clear!
  return EXIT_SUCCESS;
}
