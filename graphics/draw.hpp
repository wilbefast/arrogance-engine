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

#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED

#include "Colour.hpp"
#include "../math/Rect.hpp"
#include "../math/V2.hpp"
#include "../math/V3.hpp"

#define CIRCLE_BASE_SEGMENTS 6

#define NEAR 0.5
#define FAR 1000
#define VIEW_FIELD 60

namespace draw
{
  /// FUNCTIONS
  // set draw mode
  void use2D();
  void use3D();
  // draw primitives
  void rectangle(fRect rect, Colour c = Colour());
  void line(fV2 start, fV2 end, Colour c = Colour(), float thickness = 1.0f);
  void line(fV3 start, fV3 end, Colour c = Colour(), float thickness = 1.0f);
  void line_loop(fV2 points[], unsigned int n_pts, Colour c = Colour(),
                                                float thickness = 1.0f);
  void height_line(float height[], unsigned int n_pts, float x_spacing,
                  fV2 base = fV2(0.0f, 0.0f), unsigned int start_i = 0,
                  Colour c = Colour(), float thickness = 1.0f);
  void height_fill(float height[], unsigned int n_pts, float x_spacing,
              fV2 base = fV2(0.0f, 0.0f), unsigned int start_i = 0,
              Colour c = Colour());
  void circle(fV2 position, double radius, Colour c = Colour(),
                   bool fill = false);
};

#endif // GRAPHICS_HPP_INCLUDED
