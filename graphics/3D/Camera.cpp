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

#include "Camera.hpp"

#include "../../opengl.h"

Camera::Camera() :
yaw(0),
position(0.0f, 0.0f, 0.0f)
{

}

void Camera::turn(int amount)
{
  yaw += amount;
}

void Camera::pan(fV3 amount)
{
  double c = cos(yaw.getRad()),
          s = sin(yaw.getRad());

  position.x += c*amount.x - s*amount.z;
  position.y += amount.y;
  position.z += c*amount.z + s*amount.x;
}

void Camera::lookThrough() const
{
  glRotatef(yaw.getDeg(), 0.0f, 1.0f, 0.0f);
  glTranslatef(position.x, position.y, position.z);
}


