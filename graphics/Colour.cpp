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

#include "Colour.hpp"

#include <iostream>

using namespace std;

/// CREATION, DESTRUCTION

Colour::Colour(int _r, int _g, int _b, int _a) :
r(_r/255.0f), g(_g/255.0f), b(_b/255.0f), a(_a/255.0f)
{
}

Colour::Colour(float _r, float _g, float _b, float _a) :
r(_r), g(_g), b(_b), a(_a)
{
}

Colour::Colour(std::istream& in)
{
  in >> r >> g >> b;
  if(in.peek() == '\r')
    a = 1.0f;
  else
    in >> a;
}


/// ARRAY STYLE ACCESS

float* Colour::front()
{
  return &r;
}

float& Colour::operator[](size_t i)
{
  switch(i)
  {
    case 0: return r;
    case 1: return g;
    case 2: return b;
    case 3: default: return a;
  }
}

const float& Colour::operator[](size_t i) const
{
  switch(i)
  {
    case 0: return r;
    case 1: return g;
    case 2: return b;
    case 3: default: return a;
  }
}