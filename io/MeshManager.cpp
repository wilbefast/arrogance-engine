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

#include "../graphics/Mesh3D.hpp"

#include "file.hpp"

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
mesh()
{
}

MeshManager::~MeshManager()
{
}

/// LOADING

int MeshManager::load()
{
  // load meshes and materials

  //! FIXME
  ASSERT(mesh.load_obj(GET_ASSET("Island_001.obj")) == EXIT_SUCCESS, "Loading mesh");

  //! TODO
  //ASSERT(load_xml(GET_ASSET("3Dobjects.xml")) == EXIT_SUCCESS,
    //    "Loading 3D assets based on '3Dobjects.xml'");

  // all clear!
  log(LOG_INFO, "MeshManager::load() - Okay");
  return EXIT_SUCCESS;
}

int MeshManager::unload()
{
  // Clean up meshes and materials

  //! FIXME


  //! TODO
  // destroy meshes


  // all clear!
  log(LOG_INFO, "MeshManager::unload() - Okay");
  return EXIT_SUCCESS;
}

int MeshManager::parse_root(void* root_handle)
{
  //! TODO
  return EXIT_SUCCESS;
}

int MeshManager::parse_element(void* element)
{
  //! TODO
  return EXIT_SUCCESS;
}
