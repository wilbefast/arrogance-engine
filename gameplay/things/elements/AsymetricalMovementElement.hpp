#ifndef ASYMETRICALMOVEMENTELEMENT_HPP_INCLUDED
#define ASYMETRICALMOVEMENTELEMENT_HPP_INCLUDED

#include "MovementElement.hpp"

class AsymetricalMovementElement : public MovementElement
{
  /// ATTRIBUTES
private:
  // pseudo-constant set upon creation of object
  fV2 speed_max;
  fV2 speed_min;
  fV2 friction;

  /// METHODS
public:
  // creation, destruction
  AsymetricalMovementElement(Thing* _owner, fV2 _speed_max,
                  fV2 _friction = fV2(1.0f, 1.0f),
                  fV2 _speed_min = fV2(DEFAULT_SPEED_MIN, DEFAULT_SPEED_MIN));
  // main methods
  virtual int update(GameState* context, float delta);
  // modification
  virtual void setSpeed(fV2 new_speed);
};

#endif // ASYMETRICALMOVEMENTELEMENT_HPP_INCLUDED
