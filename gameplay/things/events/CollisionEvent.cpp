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

#include "CollisionEvent.hpp"

/// FUNCTIONS

void CollisionEvent::generate(Thing* a, Thing* b)
{
  // Save the side upon which the event occured
  iV2 side;

  // Check for collisions
  if(!a->isDead() && !b->isDead() && a->isColliding(b, &side))
  {
    // Generate collision events
    a->addEvent(new CollisionEvent(b, side));
    b->addEvent(new CollisionEvent(a, -side));
  }
}

/// METHODS

// constructors, destructor

CollisionEvent::CollisionEvent(Thing* init_other, iV2 init_side) :
ThingEvent("collision"),
other(init_other),
collision_side(init_side)
{
}

// accessors
Thing* CollisionEvent::getOther()
{
  return other;
}

iV2 CollisionEvent::getSide() const
{
  return collision_side;
}
