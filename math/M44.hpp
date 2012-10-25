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

#ifndef M44_HPP_INCLUDED
#define M44_HPP_INCLUDED

#include <iostream>

#include "V4.hpp"

// Simple vector classes
template <typename T>
class M44
{
  /* FUNCTIONS */
public:
  static M44 xRotMatrix(double angle);
  static M44 yRotMatrix(double angle);
  static M44 zRotMatrix(double angle);
  static M44 rotMatrix(double angle, V4<T> axis);

  /* ATTRIBUTES */
public:
  V4<T> col[4];

  /* METHODS */
public:
  // Constructors
  M44();
  M44(V4<T> c1, V4<T> c2, V4<T> c3, V4<T> c4);
  M44(V4<T> _col[]);
  M44(std::istream& in);

  template <typename U>
  explicit M44(const M44<U>& source);

  // Cast operator: Conversion from one Matrix type to another ;D
  template <typename U>
  operator M44<U>() const;

  // Conversion to a boolean
  operator bool() const;

  // Arithmetic operators
  template <typename U>
  M44& operator= (const M44<U>& source);
  // vector addition
  M44& operator+=(M44 const& other);
  M44 operator+(M44 const& other) const;
  M44& operator-=(M44 const& other);
  M44 operator-(M44 const& other) const;
  M44 operator-() const;   // unary minus or "additive inverse"
  // scaling
  M44& operator*=(T const& k);
  M44 operator*(T const& k) const;
  M44& operator/=(T const& k);
  M44 operator/(T const& k) const;
  // matrix-vector multiplication
  V4<T> operator*(V4<T> const& v) const;
  // matrix-matrix multiplication
  M44& operator*=(M44 const& m) const;
  M44 operator*(M44 const& m) const;

  // Array-style access
  V4<T> row(size_t i) const;
  M44& setRow(size_t i, V4<T> new_row);
  V4<T>* front();
  V4<T>& operator[](size_t i);
  const V4<T>& operator[](size_t i) const;

  // Utilities
  M44 getTranspose() const;
  M44 getInverse() const;
  bool isScaleFree() const;
  M44& toIdentity();
  M44& orthogonalise();
  void exportArray(T array[]) const;
  M44& importArray(T array[]);

  // outstream
  void print(std::ostream& out) const;

  // instream
  void read(std::istream& in);
};

// static functions
template <typename T> M44<T> transMatrix(V4<T> v);
template <typename T> M44<T> scaleMatrix(T xscale, T yscale, T zscale);

// outstream operator
template <typename T>
std::ostream& operator<<(std::ostream& out, M44<T> const& m);

// instream operator
template <typename T>
std::istream& operator>>(std::istream& in, M44<T>& m);


// common types
typedef M44<int> iM44;
typedef M44<unsigned int> uM44;
typedef M44<float> fM44;
typedef M44<double> dM44;

// NB - "Inline" files are implementations that are included rather than
// compiled. They are especially useful for templates.
#include "M44.inl"

#endif // M44_HPP_INCLUDED
