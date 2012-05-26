#include "Material.hpp"

Material::Material(colour_t a, colour_t d, colour_t s, colour_t e, GLfloat shine) :
ambient(a), diffuse(d), specular(s), emission(e), shininess(shine)
{
  if(shine < 0)
    shininess = 0;
  else if(shine > 128)
    shininess = 128;
}
