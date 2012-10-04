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

#include "BoundaryEvent.hpp"
#include "../Thing.hpp"

/// FUNCTIONS

void BoundaryEvent::generate(Thing* t, fRect* boundary)
{
  // Save the side upon which the event occured
  iV2 side;

  // Generate out of bounds event for the Thing
  if(t->isOutside(boundary, &side))
    t->addEvent(new BoundaryEvent("out_of_bounds", side));
  else
  // Generate intersect boundary event for the Thing
  if(t->isLeaving(boundary, &side))
    t->addEvent(new BoundaryEvent("intersect_bounds", side));
}

/// METHODS

// constructors, destructors

BoundaryEvent::BoundaryEvent(const char* init_name, iV2 init_side) :
ThingEvent(init_name),
boundary_side(init_side)
{
}

// accessors

iV2 BoundaryEvent::getSide() const
{
  return boundary_side;
}
