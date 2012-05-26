#ifndef MATERIAL_HPP_INCLUDED
#define MATERIAL_HPP_INCLUDED

#include "../math/V3.hpp"
#include "../platform.hpp"

typedef V3<GLfloat> colour_t;

class Material
{
    /* ATTRIBUTES */
public:
  colour_t ambient, diffuse, specular, emission;
  GLfloat shininess;

  /* METHODS */
public:
  Material(colour_t a = colour_t(0.2f, 0.2f, 0.2f),
          colour_t d = colour_t(0.8f, 0.8f, 0.8f),
          colour_t s = colour_t(0.0f, 0.0f, 0.0f),
          colour_t e = colour_t(0.0f, 0.0f, 0.0f),
          GLfloat shine = 0.0f);
};

#endif // MATERIAL_HPP_INCLUDED
