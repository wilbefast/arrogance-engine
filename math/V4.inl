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
inline V4<T>::V4() :
x(0), y(0), z(0), w(0)
{
}

// Basic constructor. Ex: iV4(3,3,3,0); for a vector,
// or iV4(3,3,3,1); for a point
template <typename T>
inline V4<T>::V4(T _x, T _y, T _z, T _w) :
x(_x), y(_y), z(_z), w(_w)
{
}

// Read from stream constructor: iV4(stringstream)
template <typename T>
inline V4<T>::V4(std::istream& in)
{
  read(in);
}

// Copy constructor. Ex: iV4 b(a);
template<typename T>
template<typename U>
inline V4<T>::V4(const V4<U>& source) :
x(static_cast<T>(source.x)),
y(static_cast<T>(source.y)),
z(static_cast<T>(source.z)),
w(static_cast<T>(source.w))
{
}

// Static cast operator from V4<T> to V4<U>. Ex: (uV4)vt;
template<typename T>
template<typename U>
inline V4<T>::operator V4<U>() const
{
  V4<U> result;
  result.x = (U) x;
  result.y = (U) y;
  result.z = (U) z;
  result.w = (U) w;
  return result; // Return type is unspecified because it is fixed
}

// Evaluate a vector as a bool (false if null-vector)
template<typename T>
inline V4<T>::operator bool() const
{
  return (x || y || z);
}

/* ARITHMETIC OPERATORS */

// Cast by affection from V4<U> to V4<T>. Ex: vt = vu;
template<typename T>
template <typename U>
inline V4<T>& V4<T>::operator= (const V4<U> &source)
{
  x = (T)source.x;
  y = (T)source.y;
  z = (T)source.z;
  w = (T)source.w;
  return *this;
}

// Vector addition and subtraction
template <typename T>
inline V4<T>& V4<T>::operator+=(V4 const& other)
{
  this->x += other.x;
  this->y += other.y;
  this->z += other.z;
  // NB - the w is not changed here !
  return (*this);
}

template <typename T>
inline V4<T> V4<T>::operator+(V4<T> const& other) const
{
  V4<T> copy(*this);
  copy += other;
  return copy;
}

template <typename T>
inline V4<T>& V4<T>::operator-=(V4<T> const& other)
{
  this->x -= other.x;
  this->y -= other.y;
  this->z -= other.z;
  // NB - the w is not changed here !
  return (*this);
}

template <typename T>
inline V4<T> V4<T>::operator-(V4<T> const& other) const
{
  V4<T> copy(*this);
  copy -= (other);
  return copy;
}

template <typename T>
inline V4<T> V4<T>::operator-() const
{
  return V4<T>(-x, -y, -z, w);
}

// Multiplication/division by scalar
template <typename T>
inline V4<T>& V4<T>::operator*=(T k)
{
  this->x *= k;
  this->y *= k;
  this->z *= k;
  // NB - the w is not changed here !
  return (*this);
}

template <typename T>
inline V4<T> V4<T>::operator*(T k) const
{
  V4<T> copy(*this);
  copy *= k;
  return copy;
}

template <typename T>
inline V4<T>& V4<T>::operator/=(T k)
{
  double inv = 1.0/k;
  this->x *= inv;
  this->y *= inv;
  this->z *= inv;
  // NB - the w is not changed here !
  return (*this);
}

template <typename T>
inline V4<T> V4<T>::operator/(T k) const
{
  V4<T> copy(*this);
  copy /= k;
  return copy;
}

// Element-wise multiplication, division
template <typename T>
inline V4<T>& V4<T>::operator*=(V4 const& other)
{
  this->x *= other.x;
  this->y *= other.y;
  this->z *= other.z;
  // NB - the w is not changed here !
  return (*this);
}

template <typename T>
inline V4<T> V4<T>::operator*(V4<T> const& other) const
{
  V4<T> copy(*this);
  copy *= other;
  return copy;
}

