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


#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include "Button.hpp"

class Input
{
  //! ATTRIBUTES
public:
  // mouse or touch-pad
  uV2 last_touch, last_hover;
  bool clicking, clicking_previous;
  Button* last_button;

  //! METHODS
public:
  // constructor
  Input();
};

#endif // INPUT_HPP_INCLUDED