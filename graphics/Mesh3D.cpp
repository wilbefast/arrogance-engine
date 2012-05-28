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

#include "Mesh3D.hpp"

#include "../assert.hpp" // for ASSERT macro
#include "../global.hpp" // for viewport

/* CREATION, DESTRUCTION */

Mesh3D::Mesh3D() :
vertices(),
faces(),
normals(),
min(),
max()
{
}

int Mesh3D::load_obj(const char* filename)
{
  // open the file
  ifstream in(filename, ios::in);
  ASSERT(in, "'Mesh3D::load_obj' opening Wavefront object file");

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
      add_vertex(vertex_t(s));
    // face
    else if(key == "f ")
      parse_faces(s);
    // vertex normal
    else if(key == "vn")
      normals.push_back(normal_t(s));
    // texture coordinates
    else if(key == "vt")
      material.add_texture_coordinate(tex_coord_t(s));
    // material definition
    else if(key == "mt" && line.substr(0, 6) == "mtllib")
    {
      // parse the material filename
      string mtl_file = line.substr(7).insert(0, ASSET_PATH);
      mtl_file = mtl_file.substr(0, mtl_file.find_first_of('\r'));
      // read the material file
      ASSERT(material.load_mtl(mtl_file.c_str()) == EXIT_SUCCESS,
        "'Mesh3D::load_obj' loading associated material file");
    }
  }

  // optimise (shrink-wrap), translate and normalise the resulting mesh
  finalise();
  in.close();


  // all clear!
  return EXIT_SUCCESS;
}

Mesh3D::~Mesh3D()
{
}


/* BUILD ITERATIVELY */

void Mesh3D::add_vertex(vertex_t new_vertex)
{
  // reset minimum and maximum coordinates of this mesh
  if(vertices.size() == 0)
    min = max = new_vertex;
  else for(size_t i = 0; i < 3; i++)
  {
    if(new_vertex[i] > max[i])
      max[i] = new_vertex[i];
    else if(new_vertex[i] < min[i])
      min[i] = new_vertex[i];
  }

  vertices.push_back(new_vertex);
}

void Mesh3D::parse_faces(istringstream& s)
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
  faces.push_back(--triangle);

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
  faces.push_back(--triangle);
}

/* FINISHED BUILDING */

void Mesh3D::finalise()
{
  // shrink-wrap the containers by copying elements into a new vector which
  // fits them all exactly (capacity = size)
  vertex_list_t(vertices).swap(vertices);
  face_list_t(faces).swap(faces);
  normal_list_t(normals).swap(normals);

  // maximum size of any dimension should be 1
  vertex_t size = max - min;
  float inv_max_side = 1.0f / (MAX(MAX(size.x, size.y), size.z));

  // center around the origin (0, 0, 0) and normalise
  vertex_t mid = (max + min) / 2.0f;
  for(vertex_list_it i = vertices.begin(); i != vertices.end(); i++)
  {
    (*i) -= mid;
    (*i) *= inv_max_side;
  }
}

/* DRAW */

void Mesh3D::draw()
{
  // Material
  material.activate();
  // Vertices
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices.front());
  // Triangles
  glEnableClientState(GL_INDEX_ARRAY);
  glIndexPointer(GL_UNSIGNED_BYTE, 0, &faces.front());
  // Normals
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, &normals.front());

  // Draw the triangles using the specified normals
  glDrawElements(GL_TRIANGLES, 3*faces.size(), GL_UNSIGNED_BYTE, &faces.front());

  // Shut down
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  material.deactivate();
  glLoadIdentity();
}
