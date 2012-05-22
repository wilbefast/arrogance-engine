// Inline file: a special implementation that is included

#include "wjd_math.hpp"

using namespace std;

// Constructors

// Null vector
template <typename T>
inline V3<T>::V3() :
x(0), y(0), z(0)
{
}

// Basic constructor. Ex: iV3(3,3,3);
template <typename T>
inline V3<T>::V3(T _x, T _y, T _z) :
x(_x), y(_y), z(_z)
{
}

// Read from stream constructor. Ex(iV3(string))
template <typename T>
inline V3<T>::V3(std::istream& in)
{
  read(in);
}

// Copy constructor. Ex: iV3 v2(v1);
template<typename T>
template<typename U>
inline V3<T>::V3(const V3<U>& source) :
x(static_cast<T>(source.x)),
y(static_cast<T>(source.y)),
z(static_cast<T>(source.z))
{
}

// Static cast operator from V3<T> to V3<U>. Ex: (uV3)vt;
template<typename T>
template<typename U>
inline V3<T>::operator V3<U>() const
{
    V3<U> result;
    result.x = (U) x;
    result.y = (U) y;
    result.z = (U) z;
    return result; // Return type is unspecified because it is fixed
}

// Evalulate a vector as a bool (false if null-vector)
template<typename T>
inline V3<T>::operator bool() const
{
    return (x || y || z);
}

// Caste by affection from V3<U> to V3<T>. Ex: vt = vu;
template<typename T>
template <typename U>
inline V3<T>& V3<T>::operator= (const V3<U> &source)
{
    x = (T)source.x;
    y = (T)source.y;
    z = (T)source.z;
    return *this;
}

// Vector addition and subtraction
template <typename T>
inline V3<T>& V3<T>::operator+=(V3 const& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return (*this);
}

template <typename T>
inline V3<T> V3<T>::operator+(V3<T> const& other) const
{
    V3<T> copy(other);
    copy += (*this);
    return copy;
}

template <typename T>
inline V3<T>& V3<T>::operator-=(V3<T> const& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;

    return (*this);
}

template <typename T>
inline V3<T> V3<T>::operator-(V3<T> const& other) const
{
    V3<T> copy(*this);
    copy -= (other);
    return copy;
}

template <typename T>
inline V3<T> V3<T>::operator-() const
{
    return V3<T>(-x, -y, -z);
}

// Multiplication/division by scalar
template <typename T>
inline V3<T>& V3<T>::operator*=(T k)
{
    this->x *= k;
    this->y *= k;
    this->z *= k;

    return (*this);
}

template <typename T>
inline V3<T> V3<T>::operator*(T k) const
{
    V3<T> copy(*this);
    copy *= k;
    return copy;
}

template <typename T>
inline V3<T>& V3<T>::operator/=(T k)
{
  T inv = 1/k;
  this->x *= inv;
  this->y *= inv;
  this->z *= inv;

  return (*this);
}

template <typename T>
inline V3<T> V3<T>::operator/(T k) const
{
    V3<T> copy(*this);
    copy /= k;
    return copy;
}

// Element-wise multiplication, division
template <typename T>
inline V3<T>& V3<T>::operator*=(V3 const& other)
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;

    return (*this);
}

template <typename T>
inline V3<T> V3<T>::operator*(V3<T> const& other) const
{
    V3<T> copy(other);
    copy *= (*this);
    return copy;
}

template <typename T>
inline V3<T>& V3<T>::operator/=(V3<T> const& other)
{
  this->x /= other.x;
  this->y /= other.y;
  this->z /= other.z;

  return (*this);
}

template <typename T>
inline V3<T> V3<T>::operator/(V3<T> const& other) const
{
    V3<T> copy(*this);
    copy /= (other);
    return copy;
}

/* INCREMENTATION AND DECREMENTATION */

template <typename T>
inline V3<T>& V3<T>::operator++()
{
  x++;
  y++;
  z++;
  return(*this);
}

template <typename T>
inline V3<T>& V3<T>::operator--()
{
  x--;
  y--;
  z--;
  return(*this);
}

template <typename T>
inline V3<T> V3<T>::operator++(int)
{
  V3<T> copy(*this);
  x++;
  y++;
  z++;
  return copy;
}

template <typename T>
inline V3<T> V3<T>::operator--(int)
{
  V3<T> copy(*this);
  x--;
  y--;
  z--;
  return copy;
}

/* ARRAY-STYLE ACCESS */

template <typename T>
inline T& V3<T>::operator[](size_t i)
{
  switch(i)
  {
    case 0: return x;
    case 1: return y;
    case 2: default: return z;
  }
}

template <typename T>
inline const T& V3<T>::operator[](size_t i) const
{
  switch(i)
  {
    case 0: return x;
    case 1: return y;
    case 2: default: return z;
  }
}



/* ACCESSORS */

// Create a vector of absolute values
template <typename T>
inline V3<T> V3<T>::abs() const
{
    return V3<T>(ABS(x), ABS(y), ABS(z));
}

// Create a "sign" vector, containing the signs of the x and y values
template <typename T>
inline V3<int> V3<T>::sign() const
{
    return V3<int>(SIGN(x), SIGN(y), SIGN(z));
}

// Calculate the norm (length) of the Vector
template <typename T>
inline float V3<T>::getNorm() const
{
    return sqrt(this->getNorm2());
}

template <typename T>
inline float V3<T>::getNorm2() const
{
    // This is less expensive since there is no sqrt to perform
    return x*x + y*y + z*z;
}

template <typename T>
inline void V3<T>::setMagnitude(float new_magnitude)
{
    (*this) *= (new_magnitude/getNorm());
}

/* OUTSTREAM OPERATOR */

template <typename T>
inline void V3<T>::print(ostream& stream) const
{
    stream << '(' << x << ',' << y << ',' << z << ')';
}

template <typename T>
inline ostream& operator<<(std::ostream& stream, V3<T> const& v)
{
    v.print(stream);
    return stream;
}

/* INSTREAM OPERATOR */

template <typename T>
inline void V3<T>::read(std::istream& in)
{
  in >> x >> y >> z;
}

template <typename T>
std::istream& operator>>(std::istream& in, V3<T>& v)
{
  v.read(in);
  return in;
}
