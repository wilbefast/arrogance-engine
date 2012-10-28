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

#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "../../math/V3.hpp"
#include "../../math/Angle.hpp"

class Camera
{
  //! ATTRIBUTES
private:
  Angle yaw;
  fV3 position;

  //! METHODS
public:
  // constructors, destructors
  Camera();
  // accessors
  void reset();
  void turn(int amount);
  void pan(fV3 amount);
  void lookThrough() const;
};

#endif // CAMERA_HPP_INCLUDED
