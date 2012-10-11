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

#ifndef MODELVIEWSTATE_HPP_INCLUDED
#define MODELVIEWSTATE_HPP_INCLUDED

#include "../math/V3.hpp"

#include "GameState.hpp"

class ModelViewState : public GameState
{
  /// CONSTANTS
private:
  static const float CAMERA_MAX_ANGLE = 90.0f;

  /// ATTRIBUTES
private:
  iV3 camera_angle;
  iV3 camera_offset;

  /// METHODS
public:
  // Creation, destruction
  ModelViewState();
  int startup();
  int shutdown();
  // Overrides GameState
  int update(float delta);
  void draw();
};

#endif // MODELVIEWSTATE_HPP_INCLUDED
