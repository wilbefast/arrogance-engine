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

#ifndef MATERIAL_HPP_INCLUDED
#define MATERIAL_HPP_INCLUDED

#include "../../math/numerise.h"
#include "../../utils/IntrusiveLinked.hpp"

#include "../Colour.hpp"
#include "../Texture.hpp"

class Material : public IntrusiveLinked
{
  //! FRIENDS
  friend class MaterialLibrary;

  //! ATTRIBUTES
public:
  // identifier
  str_id id;
  // material
  Colour ambient, diffuse, specular, emissive;
  float shininess;
  // UV map
  bool use_texture;
  Texture texture;

  //! METHODS
public:
  // creation, destruction
  Material(Colour a = Colour(0.2f, 0.2f, 0.2f, 1.0f),
          Colour d = Colour(0.8f, 0.8f, 0.8f, 1.0f),
          Colour s = Colour(0.0f, 0.0f, 0.0f, 1.0f),
          Colour e = Colour(0.0f, 0.0f, 0.0f, 1.0f),
          float shine = 0.0f);
  // drawing
  bool activate();
  void deactivate();
};

#endif // MATERIAL_HPP_INCLUDED
