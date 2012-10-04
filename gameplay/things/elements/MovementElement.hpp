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

#ifndef MOVEMENTELEMENT_HPP_INCLUDED
#define MOVEMENTELEMENT_HPP_INCLUDED

#include "ThingElement.hpp"
#include "../../../scenes/GameState.hpp"

class MovementElement : public ThingElement
{
  /// CONSTANTS
protected:
  static const float DEFAULT_SPEED_MIN;

  /// ATTRIBUTES
protected:
  fV2 speed;
  fV2 previous_position;
  // scalars stored on speed change for quick access
  float speed_scalar;
  float angle;

  /// METHODS
public:
  // constructors, destructors
  MovementElement(Thing* init_owner);
  ~MovementElement();
  // main methods
  void springTowards(fV2 target_position, float spring_constant);
  void add_springTowards(fV2 target_position, float spring_constant);
  void moveTowards(fV2 target_position, float speed);
  void add_moveTowards(fV2 target_position, float speed);
  void add_speedScalar(float speed);
  void bounce(iV2 collision_side);
  virtual int update(GameState* context, float delta);
  // modification
  virtual void setSpeed(fV2 new_speed);
  void setSpeed_scalar(float new_speed_scalar);
  void setSpeedX(float new_speed);
  void setSpeedY(float new_speed);
  void addSpeed(fV2 force);
  void addSpeedX(float force);
  void addSpeedY(float force);
  // query
  float getSpeedX() const;
  float getSpeedY() const;
  fV2 getSpeed() const;
  fV2 getPrevPos() const;
  float getSpeed_scalar() const;
  float getAngle() const;

};

#endif // MOVEMENTELEMENT_HPP_INCLUDED
