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

// Inline file: a special implementation that is included

#include "wjd_math.h"

// Constructors

// Null vector
template <typename T>
inline V2<T>::V2() :
x(0), y(0)
{
}

// Basic constructor. Ex: iV2(3,3);
template <typename T>
inline V2<T>::V2(T xx, T yy) :
x(xx), y(yy)
{
}

// Read from stream constructor: iV2(stringstream)
template <typename T>
inline V2<T>::V2(std::istream& in)
{
  read(in);
}

// Copy constructor. Ex: iV2 v2(v1);
template<typename T>
template<typename U>
inline V2<T>::V2(const V2<U>& source) :
x(static_cast<T>(source.x)),
y(static_cast<T>(source.y))
{
}

// Static cast operator from V2<T> to V2<U>. Ex: (uV2)vt;
template<typename T>
template<typename U>
inline V2<T>::operator V2<U>() const
{
  V2<U> result;
  result.x = (U) x;
  result.y = (U) y;
  return result; // Return type is unspecified because it is fixed
}

// Evaluate a vector as a bool (false if null-vector)
template<typename T>
inline V2<T>::operator bool() const
{
  return (x || y);
}

// Cast by affection from V2<U> to V2<T>. Ex: vt = vu;
template<typename T>
template <typename U>
inline V2<T>& V2<T>::operator= (const V2<U> &source)
{
  x = (T)source.x;
  y = (T)source.y;
  return *this;
}

/* ARITHMETIC OPERATORS */

// Vector addition and subtraction
template <typename T>
inline V2<T>& V2<T>::operator+=(V2 const& other)
{
  this->x += other.x;
  this->y += other.y;

  return (*this);
}

template <typename T>
inline V2<T> V2<T>::operator+(V2<T> const& other) const
{
  V2<T> copy(*this);
  copy += other;
  return copy;
}

template <typename T>
inline V2<T>& V2<T>::operator-=(V2<T> const& other)
{
  this->x -= other.x;
  this->y -= other.y;

  return (*this);
}

template <typename T>
inline V2<T> V2<T>::operator-(V2<T> const& other) const
{
  V2<T> copy(*this);
  copy -= (other);
  return copy;
}

template <typename T>
inline V2<T> V2<T>::operator-() const
{
  return V2<T>(-x, -y);
}

// Multiplication/division by scalar
template <typename T>
inline V2<T>& V2<T>::operator*=(T k)
{
  this->x *= k;
  this->y *= k;

  return (*this);
}

template <typename T>
inline V2<T> V2<T>::operator*(T k) const
{
  V2<T> copy(*this);
  copy *= k;
  return copy;
}

template <typename T>
inline V2<T>& V2<T>::operator/=(T k)
{
  T inv = 1/k;
  this->x *= inv;
  this->y *= inv;

  return (*this);
}

template <typename T>
inline V2<T> V2<T>::operator/(T k) const
{
  V2<T> copy(*this);
  copy /= k;
  return copy;
}

// Element-wise multiplication, division
template <typename T>
inline V2<T>& V2<T>::operator*=(V2 const& other)
{
  this->x *= other.x;
  this->y *= other.y;

  return (*this);
}

template <typename T>
inline V2<T> V2<T>::operator*(V2<T> const& other) const
{
  V2<T> copy(*this);
  copy *= other;
  return copy;
}

template <typename T>
inline V2<T>& V2<T>::operator/=(V2<T> const& other)
{
  this->x /= other.x;
  this->y /= other.y;

  return (*this);
}

template <typename T>
inline V2<T> V2<T>::operator/(V2<T> const& other) const
{
  V2<T> copy(*this);
  copy /= (other);
  return copy;
}

/* INCREMENTATION AND DECREMENTATION */

template <typename T>
inline V2<T>& V2<T>::operator++()
{
  x++;
  y++;
  return(*this);
}

template <typename T>
inline V2<T>& V2<T>::operator--()
{
  x--;
  y--;
  return(*this);
}

template <typename T>
inline V2<T> V2<T>::operator++(int)
{
  V2<T> copy(*this);
  x++;
  y++;
  return copy;
}

template <typename T>
inline V2<T> V2<T>::operator--(int)
{
  V2<T> copy(*this);
  x--;
  y--;
  return copy;
}

/* ARRAY-STYLE ACCESS */

template <typename T>
inline T* V2<T>::front()
{
  return &x;
}

template <typename T>
inline T& V2<T>::operator[](size_t i)
{
  switch(i)
  {
    case 0: return x;
    case 1: default: return y;
  }
}

template <typename T>
inline const T& V2<T>::operator[](size_t i) const
{
  switch(i)
  {
    case 0: return x;
    case 1: default: return y;
  }
}

/* ACCESSORS */

// Create a vector of absolute values
template <typename T>
inline V2<T> V2<T>::abs() const
{
  return V2<T>(ABS(x), ABS(y));
}

// Create a "sign" vector, containing the signs of the x and y values
template <typename T>
inline V2<int> V2<T>::sign() const
{
  return V2<int>(SIGN(x), SIGN(y));
}

// Calculate the angle (degrees) corresponding to this Vector
template <typename T>
inline float V2<T>::getAngle() const
{
  return RAD2DEG(atan2(y, x));
}

// Calculate the norm (length) of the Vector
template <typename T>
inline float V2<T>::getNorm() const
{
  return sqrt(this->getNorm2());
}

template <typename T>
inline float V2<T>::getNorm2() const
{
  // This is less expensive since there is no sqrt to perform
  return x*x + y*y;
}

template <typename T>
inline V2<T>& V2<T>::setMagnitude(float new_magnitude)
{
  float norm = getNorm();
  return ((*this) *= ((norm == 0) ? 0 : (new_magnitude/getNorm())));
}

template <typename T>
inline V2<T>& V2<T>::normalise()
{
  return (setMagnitude(1));
}

/* STATIC FUNCTIONS */


// Scalar product
template <typename T>
float dot(V2<T>const a, V2<T>const b)
{
	return a.x*b.x + a.y*b.y;
}

// Vector product
template <typename T>
V2<T> cross(V2<T>const a, V2<T>const b)
{
	return V2<T>(a.x*b.y - a.y*b.x, a.y*b.x - a.y*b.x);
}

// Determinant
template <typename T>
T det(V2<T>const a, V2<T>const b)
{
	return (a.x*b.y - a.y*b.x);
}

// Linear interpolation
template <typename T>
V2<T> inter(V2<T>const a, V2<T>const b, T f)
{
  return V2<T>((b-a)*f + b);
}

/* OUTSTREAM OPERATOR */

template <typename T>
inline void V2<T>::print(std::ostream& stream) const
{
  stream << '(' << x << ',' << y << ')';
}

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, V2<T> const& v)
{
  v.print(stream);
  return stream;
}

/* INSTREAM OPERATOR */

template <typename T>
inline void V2<T>::read(std::istream& in)
{
  in >> x >> y;
}

template <typename T>
std::istream& operator>>(std::istream& in, V2<T>& v)
{
  v.read(in);
  return in;
}