template <typename T>
inline V4<T>& V4<T>::operator/=(V4<T> const& other)
{
  this->x /= other.x;
  this->y /= other.y;
  this->z /= other.z;
  // NB - the w is not changed here !
  return (*this);
}

template <typename T>
inline V4<T> V4<T>::operator/(V4<T> const& other) const
{
  V4<T> copy(*this);
  copy /= (other);
  return copy;
}

/* INCREMENTATION AND DECREMENTATION */

template <typename T>
inline V4<T>& V4<T>::operator++()
{
  x++;
  y++;
  z++;
  // NB - the w is not changed here !
  return(*this);
}

template <typename T>
inline V4<T>& V4<T>::operator--()
{
  x--;
  y--;
  z--;
  // NB - the w is not changed here !
  return(*this);
}

template <typename T>
inline V4<T> V4<T>::operator++(int)
{
  V4<T> copy(*this);
  x++;
  y++;
  z++;
  // NB - the w is not changed here !
  return copy;
}

template <typename T>
inline V4<T> V4<T>::operator--(int)
{
  V4<T> copy(*this);
  x--;
  y--;
  z--;
  // NB - the w is not changed here !
  return copy;
}

/* ARRAY-STYLE ACCESS */

template <typename T>
inline T* V4<T>::front()
{
  return &x;
}

template <typename T>
inline T& V4<T>::operator[](size_t i)
{
  switch(i)
  {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: default: return w;
  }
}

template <typename T>
inline const T& V4<T>::operator[](size_t i) const
{
  switch(i)
  {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: default: return w;
  }
}



/* ACCESSORS */

// Create a vector of absolute values
template <typename T>
inline V4<T> V4<T>::abs() const
{
  return V4<T>(ABS(x), ABS(y), ABS(z), w);
}

// Create a "sign" vector, containing the signs of the x and y values
template <typename T>
inline V4<int> V4<T>::sign() const
{
  return V4<int>(SIGN(x), SIGN(y), SIGN(z), w);
}

// Calculate the norm (length) of the Vector
template <typename T>
inline float V4<T>::getNorm() const
{
  return sqrt(this->getNorm2());
}

template <typename T>
inline float V4<T>::getNorm2() const
{
  // This is less expensive since there is no sqrt to perform
  return x*x + y*y + z*z;
}

template <typename T>
inline V4<T>& V4<T>::setMagnitude(float new_magnitude)
{
  return ((*this) *= (new_magnitude/getNorm()));
}

template <typename T>
inline V4<T>& V4<T>::normalise()
{
  return (setMagnitude(1));
}

template <typename T>
inline V4<T> V4<T>::noW() const
{
  return V4<T>(x, y, z, 0);
}

/* STATIC FUNCTIONS */

// Scalar product -- ignore w
template <typename T>
float dot(V4<T>const a, V4<T>const b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Scalar product -- take w into account
template <typename T>
float wdot(V4<T>const a, V4<T>const b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

// Vector product
template <typename T>
V4<T> cross(V4<T>const a, V4<T>const b)
{
	return V4<T>(a.y*b.z - a.z*b.y,
                a.z*b.x - a.x*b.z,
                a.x*b.y - a.y*b.x,
                a.w || b.w);
}

// Linear interpolation
template <typename T>
V4<T> inter(V4<T>const a, V4<T>const b, T f)
{
  return V4<T>((b-a)*f + b);
}

/* OUTSTREAM OPERATOR */

template <typename T>
inline void V4<T>::print(std::ostream& stream) const
{
  stream << '(' << x << ',' << y << ',' << z << ',' << w << ')';
}

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, V4<T> const& v)
{
  v.print(stream);
  return stream;
}

/* INSTREAM OPERATOR */

template <typename T>
inline void V4<T>::read(std::istream& in)
{
  in >> x >> y >> z >> w;
}

template <typename T>
std::istream& operator>>(std::istream& in, V4<T>& v)
{
  v.read(in);
  return in;
}

