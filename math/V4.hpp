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

#ifndef V4_HPP_INCLUDED
#define V4_HPP_INCLUDED

#include <iostream>

#define ORIGIN4(type) V4<type>(0,0,0)
#define OX4(type) V4<type>(1,0,0)
#define OY4(type) V4<type>(0,1,0)
#define OZ4(type) V4<type>(0,0,1)

#include "V3.hpp"

// Simple vector classes
template <typename T>
class V4
{
  /* CONSTANTS */

  /* ATTRIBUTES */
public:
  T x, y, z, w; // normally w = 0 for vectors or w = 1 for points

  /* METHODS */
public:
  // Constructors
  V4();
  V4(T x, T y, T z, T w = 0);
  V4(std::istream& in);
  V4(const V3<T>& source);

  template <typename U>
  explicit V4(const V4<U>& source);


  // Cast operator: Conversion from one Vector type to another ;D
  template <typename U>
  operator V4<U>() const;

  // Conversion to a boolean
  operator bool() const;

  // Arithmetic operators
  template <typename U>
  V4& operator= (const V4<U>& source);
  // vector addition
  V4& operator+=(V4 const& other);
  V4 operator+(V4 const& other) const;
  V4& operator-=(V4 const& other);
  V4 operator-(V4 const& other) const;
  V4 operator-() const;   // unary minus or "additive inverse"
  // scaling
  V4& operator*=(T k);
  V4 operator*(T k) const;
  V4& operator/=(T k);
  V4 operator/(T k) const;
  // element-wise multiplication, division
  V4& operator*=(V4 const& other);
  V4 operator*(V4 const& other) const;
  V4& operator/=(V4 const& other);
  V4 operator/(V4 const& other) const;
  // incrementation and decrementation
  V4& operator++(); // prefix
  V4& operator--();
  V4 operator++(int); // postfix
  V4 operator--(int);

  // Array-style access
  T* front();
  T& operator[](size_t i);
  const T& operator[](size_t i) const;

  // Accessors
  V4 abs() const;
  V4<int> sign() const;
  float getNorm() const;
  float getNorm2() const;
  V4<T>& setMagnitude(float new_magnitude);
  V4<T>& normalise();
  V4<T> noW() const;

  // outstream
  void print(std::ostream& out) const;

  // instream
  void read(std::istream& in);
};

// static functions
template <typename T> float dot(V4<T> const a, V4<T> const b);
template <typename T> float wdot(V4<T> const a, V4<T> const b);
template <typename T> V4<T> cross(V4<T> const a, V4<T> const b);
template <typename T> V4<T> inter(V4<T> const a, V4<T> const b, T f);

// outstream operator
template <typename T>
std::ostream& operator<<(std::ostream& out, V4<T> const& v);

// instream operator
template <typename T>
std::istream& operator>>(std::istream& in, V4<T>& v);

typedef V4<int> iV4;
typedef V4<unsigned int> uV4;
typedef V4<float> fV4;
typedef V4<double> dV4;

// NB - "Inline" files are implementations that are included rather than
// compiled. They are especially useful for templates.
#include "V4.inl"

#endif // V4_HPP_INCLUDED
