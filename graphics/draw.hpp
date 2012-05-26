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

#include "../math/V2.hpp"
#include "../math/V3.hpp"

#define CIRCLE_N_SEGMENTS 12

namespace draw
{
    /// NESTING
    class Colour
    {
        public:
        // attributes
        float r, g, b, a;
        // methods
        Colour(int _r, int _g, int _b, int _a = 255) :
        r(_r/255.0f), g(_g/255.0f), b(_b/255.0f), a(_a/255.0f)
        { }
        Colour(float _r = 1, float _g = 1, float _b = 1, float _a = 1) :
        r(_r), g(_g), b(_b), a(_a)
        { }
    };

    /// FUNCTIONS
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
