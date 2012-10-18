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

#ifndef MESHMANAGER_HPP_INCLUDED
#define MESHMANAGER_HPP_INCLUDED

#include "ResourceManager.hpp"
#include "../graphics/Mesh3D.hpp"

class MeshManager : public ResourceManager
{
  /// CONSTANTS
private:

  /// SINGLETON
private:
  static MeshManager* instance;
public:
  static MeshManager* getInstance();

  /// ATTRIBUTES
public: /// FIXME
  Mesh3D mesh;

  /// METHODS
private:
  // creation, destruction
  MeshManager();
public:
  ~MeshManager();
  // loading -- overrides ResourceManager
  int load();
  int unload();
  int parse_root(void* root_handle);
  int parse_element(void* element);
};

#endif // MESHMANAGER_HPP_INCLUDED
