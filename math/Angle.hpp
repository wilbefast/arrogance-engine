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

#ifndef ANGLE_HPP_INCLUDED
#define ANGLE_HPP_INCLUDED

class Angle
{
  //! ATTRIBUTES
private:
  int degrees;
  double radians;

  //! METHODS
public:
  // constructors, destructors
  Angle();
  Angle(int degrees);
  Angle(double radians);
  Angle(int degrees, double radians);
  Angle(Angle const& other);
  // cast operators
  operator int() const;
  operator double() const;
  // accessors
  int getDeg() const;
  double getRad() const;
  // affection
  Angle& operator=(const int& deg);
  Angle& operator=(const double& rad);
  // addition
  Angle& operator+=(const int& deg);
  Angle operator+(const int& deg) const;
  Angle& operator+=(const double& rad);
  Angle operator+(const double& rad) const;
  // subtraction
  Angle& operator-=(const int& deg);
  Angle operator-(const int& deg) const;
  Angle& operator-=(const double& rad);
  Angle operator-(const double& rad) const;
  // multiplication
  Angle& operator*=(const int& deg);
  Angle operator*(const int& deg) const;
  Angle& operator*=(const double& rad);
  Angle operator*(const double& rad) const;
  // division
  Angle& operator/=(const int& deg);
  Angle operator/(const int& deg) const;
  Angle& operator/=(const double& rad);
  Angle operator/(const double& rad) const;
};

#endif // ANGLE_HPP_INCLUDED
