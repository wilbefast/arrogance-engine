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

#ifndef V2_HPP_INCLUDED
#define V2_HPP_INCLUDED

#include <iostream>

#define ORIGIN(type) V2<type>(0,0)
#define SQUARE(type, size) V2<type>(SIZE,SIZE)

// Simple vector classes
template <typename T>
class V2
{
  /* ATTRIBUTES */
public:
      T x, y;

  /* METHODS */
public:
  // Constructors
  V2();
  V2(T x, T y);
  V2(std::istream& in);

  template <typename U>
  explicit V2(const V2<U>& source);

  // Cast operator: Conversion from one Vector type to another ;D
  template <typename U>
  operator V2<U>() const;

  // Conversion to a boolean
  operator bool() const;

  // Arithmetic operators
  template <typename U>
  V2& operator= (const V2<U>& source);
  // vector addition
  V2& operator+=(V2 const& other);
  V2 operator+(V2 const& other) const;
  V2& operator-=(V2 const& other);
  V2 operator-(V2 const& other) const;
  V2 operator-() const;   // unary minus or "additive inverse"
  // scaling
  V2& operator*=(T k);
  V2 operator*(T k) const;
  V2& operator/=(T k);
  V2 operator/(T k) const;
  // element-wise multiplication, division
  V2& operator*=(V2 const& other);
  V2 operator*(V2 const& other) const;
  V2& operator/=(V2 const& other);
  V2 operator/(V2 const& other) const;

  // accessors
  V2 abs() const;
  V2<int> sign() const;
  float getAngle() const;
  float getNorm() const;
  float getNorm2() const;
  void setMagnitude(float new_magnitude);

  // outstream
  void print(std::ostream& stream) const;

  // instream
  void read(std::istream& in);
};

  // outstream operator
template <typename T>
std::ostream& operator<<(std::ostream& stream, V2<T> const& v);

// instream operator
template <typename T>
std::istream& operator>>(std::istream& in, V2<T>& v);

typedef V2<int> iV2;
typedef V2<unsigned int> uV2;
typedef V2<float> fV2;
typedef V2<double> dV2;

// NB - "Inline" files are implementations that are included rather than
// compiled. They are especially useful for templates.
#include "V2.inl"

#endif // V2_HPP_INCLUDED

