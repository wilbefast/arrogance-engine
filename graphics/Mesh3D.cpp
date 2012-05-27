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

#include "Mesh3D.hpp"

#include "../global.hpp" // for viewport

/* CREATION, DESTRUCTION */

Mesh3D::Mesh3D() :
vertices(),
faces(),
normals(),
texture_coordinates(),
min(),
max()
{
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

void Mesh3D::add_face(face_t new_face)
{
  faces.push_back(new_face);
}

void Mesh3D::add_normal(normal_t new_normal)
{
  normals.push_back(new_normal);
}

void Mesh3D::add_texture_coordinate(vertex_t new_texture_coordinate)
{
  texture_coordinates.push_back(new_texture_coordinate);
}

void Mesh3D::set_material(Material new_material)
{
  material = new_material;
}

/* FINISHED BUILDING */

void Mesh3D::finalise()
{
  // shrink-wrap the containers by copying elements into a new vector which
  // fits them all exactly (capacity = size)
  vertex_list_t(vertices).swap(vertices);
  face_list_t(faces).swap(faces);
  normal_list_t(normals).swap(normals);
  vertex_list_t(texture_coordinates).swap(texture_coordinates);

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
  // Materials
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.emission.front());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material.shininess));

  // Vertices
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices.front());
  // Triangles
  glEnableClientState(GL_INDEX_ARRAY);
  glIndexPointer(GL_UNSIGNED_BYTE, 0, &faces.front());
  // Normals
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, &normals.front());
  // Texture
  if(texture)
  {
    glBindTexture(GL_TEXTURE_2D, texture->getHandle());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &texture_coordinates.front());
  }

  // Draw the triangles using the specified normals
  glDrawElements(GL_TRIANGLES, 3*faces.size(), GL_UNSIGNED_BYTE, &faces.front());

  // Shut down
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  if(texture)
  {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  glLoadIdentity();
}
