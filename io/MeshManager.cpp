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

#include "../assert.hpp"
#include "../warn.hpp"
#include "../math/V3.hpp"

#include "../graphics/Mesh3D.hpp"

#include "file.hpp"
#include "MeshManager.hpp"

using namespace std;

/// SINGLETON<

MeshManager* MeshManager::instance = NULL;

MeshManager* MeshManager::getInstance()
{
  if(!instance)
    instance = new MeshManager();
  return instance;
}

/// CREATION & DESTRUCTION

MeshManager::MeshManager() :
started(false),
mesh()
{
}

int MeshManager::startup()
{
  // don't start twice!
  if(started)
    WARN_RTN("MeshManager::startup","already started!", EXIT_SUCCESS);

  // load models
  /// TODO
  ASSERT(mesh.load_obj(GET_ASSET("knight.obj")) == EXIT_SUCCESS, "Loading knight.obj");
  // All good!
  started = true;
  return EXIT_SUCCESS;
}
