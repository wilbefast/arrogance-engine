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

#ifndef JOINT_HPP_INCLUDED
#define JOINT_HPP_INCLUDED

#include "vertex.hpp"

class Joint
{
  /// ATTRIBUTES
private:
  // father only stores the first child: subsequent children are stored by their
  // older brother. Search "Decorator Pattern" for more information.
  Joint *father, *child, *sibling;
  vertex_t position;

  /// METHODS
public:
  // creation, destruction
  Joint();
  Joint(Joint* _father, vertex_t position);
  // draw
  void draw();
  // modify armature
  Joint* addChild(Joint* new_child);
  Joint* removeChild(Joint* new_child);
  void setPosition(vertex_t _position);
};

#endif // JOINT_HPP_INCLUDED
