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


#ifndef COLOUR_HPP_INCLUDED
#define COLOUR_HPP_INCLUDED

#include <istream>

class Colour
{
  /// ATTRIBUTES
public:
  float r, g, b, a;


  /// METHODS
public:
  // creation, destruction
  Colour(int _r, int _g, int _b, int _a = 255);
  Colour(float _r = 1, float _g = 1, float _b = 1, float _a = 1);
  Colour(std::istream& in);

  // array-style access
  float* front();
  float& operator[](size_t i);
  const float& operator[](size_t i) const;

  // instream, outstream
  void read(std::istream& in);
  void write(std::ostream& stream) const;
};

// instream operator, outstream operator
std::ostream& operator<<(std::ostream& stream, Colour const& c);
std::istream& operator>>(std::istream& in, Colour& c);

#endif // COLOUR_HPP_INCLUDED
