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

#include <sstream>  // for istringstream
#include <fstream>  // for ifstream

#include "../io/file.hpp" // for ASSET_PATH

#include "../debug/assert.h"

#include "Texture.hpp"

#include "MaterialLibrary.hpp"

using namespace std;

//! CREATION, DESTRUCTION

MaterialLibrary::MaterialLibrary() :
first_material(),
current_material(&first_material)
{
}

int MaterialLibrary::load_mtl(const char* filename)
{
  // open the file
  ifstream in(filename, ios::in);
  ASSERT(in, "'MaterialLibrary::load_mtl' opening material file");

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
      current_material->ambient = Colour(s);
    // diffuse
    else if(key == "Kd")
      current_material->diffuse = Colour(s);
    // specular
    else if(key == "Ks")
      current_material->specular = Colour(s);
    // emission
    else if(key == "Ke")
      current_material->emissive = Colour(s);
    // shininess
    else if(key == "Ns")
      s >> current_material->shininess;
    // transparency / alpha
    else if (key == "d " || key == "Tr")
    {
      s >> current_material->ambient.a;
      current_material->diffuse.a = current_material->specular.a
        = current_material->emissive.a = current_material->ambient.a;
    }

    // texture
    else if(key == "ma" && !line.substr(0, 6).compare("map_Kd"))
    {
      // parse the texture filename
      string tex_file = line.substr(7).insert(0, ASSET_PATH);
      tex_file = tex_file.substr(0, tex_file.find_first_of('\r'));
      // load the texture file
      ASSERT(current_material->texture.load(tex_file.c_str()) == EXIT_SUCCESS,
          "'MaterialLibrary::load_mtl' loading associated texture file");
      current_material->use_texture = true;
    }

    // start new material
    else if (key == "ne" && !line.substr(0, 6).compare("newmtl"))
    {
      // never close empty material
      if(current_material->id)
        current_material = (Material*)current_material->newNext(new Material());
      // get the name of the group
      current_material->id = numerise(line.substr(7).c_str());
    }
  }

  // all clear !
  return EXIT_SUCCESS;
}

MaterialLibrary::~MaterialLibrary()
{
  // delete all materials
  first_material.deleteConnections();
}

//! QUERY

Material* MaterialLibrary::getMaterial(const char* name)
{
  // hash the name for speedy look-up
  str_id id = numerise(name);

  current_material = &first_material;
  do
  {
    // are these the droids we're looking for?
    if(current_material->id == id)
      return current_material;

    // move on to next material
    current_material = (Material*)current_material->getNext();
	}
	while(current_material != &first_material);

	// couldn't find it: go about your business
	return NULL;
}
