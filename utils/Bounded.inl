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

using namespace std;


#define BOUND(v, min, max) ((v > max) ? max : ((v < min) ? min : v))

//! CONSTRUCTORS

// Minimum set to 0 by default
template <typename T>
inline Bounded<T>::Bounded(T _max) :
value(0),
min(0),
max(_max)
{
}

// Minimum and maximum set explicitely
template <typename T>
inline Bounded<T>::Bounded(T _min, T _max) :
value(_min),
min(_min),
max(_max)
{
}

// Minimum, maximum and current value set explicitely
template <typename T>
inline Bounded<T>::Bounded(T _min, T _max, T _value) :
value(BOUND(value, min, max)),
min(_min),
max(_max)
{
}

// Read from stream constructor
template <typename T>
inline Bounded<T>::Bounded(std::istream& in)
{
  read(in);
}

// Copy constructor. Ex: iBounded b2(b1);
template<typename T>
template<typename U>
inline Bounded<T>::Bounded(const Bounded<U>& source) :
value(static_cast<T>(source.value)),
min(static_cast<T>(source.min)),
max(static_cast<T>(source.max))
{
}


//! CASTE OPERATORS

// Static cast operator from Bounded<T> to Bounded<U>. Ex: (uBounded)bt;
template<typename T>
template<typename U>
inline Bounded<T>::operator Bounded<U>() const
{
  Bounded<U> result;
  result.value = (U) value;
  result.min = (U) min;
  result.max = (U) max;
  return result;
}

// Get the value that has been bounded, ignoring the min and max
template<typename T> inline Bounded<T>::operator T() const
{
  return value;
}

// Cast by affection from Bounded<U> to Bounded<T>. Ex: bt = bu;
template<typename T>
template <typename U>
inline Bounded<T>& Bounded<T>::operator= (const Bounded<U> &source)
{
  value = (T)source.value;
  min = (T)source.min;
  max = (T)source.max;
  return *this;
}


//! ARITHMETIC OPERATORS

// Additions and subtractions
template<typename T> inline Bounded<T>& Bounded<T>::operator+=(const T& v)
{
  value = BOUND(value + v, min, max);
  return (*this);
}

template<typename T> inline T Bounded<T>::operator+(const T& v) const
{
  return value + v;
}

template<typename T> inline Bounded<T>& Bounded<T>::operator-=(const T& v)
{
  value = BOUND(value - v, min, max);
  return (*this);
}

template<typename T> inline T Bounded<T>::operator-(const T& v) const
{
  return value - v;
}

template<typename T> inline T Bounded<T>::operator-() const
{
  return -value;
}

// Multiplications and divisions
template<typename T> inline Bounded<T>& Bounded<T>::operator*=(const T& k)
{
  value = BOUND(value*k, min, max);
  return (*this);
}

template<typename T> inline T Bounded<T>::operator*(const T& k) const
{
  return value*k;
}

template<typename T> inline Bounded<T>& Bounded<T>::operator/=(const T& k)
{
  value = BOUND(value/k, min, max);
  return (*this);
}

template<typename T> inline T Bounded<T>::operator/(const T& k) const
{
  return value/k;
}

// incrementation and decrementation

 // prefix -- preincrement
template<typename T> inline Bounded<T>& Bounded<T>::operator++()
{
  if(value < max)
    value++;
  return (*this);
}

template<typename T> inline Bounded<T>& Bounded<T>::operator--()
{
  if(value > min)
    value--;
  return (*this);
}

 // postfix -- postincrement
template<typename T> inline T Bounded<T>::operator++(int)
{
  T old_value = value;
  ++(*this);
  return old_value;
}

template<typename T> inline T Bounded<T>::operator--(int)
{
  T old_value = value;
  --(*this);
  return old_value;
}


//! OUTSTREAM

template <typename T>
inline void Bounded<T>::print(ostream& stream) const
{
  stream << value << " in [" << min << ',' << max << ']';
}

template <typename T>
inline ostream& operator<<(std::ostream& stream, Bounded<T> const& b)
{
  b.print(stream);
  return stream;
}


//! INSTREAM

template <typename T>
inline void Bounded<T>::read(std::istream& in)
{
  in >> value >> min >> max;
  value = BOUND(value, min, max);
}

template <typename T>
std::istream& operator>>(std::istream& in, Bounded<T>& b)
{
  b.read(in);
  return in;
}
