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

#ifndef BOUNDED_HPP_INCLUDED
#define BOUNDED_HPP_INCLUDED

// Bounded value class
template <typename T>
class Bounded
{
  /* ATTRIBUTES */
private:
  T value, min, max;

  /* METHODS */
public:
  // Constructors
  Bounded(T _max);
  Bounded(T _min, T _max);
  Bounded(T _min, T _max, T _value);
  Bounded(std::istream& in);
  template <typename U> explicit Bounded(const Bounded<U>& source);

  // Conversions
  operator T() const;
  template <typename U> operator Bounded<U>() const;
  template <typename U> Bounded& operator= (const Bounded<U>& source);

  // Arithmetic operators
  // Additions and subtractions
  Bounded& operator+=(T v);
  T operator+(T v) const;
  Bounded& operator-=(T v);
  T operator-(T v) const;
  T operator-() const;   // unary minus or "additive inverse"
  // Multiplications and divisions
  Bounded& operator*=(T k);
  T operator*(T k) const;
  Bounded& operator/=(T k);
  T operator/(T k) const;

  // incrementation and decrementation
  Bounded& operator++(); // prefix -- preincrement
  Bounded& operator--();
  T operator++(int); // postfix -- postincrement
  T operator--(int);

  // outstream
  void print(std::ostream& stream) const;

  // instream
  void read(std::istream& in);
};

// outstream operator
template <typename T>
std::ostream& operator<<(std::ostream& stream, Bounded<T> const& b);

// instream operator
template <typename T>
std::istream& operator>>(std::istream& in, Bounded<T>& b);

typedef Bounded<int> iBounded;
typedef Bounded<unsigned int> uBounded;
typedef Bounded<float> fBounded;
typedef Bounded<double> dBounded;

// NB - "Inline" files are implementations that are included rather than
// compiled. They are especially useful for templates.
#include "Bounded.inl"

#endif // BOUNDED_HPP_INCLUDED
