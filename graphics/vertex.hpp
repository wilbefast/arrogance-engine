#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED

#include "../opengl.h"        // Needed for openGL/GLES types
#include "../math/V3.hpp"

#include <vector>

typedef V3<GLfloat> vertex_t;
typedef std::vector<vertex_t> vertex_list_t;
typedef vertex_list_t::iterator vertex_list_it;

#endif // VERTEX_HPP_INCLUDED
