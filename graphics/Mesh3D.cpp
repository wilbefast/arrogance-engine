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

#include "../debug/assert.h"            // for ASSERT macro
#include "../io/file.hpp"               // for ASSET_PATH
#include "../math/V4.hpp"               // used to store quads temporarily

using namespace std;

//! PRIVATE FUNCTIONS

static inline bool isSeperator(char c)
{
  return (c == ' ' || c == '\t' || c == '/');
}

// in the case of a quad, indices are arranged as follows (at least when the
// exporter is wings3D).
//  0--1
//  |\ |
//  | \|
//  3--2

// triangle (0,1,2)
template <typename T>
static inline V3<T> toFirstV3(V4<T> v)
{
  return V3<T>(v.x, v.y, v.z);
}

//triangle (0,2,3)
template <typename T>
static inline V3<T> toSecondV3(V4<T> v)
{
  return V3<T>(v.x, v.z, v.w);
}

//! CREATION, DESTRUCTION

Mesh3D::Mesh3D() :
vertices(),
faces(),
normals(),
min(),
max(),
first_group(new Group()),
current_group(first_group)
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
    // vertex normal
    else if(key == "vn")
      normals.push_back(normal_t(s));
    // texture coordinates
    else if(key == "vt")
      current_group->material.add_texture_coordinate(tex_coord_t(s));
    // face
    else if(key == "f ")
      parse_faces(s);
    // group
    else if (key == "g " && faces.size()) // never close empty groups
    {
      // close current group
      current_group->last_face = faces.size()-1;
      // start new group
      current_group = (Group*)current_group->newNext(new Group());
      current_group->first_face = faces.size();

    }
    // material definition
    else if(key == "mt" && line.substr(0, 6) == "mtllib")
    {
      // parse the material filename
      string mtl_file = line.substr(7).insert(0, ASSET_PATH);
      mtl_file = mtl_file.substr(0, mtl_file.find_first_of('\r'));
      // read the material file
      ASSERT(current_group->material.load_mtl(mtl_file.c_str()) == EXIT_SUCCESS,
        "'Mesh3D::load_obj' loading associated material file");
    }
  }

  // file is no longer needed
  in.close();

  // optimise (shrink-wrap) the resulting mesh, connect group list, etc
  finalise();

  // all clear!
  return EXIT_SUCCESS;
}

Mesh3D::~Mesh3D()
{
  IntrusiveLinked* deletion_iterator = current_group->getNext();
  while(deletion_iterator != NULL)
  {
    delete deletion_iterator;
    deletion_iterator = current_group->getNext();
  }
  delete current_group;
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
  // we'll consider only faces with 3 (triangle) or 4 (quad) faces
  iV4 vert_indices(-1, -1, -1, -1),
      uv_indices(-1, -1, -1, -1),
      norm_indices(-1, -1, -1, -1);

  /*! the OBJ format has defines faces:
        f A B C D ...
      A, B, C, D and so on are triplets of values seperated by slashes:
        A = Vi/UVi/Ni
      The texture coordinate index (UVi) and normal index (Ni) are optional,
      so we might have something of the form:
        A = Vi//
      */
  static bool first = true;
  size_t str_i = 0;
  for(size_t vtx_i = 0; vtx_i < 4  && s.peek() >= ' '; vtx_i++)
  {
    // 1. read the vertex index
    s >> vert_indices[vtx_i];

    // 2. read the (optional) texture coordinate index
    do { s.seekg(str_i++, ios_base::beg); } while(s.peek() > '/');
    if(s.peek() == '/') s.seekg(str_i++, ios_base::beg); else break;
    s >> uv_indices[vtx_i];

    // 3. read the (optional) normal index
    do { s.seekg(str_i++, ios_base::beg); } while(s.peek() > '/');
    if(s.peek() == '/') s.seekg(str_i++, ios_base::beg); else break;
    s >> norm_indices[vtx_i];

    // 4. move on to the next block
    while(isSeperator((s.peek()))) { s.seekg(str_i++, ios_base::beg); }
  }

  // always add a first triangle
  face_t triangle(toFirstV3(vert_indices),
                  toFirstV3(uv_indices),
                  toFirstV3(norm_indices));
  // remember: OBJ indices start at 1 rather than 0 !
  faces.push_back(--triangle);

  // add a second triangle only if the face was a quad
  if(vertices[3] == -1)
    return;
  triangle = face_t(toSecondV3(vert_indices),
                    toSecondV3(uv_indices),
                    toSecondV3(norm_indices));
  // remember: OBJ indices start at 1 rather than 0 !
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

  // Discard the last (empty) Group we created
  IntrusiveLinked* delete_iterator = current_group;
  current_group = (Group*)current_group->getPrev();
  delete delete_iterator;

  // Connect first and last elements and sent pointer back at the start
  current_group->newNext(first_group);
  current_group = first_group;
}

void Mesh3D::unitise()
{
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
  max = (max - mid)*inv_max_side;
  min = (min - mid)*inv_max_side;

}

/* DRAW */


void Mesh3D::draw()
{
	// clear and reset
	glClear(GL_DEPTH_BUFFER_BIT |GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // move camera
	glMatrixMode(GL_PROJECTION);

  glBegin(GL_POINTS);
	for(size_t i = 0; i < vertices.size(); i++)
    glVertex3fv(vertices[i].front());
  glEnd();
}
/*
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
  //material.deactivate();
  glLoadIdentity();
}*/
