/*
Copyright (C) 2012 William James Dyce

This program is free software: you can redistribute it and/or modify
it under he terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "vector_angles.hpp"
#include "wjd_math.h"

void addAngle(fV2& current_direction, double turn)
{
  double tcos = cos(turn), tsin = sin(turn);
  current_direction.x = current_direction.x * tcos - current_direction.y * tsin;
  current_direction.y = current_direction.x * tsin + current_direction.y * tcos;
}

double angleBetween(fV2 const& a, fV2 const& b)
{
  fV2 na(a), nb(b);
  na.normalise();
  nb.normalise();
  return acos(dot(na, nb));
}

bool turn(fV2& curent_direction, fV2 desired_direction, double max_turn)
{
  // calculate angle between the two
  double angle_between = angleBetween(curent_direction, desired_direction);
  if(angle_between < 0.01) // about 0.6 degrees
    return true;

  // circular interpolation
  if(dot(curent_direction, desired_direction) > 0)
    // turn right
    addAngle(curent_direction, -std::min(max_turn, angle_between));
  else
    // turn left
    addAngle(curent_direction, std::min(max_turn, angle_between));

  return false;
}
