/*
Black Dog: a game for Android and Desktop platforms using SDL and OpenGLES.
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
#ifndef GRAPHICINCARNATION_HPP_INCLUDED
#define GRAPHICINCARNATION_HPP_INCLUDED

#include "Graphic.hpp"

// Instance of an graphic on the screen (with position, etc)

class GraphicIncarnation
{
  /// FLAGS
public:
  static const char CENTER_X = 0b00000001;
  static const char CENTER_Y = 0b00000010;

  /// ATTRIBUTES
protected:
  Graphic* sprite;
  fRect destination;
  fV2 offset, size;
  float angle;
  char flags;
  bool hidden;

  /// METHODS
public:
  // contructors, destructors
  GraphicIncarnation(fV2 _size = fV2(0.0f, 0.0f),
                    fV2 _offset = fV2(0.0f, 0.0f),
                    char _flags = CENTER_X|CENTER_Y);
  ~GraphicIncarnation();
  // query
  bool isSprite(Graphic* check_sprite) const;
  // accessors
  void setAngle(float new_angle);
  bool setSprite(Graphic* new_sprite);
  void setHidden(bool _hidden);
  void setOffset(fV2 _offset);
  void setSize(fV2 _size);
  // modification
  virtual void setPosition(fV2 _position);
  virtual void draw();
};

#endif // GRAPHICINCARNATION_HPP_INCLUDED
