#ifndef MESH3D_HPP_INCLUDED
#define MESH3D_HPP_INCLUDED

#include <vector>

#include "../platform.hpp"  // for openGL types
#include "../math/V3.hpp"

typedef V3<GLfloat> vertex_t;
typedef vector<vertex_t> vertex_list_t;
typedef vertex_list_t::iterator vertex_list_it;

typedef V3<GLubyte> face_t;
typedef vector<face_t> face_list_t;
typedef face_list_t::iterator face_list_it;

typedef V3<GLfloat> normal_t;
typedef vector<normal_t> normal_list_t;
typedef vertex_list_t::iterator normal_list_it;


class Mesh3D
{
  /* ATTRIBUTES */
private:
  // containers
  vertex_list_t vertices;
  face_list_t faces;
  normal_list_t normals;
  vertex_list_t texture_coordinates;
  // max and minimum coordinates
  vertex_t min, max;

  /* METHODS */
public:
  // creation, destruction
  Mesh3D();
  ~Mesh3D();
  // build iteratively
  void add_vertex(vertex_t new_vertex);
  void add_face(face_t new_face);
  void add_normal(normal_t new_normal);
  void add_texture_coordinate(vertex_t new_texture_coordinate);
  // finished building
  void finalise();
  // draw
  void draw();

};

#endif // MESH3D_HPP_INCLUDED
