#ifndef V3_HPP_INCLUDED
#define V3_HPP_INCLUDED

#include <iostream>

#define ORIGIN3(type) V3<type>(0,0,0)

// Simple vector classes
template <typename T>
class V3
{
  /* ATTRIBUTES */
public:
      T x, y, z;

  /* METHODS */
public:
  // Constructors
  V3();
  V3(T x, T y, T z);
  V3(std::istream& in);

  template <typename U>
  explicit V3(const V3<U>& source);


  // Cast operator: Conversion from one Vector type to another ;D
  template <typename U>
  operator V3<U>() const;

  // Conversion to a boolean
  operator bool() const;

  // Arithmetic operators
  template <typename U>
  V3& operator= (const V3<U>& source);
  // vector addition
  V3& operator+=(V3 const& other);
  V3 operator+(V3 const& other) const;
  V3& operator-=(V3 const& other);
  V3 operator-(V3 const& other) const;
  V3 operator-() const;   // unary minus or "additive inverse"
  // scaling
  V3& operator*=(T k);
  V3 operator*(T k) const;
  V3& operator/=(T k);
  V3 operator/(T k) const;
  // element-wise multiplication, division
  V3& operator*=(V3 const& other);
  V3 operator*(V3 const& other) const;
  V3& operator/=(V3 const& other);
  V3 operator/(V3 const& other) const;
  // incrementation and decrementation
  V3& operator++(); // prefix
  V3& operator--();
  V3 operator++(int); // postfix
  V3 operator--(int);

  // Array-style access
  T& operator[](size_t i);
  const T& operator[](size_t i) const;

  // Accessors
  V3 abs() const;
  V3<int> sign() const;
  float getNorm() const;
  float getNorm2() const;
  void setMagnitude(float new_magnitude);

  // outstream operator
  void print(std::ostream& out) const;

  // instream operator
  void read(std::istream& in);
};

// outstream operator
template <typename T>
std::ostream& operator<<(std::ostream& out, V3<T> const& v);

// instream operator
template <typename T>
std::istream& operator>>(std::istream& in, V3<T>& v);

typedef V3<int> iV3;
typedef V3<unsigned int> uV3;
typedef V3<float> fV3;
typedef V3<double> dV3;

// NB - "Inline" files are implementations that are included rather than
// compiled. They are especially useful for templates.
#include "V3.inl"

#endif // V3_HPP_INCLUDED
