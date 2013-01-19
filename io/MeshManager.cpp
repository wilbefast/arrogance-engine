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

#include "MeshManager.hpp"

#include "../debug/assert.h"
#include "../debug/warn.h"
#include "../math/V3.hpp"

#include "file.hpp"
#include "tinyxml/tinyxml.h"

using namespace std;

//! SINGLETON

MeshManager* MeshManager::instance = NULL;

MeshManager* MeshManager::getInstance()
{
  if(!instance)
    instance = new MeshManager();
  return instance;
}

//! CREATION & DESTRUCTION

MeshManager::MeshManager() :
ResourceManager(),
meshes()
{
}

MeshManager::~MeshManager()
{
}

/// LOADING

int MeshManager::load()
{
  // load meshes and materials

  ASSERT(load_xml(GET_ASSET("objects3D.xml")) == EXIT_SUCCESS,
        "Loading 3D assets based on 'objects3D.xml'");

  // all clear!
  log(LOG_INFO, "MeshManager::load() - Okay");
  return EXIT_SUCCESS;
}

int MeshManager::unload()
{
  // Clean up the meshes
  for(MeshI i = meshes.begin(); i != meshes.end(); i++)
    delete (*i).second;

  // all clear!
  log(LOG_INFO, "MeshManager::unload() - Okay");
  return EXIT_SUCCESS;
}

int MeshManager::parse_root(void* root_handle)
{
  // load meshes
  ASSERT(parse_list(root_handle, "mesh_list") == EXIT_SUCCESS,
              "MeshManager parsing list of meshes");


  return EXIT_SUCCESS;
}

int MeshManager::parse_element(void* element)
{
  // cast element to TinyXML element
  TiXmlElement* txml_element = (TiXmlElement*)element;

  // texture element
  if(!strcmp(txml_element->Value(), "mesh"))
  {
    // get the name of the texture and deduce its filename
    const char* name = txml_element->Attribute("name");
    if(!name)
      WARN_RTN("MeshManager::load_xml", "malformed mesh tag", EXIT_FAILURE);

    // load the texture
    string filename = io::name_to_path(name, MESH_FILETYPE);
    load_mesh(filename.c_str(), name);

    // continue to the next
    txml_element = txml_element->NextSiblingElement();
  }
  // all clear!
  return EXIT_SUCCESS;
}

/// 3D MESHES

int MeshManager::load_mesh(const char* source_file, const char* name)
{
  // load image file
  Mesh3D* new_mesh = new Mesh3D();
  ASSERT(new_mesh->load(source_file) == EXIT_SUCCESS, source_file);

  // save under requested name
  str_id hash = numerise(name);
  meshes[hash] = new_mesh;

  // All clear !
  return EXIT_SUCCESS;
}

Mesh3D* MeshManager::get_mesh(const char* name)
{
  // search for the resource
  str_id hash = numerise(name);
  MeshI i = meshes.find(hash);
  // make sure that it is found
  if(i == meshes.end())
    WARN_RTN("MeshManager::get_mesh invalid identifier", name, NULL);

  // return the texture we recovered
  return (*i).second;
}

