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

#ifndef MESH3D_HPP_INCLUDED
#define MESH3D_HPP_INCLUDED

#include "vertex.hpp"
#include "Material.hpp"

#include "../utils/IntrusiveLinked.hpp"   // for Group

#include <sstream>
#include <vector>

// faces, for debugging
class face_t
{
public:
  // attributes
  V3<GLubyte> vertex_i, uv_i, normal_i;
  // constructor
  face_t(V3<GLubyte> _vertex_i, V3<GLubyte> _uv_i, V3<GLubyte> _normal_i) :
                    vertex_i(_vertex_i), uv_i(_uv_i), normal_i(_normal_i) { }
  // decrementation operator: C++ indices start at 0, OBJ indices at 1
  face_t& operator--(){ vertex_i--; uv_i--; normal_i--; return (*this); }
};
typedef std::vector<face_t> face_list_t;
typedef face_list_t::iterator face_list_it;

typedef V3<GLfloat> normal_t;
typedef std::vector<normal_t> normal_list_t;
typedef vertex_list_t::iterator normal_list_it;


class Mesh3D
{
  //! NESTING
private:

  // sub-mesh within the mesh, associated with a given material
  class Group : public IntrusiveLinked
  {
    // ATTRIBUTES
  public:
    // faces in this group
    size_t first_face, last_face;
    // material
    Material material;
    // METHODS
  public:
    Group() : IntrusiveLinked(), first_face(0), last_face(0), material() { }
  };

  //! ATTRIBUTES
private:
  // containers
  vertex_list_t vertices;
  face_list_t faces;
  normal_list_t normals;
  // max and minimum coordinates
  vertex_t min, max;
  // there is always at least one group within the mesh
  Group *first_group, *current_group;


  //! METHODS
public:
  // creation, destruction
  Mesh3D();
  int load_obj(const char* filename);
  ~Mesh3D();
  // draw
  void draw();
  // set the size to 1
  void unitise();

  //! SUBROUTINES
private:
  // build iteratively
  void add_vertex(vertex_t new_vertex);
  void parse_faces(std::istringstream& s);
  // finished building
  void finalise();

};

#endif // MESH3D_HPP_INCLUDED
