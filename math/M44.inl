/*
Arrogance Engine: a simple SDL/OpenGL game engine for Desktop and Android.
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

// Inline file: a special implementation that is included

#include "wjd_math.hpp"

using namespace std;

// Constructors

// Uninitialised matrix
template <typename T>
inline M44<T>::M44()
// NB - garbage in cells!
{
}

// Construction from a series of cols
template <typename T>
M44<T>::M44(V4<T> r0, V4<T> r1, V4<T> r2, V4<T> r3)
{
  col[0] = r0;
  col[1] = r1;
  col[2] = r2;
  col[3] = r3;
}

// Construction from an array of cols
template <typename T>
M44<T>::M44(V4<T> _col[])
{
  for(unsigned int i = 0; i < 4; i++)
    col[i] = _col[i];
}

// Read from stream constructor: iM44(stringstream)
template <typename T>
inline M44<T>::M44(std::istream& in)
{
  read(in);
}

// Copy constructor. Ex: iM44 m2(m1);
template<typename T>
template<typename U>
inline M44<T>::M44(const M44<U>& source)
{
  for(unsigned int i = 0; i < 4; i++)
    col[i] = source.col[i];
}

// Static cast operator from iM44<T> to iM44<U>. Ex: (iM44)mt;
template<typename T>
template<typename U>
inline M44<T>::operator M44<U>() const
{
  M44<U> result;
  for(unsigned int i = 0; i < 4; i++)
    result.col[i] = (U)col[i];
  return result; // Return type is unspecified because it is fixed
}

// Evaluate a vector as a bool (false if null-vector)
template<typename T>
inline M44<T>::operator bool() const
{
  return (col[0] || col[1] || col[2] || col[3]);
}

/* ARITHMETIC OPERATORS */

// Cast by affection from M44<U> to M44<T>. Ex: mt = mu;
template<typename T>
template <typename U>
inline M44<T>& M44<T>::operator= (const M44<U> &source)
{
  for(unsigned int i = 0; i < 4; i++)
    col[i] = (T)source.col[i];
  return (*this);
}

// Matrix addition and subtraction
template <typename T>
inline M44<T>& M44<T>::operator+=(M44 const& other)
{
  for(unsigned int i = 0; i < 4; i++)
  {
    col[i] += other.col[i];
    col[i][3] += other.col[i][3]; // NB - w was not touched by V4's operators
  }
  return (*this);
}

template <typename T>
inline M44<T> M44<T>::operator+(M44<T> const& other) const
{
  M44<T> copy(*this);
  copy += other;
  return copy;
}

template <typename T>
inline M44<T>& M44<T>::operator-=(M44 const& other)
{
  for(unsigned int i = 0; i < 4; i++)
  {
    col[i] -= other.col[i];
    col[i][3] -= other.col[i][3]; // NB - w was not touched by V4's operators
  }
  return (*this);
}

template <typename T>
inline M44<T> M44<T>::operator-(M44<T> const& other) const
{
  M44<T> copy(*this);
  copy -= other;
  return copy;
}

template <typename T>
inline M44<T> M44<T>::operator-() const
{
  return M44<T>(-col[0], -col[1], -col[2],
                  V4<T>(-col[3][0], -col[3][1], -col[3][2], -col[3][3]));
                  // NB - w was not touched by V4's operators
}

// Multiplication/division by scalar
template <typename T>
inline M44<T>& M44<T>::operator*=(T const& k)
{
  for(unsigned int i = 0; i < 4; i++)
  {
    col[i] *= k;
    col[i][3] *= k; // NB - w was not touched by V4's operators
  }
  return (*this);
}

template <typename T>
inline M44<T> M44<T>::operator*(T const& k) const
{
  M44<T> copy(*this);
  copy *= k;
  return copy;
}

template <typename T>
inline M44<T>& M44<T>::operator/=(T const& k)
{
  double inv = 1.0/k;
  for(unsigned int i = 0; i < 4; i++)
  {
    col[i] *= inv;
    col[i][3] *= inv; // NB - w is not touched by V4's operators
  }
  // NB - the w is not changed here !
  return (*this);
}

template <typename T>
inline M44<T> M44<T>::operator/(T const& k) const
{
  M44<T> copy(*this);
  copy /= k;
  return copy;
}

// Matrix-vector multiplication
template <typename T>
V4<T> M44<T>::operator*(V4<T> const& v) const
{
  return V4<T>( wdot(v, row(0)),
                wdot(v, row(1)),
                wdot(v, row(2)),
                wdot(v, row(3)));
}


// Matrix-matrix multiplication
template <typename T>
M44<T>& M44<T>::operator*=(M44<T> const& other) const
{
  (*this) = ((*this)*other);
}

template <typename T>
M44<T> M44<T>::operator*(M44<T> const& other) const
{
  return M44<T>(V4<T>((*this)*other.col[0]),
                  V4<T>((*this)*other.col[1]),
                  V4<T>((*this)*other.col[2]),
                  V4<T>((*this)*other.col[3]));
}

/* ARRAY-STYLE ACCESS */

template <typename T>
inline V4<T> M44<T>::row(size_t i) const
{
  if(i >= 4)
    i = 3;
  return V4<T>(col[0][i], col[1][i], col[2][i], col[3][i]);
}

