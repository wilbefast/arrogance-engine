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

#include "Angle.hpp"

#include "wjd_math.h"

//! CONSTRUCTORS, DESTRUCTORS
Angle::Angle() :
degrees(0),
radians(0.0)
{
}

Angle::Angle(int _degrees) :
degrees(_degrees),
radians(DEG2RAD(_degrees))
{
}

Angle::Angle(double _radians) :
degrees(RAD2DEG(_radians)),
radians(_radians)
{
}

Angle::Angle(int _degrees, double _radians) :
degrees(_degrees),
radians(_radians)
{
}

Angle::Angle(Angle const& other) :
degrees(other.degrees),
radians(other.radians)
{
}

//! CASTE OPERATORS

Angle::operator int() const
{
  return degrees;
}

Angle::operator double() const
{
  return radians;
}

//! ACCESSORS
int Angle::getDeg() const
{
  return degrees;
}

double Angle::getRad() const
{
  return radians;
}


//! AFFECTION
Angle& Angle::operator=(const int& deg)
{
  degrees = deg;
  radians = DEG2RAD(deg);
  return (*this);
}

Angle& Angle::operator=(const double& rad)
{
  degrees = ((int)RAD2DEG(rad))%360;
  radians = rad;
  return (*this);
}


//! ADDITION
Angle& Angle::operator+=(const int& deg)
{
  degrees = (degrees + deg)%360;
  radians = DEG2RAD(degrees);
  return (*this);
}

Angle Angle::operator+(const int& deg) const
{
  Angle copy(*this);
  copy += deg;
  return copy;
}

Angle& Angle::operator+=(const double& rad)
{
  radians += rad;
  degrees = ((int)RAD2DEG(rad))%360;
  return (*this);
}

Angle Angle::operator+(const double& rad) const
{
  Angle copy(*this);
  copy += rad;
  return copy;
}


//! SUBTRACTION
Angle& Angle::operator-=(const int& deg)
{
  degrees = (degrees - deg)%360;
  radians = DEG2RAD(degrees);
  return (*this);
}

Angle Angle::operator-(const int& deg) const
{
  Angle copy(*this);
  copy -= deg;
  return copy;
}

Angle& Angle::operator-=(const double& rad)
{
  radians -= rad;
  degrees = ((int)RAD2DEG(rad))%360;
  return (*this);
}

Angle Angle::operator-(const double& rad) const
{
  Angle copy(*this);
  copy -= rad;
  return copy;
}

//! MULTIPLICATION
Angle& Angle::operator*=(const int& deg)
{
  degrees = (degrees * deg)%360;
  radians = DEG2RAD(degrees);
  return (*this);
}

Angle Angle::operator*(const int& deg) const
{
  Angle copy(*this);
  copy *= deg;
  return copy;
}

Angle& Angle::operator*=(const double& rad)
{
  radians *= rad;
  degrees = ((int)RAD2DEG(rad))%360;
  return (*this);
}

Angle Angle::operator*(const double& rad) const
{
  Angle copy(*this);
  copy *= rad;
  return copy;
}

//! DIVISION
Angle& Angle::operator/=(const int& deg)
{
  degrees = (degrees / deg)%360;
  radians = DEG2RAD(degrees);
  return (*this);
}

Angle Angle::operator/(const int& deg) const
{
  Angle copy(*this);
  copy /= deg;
  return copy;
}

Angle& Angle::operator/=(const double& rad)
{
  radians /= rad;
  degrees = ((int)RAD2DEG(rad))%360;
  return (*this);
}

Angle Angle::operator/(const double& rad) const
{
  Angle copy(*this);
  copy /= rad;
  return copy;
}
