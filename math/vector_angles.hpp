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

#ifndef VECTOR_ANGLES_HPP_INCLUDED
#define VECTOR_ANGLES_HPP_INCLUDED

#include "V2.hpp"

void addAngle(fV2& current_direction, double turn);

double angleBetween(fV2 const& a, fV2 const& b);

bool turn(fV2& curent_direction, fV2 desired_direction, double max_turn);

#endif // VECTOR_ANGLES_HPP_INCLUDED
