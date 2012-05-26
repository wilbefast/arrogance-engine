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
  ASSERT(load_obj(GET_ASSET("knight.obj")) == EXIT_SUCCESS, "Loading knight.obj");
  // All good!
  started = true;
  return EXIT_SUCCESS;
}


/* LOAD EXTERNAL RESOURCES */

// object loading subroutine
void parse_face(istringstream& s, Mesh3D& mesh)
{
  // we'll consider faces with 3 (triangle) or 4 (quad) faces
  int vertices[4] = {-1, -1, -1, -1};

  // skip past all the '/' symbols as for now we're ignoring normals
  size_t get = 0;
  for(size_t v_i = 0; v_i < 4  && s.peek() >= ' '; v_i++)
  {
    s >> vertices[v_i];
    for( ; s.peek() > ' '; s.seekg(get, ios_base::beg), get++);
  }

  // always add a first triangle
  face_t triangle(vertices[0], vertices[1], vertices[2]);
  // remember: OBJ indices start at 1 rather than 0 !
  mesh.add_face(--triangle);

  // add a second triangle only if the face was a quad
  if(vertices[3] == -1)
    return;

  // in the case of a quad, indices are arranged as follows (at least when the
  // exporter is wings3D).
  //  0--1
  //  |\ |
  //  | \|
  //  3--2
  triangle = face_t(vertices[0], vertices[2], vertices[3]);
  mesh.add_face(--triangle);
}

// main object loading method
int MeshManager::load_obj(const char* filename)
{
  // open the file
  ifstream in(filename, ios::in);
  ASSERT(in, "'MeshManager::load_obj' opening Wavefront object file");

  // read each line
  string line;
  while (getline(in, line))
  {
    // lop off the first two characters, used to determine how we read the line
    if(line.size() < 2 || line[0] == '#')
      continue;
    string key(line.substr(0, 2));
    istringstream s(line.substr(2));

    // vertex
    if(key == "v ")
      mesh.add_vertex(vertex_t(s));
    // face
    else if(key == "f ")
      parse_face(s, mesh);
    // vertex normal
    else if(key == "vn")
      mesh.add_normal(normal_t(s));
    // texture coordinates
    else if(key == "vt")
      mesh.add_texture_coordinate(vertex_t(s));
    // material definition
    else if(key == "mt" && line.substr(0, 6) == "mtllib")
    {
      string mtl_file = line.substr(7).insert(0,ASSET_PATH);
      mtl_file = mtl_file.substr(0, mtl_file.find_first_of('\r'));
      ASSERT(load_mtl(mtl_file.c_str()) == EXIT_SUCCESS,
        "'MeshManager::load_obj' loading associated material file");
    }
  }

  // optimise (shrink-wrap), translate and normalise the resulting mesh
  mesh.finalise();
  in.close();


  // all clear!
  return EXIT_SUCCESS;
}


int MeshManager::load_mtl(const char* filename)
{
  cout << '\'' << filename << '\'' << endl;

  // open the file
  ifstream in(filename, ios::in);
  ASSERT(in, "'MeshManager::load_mtl' opening material file");

  // read each line
  string line;
  while (getline(in, line))
  {
    cout << line << endl;
  }

  // close the file even though the destructor does this for us - I have OCD :D
  in.close();

  // all clear!
  return EXIT_SUCCESS;
}