template <typename T>
inline M44<T>& M44<T>::setRow(size_t i, V4<T> new_row)
{
  for(size_t c = 0; c < 4; c++)
    col[c][i] = new_row[c];
  return (*this);
}

template <typename T>
inline V4<T>* M44<T>::front()
{
  return col; // equivalent to &(col[0])
}

template <typename T>
inline V4<T>& M44<T>::operator[](size_t i)
{
  return (i >= 4) ? col[3] : col[i];
}

template <typename T>
inline const V4<T>& M44<T>::operator[](size_t i) const
{
  return (i >= 4) ? col[3] : col[i];
}

/* UTILITIES */

template <typename T>
inline M44<T> M44<T>::getTranspose() const
{
  return M44<T>(row(0), row(1), row(2), row(3));
}

template <typename T>
inline M44<T> M44<T>::getInverse() const
{
  if(!isScaleFree())
    cout << "MATRIX IS NOT SCALE FREE!" << endl;

  M44<T> Rt =
    M44<T>(col[0].noW(), col[1].noW(), col[2].noW(), V4<T>()).getTranspose();
  V4<T> invT = -Rt*col[3]; invT.w = 1;
  return M44<T>(Rt.col[0], Rt.col[1], Rt.col[2], invT);
}

template <typename T>
inline bool M44<T>::isScaleFree() const
{
  V4<T> row0 = row(0), row1 = row(1), row2 = row(2);
  return(row0.getNorm2() == 1
         && row1.getNorm2() == 1
         && row2.getNorm2() == 1
         && dot(row0, row1) == 0
         && dot(row0, row2) == 0
         && dot(row1, row2) == 0);
}

template <typename T>
inline M44<T>& M44<T>::toIdentity()
{
  for(size_t r = 0; r < 4; r++) for(size_t c = 0; c < 4; c++)
    col[c][r] = (r == c) ? 1 : 0;
  return (*this);
}

template <typename T>
inline M44<T>& M44<T>::orthogonalise()
{
  // we want ensure 0 perpendicular to 1 and 2, and 1 to 2.
  setRow(2, cross(row(1), row(2))); // 0 and 1 perpendicular to 2
  setRow(1, cross(row(0), row(2))); // 1 perpendicular to 2
  return (*this);
}


/* STATIC FUNCTIONS */

template <typename T>
inline M44<T> M44<T>::xRotMatrix(double angle)
{
  double cs = cos(angle), sn = sin(angle);
  return M44<T>(V4<T>(1,0,0),
                V4<T>(0,cs,-sn),
                V4<T>(0,cs,sn),
                V4<T>(0,0,0,1));
}

template <typename T>
inline M44<T> M44<T>::yRotMatrix(double angle)
{
  double cs = cos(angle), sn = sin(angle);
  return M44<T>(V4<T>(cs,0,sn),
                V4<T>(0,1,0),
                V4<T>(-sn,0,cs),
                V4<T>(0,0,0,1));
}

template <typename T>
inline M44<T> M44<T>::zRotMatrix(double angle)
{
  double cs = cos(angle), sn = sin(angle);
  return M44<T>(V4<T>(cs,-sn,0),
                V4<T>(sn,cs,0),
                V4<T>(0,0,1),
                V4<T>(0,0,0,1));
}

template <typename T>
inline M44<T> M44<T>::rotMatrix(double angle, V4<T> ax)
{
  T cs = cos(angle), sn = sin(angle), opcs = 1-cs,
         XX = ax.x*ax.x, XY = ax.x*ax.y, XZ = ax.x*ax.z,
         YY = ax.y*ax.y, YZ = ax.y*ax.z, ZZ = ax.z*ax.z;


  return M44<T>(V4<T>(opcs*XX + cs, opcs*XY - ax.z*sn, opcs*XZ + ax.y*sn),
                V4<T>(opcs*XY + ax.z*sn, opcs*YY + cs, opcs*YZ - ax.x*sn),
                V4<T>(opcs*XZ - ax.y*sn, opcs*YZ + ax.x*sn, opcs*ZZ + cs),
                V4<T>(0,0,0,1));
}

template <typename T>
inline M44<T> transMatrix(V4<T> v)
{
  return M44<T>(V4<T>(1,0,0,0),
                V4<T>(0,1,0,0),
                V4<T>(0,0,1,0),
                V4<T>(v.x,v.y,v.z,1));
}

template <typename T>
inline M44<T> scaleMatrix(T xscale, T yscale, T zscale)
{
  return M44<T>(V4<T>(xscale,0,0,0),
                V4<T>(0,yscale,0,0),
                V4<T>(0,0,zscale,0),
                V4<T>(0,0,0,1));
}


/* OUTSTREAM OPERATOR */

template <typename T>
inline void M44<T>::print(ostream& stream) const
{
  stream << endl;
  for(size_t i = 0; i < 4; i ++)
  {
    stream << '[' << row(i) << ']';
    if(i != 3)
      stream << endl;
  }
}

template <typename T>
inline ostream& operator<<(std::ostream& stream, M44<T> const& m)
{
  m.print(stream);
  return stream;
}

/* INSTREAM OPERATOR */

template <typename T>
inline void M44<T>::read(std::istream& in)
{
  in >> col[0] >> col[1] >> col[2] >> col[3];
}

template <typename T>
std::istream& operator>>(std::istream& in, M44<T>& m)
{
  m.read(in);
  return in;
}
