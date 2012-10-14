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

#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include "SDL.h"               // Needed for SDL_Surface

#include "../opengl.h"         // Needed for GLuint
#include "../math/V2.hpp"      // Needed for iV2
#include "../math/Rect.hpp"    // Needed for iRect

class Texture
{
  /// ATTRIBUTES
private:
  GLuint handle;
  bool loaded;
  iRect area; // size of the texture

  /// METHODS
public:
  // constructors, destructors
  Texture();
  int load(const char* filename);
  int from_surface(SDL_Surface* surface);
  int unload();
  ~Texture();
  // accessors
  iRect getArea() const;
  GLuint getHandle() const;
  void draw(const fRect* source_pointer,
            const fRect* destination_pointer,
            float angle = 0.0);
};

#endif // TEXTURE_HPP_INCLUDED
