#include <iostream>

#include "math_test.hpp"
#include "../math/M44.hpp"

using namespace std;

int math_test()
{
  cout << "ARITHMETIC" << endl;
  fV4 a(1, -2, 3, 1), b(4, 5, -6, 0);
  cout << "a = " << a << ", b = " << b << endl;
  a += b;
  cout << "a += b => a = " << a << endl;
  a -= b;
  cout << "a -= b => a = " << a << endl;

  cout << "a + b = " << a + b << ", b + a = " << b + a <<  endl;
  cout << "a - b = " << a - b << ", b - a = " << b - a << endl;
  cout << "a * b = " << a * b << ", b * a = " << b * a << endl;
  cout << "a / b = " << a / b << ", b / a = " << b / a << endl;
  cout << "++a => a = " << ++a << endl;
  cout << "--a => a = " << --a << endl;

  cout << endl << "UTILITIES" << endl;
  for(int i = 0; i < 4; i++) cout << "a[" << i << "] = " << a[i] << ", ";
  cout << endl;
  fV4 op(0,0,0,1), ov(0,0,0,0);
  cout << "op = " << op << ", ov = " << ov << endl;
  cout << "(bool)op = " << (bool)op << ", (bool)ov = " << (bool)ov << endl;
  cout << "(bool)a = " << (bool)a << ", (bool)b = " << (bool)b << endl;

  cout << endl << "DOT PRODUCT" << endl;
  cout << "a.b = " << dot(a, b) << endl;
  cout << "a.op = " << dot(a, op) << endl;
  cout << "a.ov = " << dot(a, ov) << endl;

  cout << endl << "CROSS PRODUCT" << endl;
  fV4 c = cross(a, b);
  cout << "a^b = " << c << endl;
  cout << "a.c = " << dot(a,c) << ", b.c = " << dot(b,c) << endl;

  cout << endl << "MATRICES" << endl;
  fV4 d(-5, -1, 2, 7);
  fM44 M(a, b, c, d);
  cout << "d = " << d << endl << "M = (a, b, c, d) = " << M << endl;
  cout << "M + M = " << M + M << endl;
  cout << "M - M = " << M - M << endl;
  cout << "M * 2 = " << M * 2.0f << endl;
  cout << "M / 2 = " << M / 2.0f << endl;

  cout << endl << "MATRIX TO ARRAY" << endl;
  cout << "M = (a, b, c, d) = " << M << endl;
  float array[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  M.exportArray(array);
  cout << "array = " << endl;
  for(size_t i = 0; i < 16; i++)
    cout << array[i] << ' ';
  cout << endl;

  cout << endl << "MATRIX-VECTOR MULTIPLICATION" << endl;
  cout << "M = " << M << endl;
  cout << "a = " << a << endl;
  cout << "M * a = " << M * a << endl;

  cout << endl << "MATRIX-MATRIX MULTIPLICATION" << endl;
  fM44 M2((fV4(9, 10, 11, 12)),
          (fV4(8, 7, 6, 5)),
          (fV4(5, 6, 7, 8)),
          (fV4(1, 2, 3, 4)));
  cout << "M2 = "<< M2 << endl;
  cout << "M2 * M = " << M2*M << endl;
  cout << "M * M2 = " << M*M2 << endl;

  cout << endl << "TRANSPOSE" << endl;
  cout << "M = " << M << endl;
  cout << "M.getTranspose() = " << M.getTranspose() << endl;

  cout << endl << "TRANSLATION MATRIX" << endl;
  cout << "translation matrix of " << b << " = " << transMatrix(b) << endl;

  cout << endl << "ROTATION AROUND AN AXIS" << endl;
  double angle = PI / 2;
  cout << "cos(angle) = " << cos(angle) << " sin(angle) = " << sin(angle) << endl;
  cout << "90 degrees around X = " << fM44::xRotMatrix(angle) << endl;
  cout << "90 degrees around Y = " << fM44::yRotMatrix(angle) << endl;
  cout << "90 degrees around Z = " << fM44::zRotMatrix(angle) << endl;

  cout << endl << "GENERALISED ROTATION" << endl;
  cout << "90 degrees around X = "
        << fM44::rotMatrix(angle, OX4(float)) << endl;
  cout << "90 degrees around Y = "
        << fM44::rotMatrix(angle, OY4(float)) << endl;
  cout << "90 degrees around Z = "
        << fM44::rotMatrix(angle, OZ4(float)) << endl;

  cout << endl << "IDENTITY" << endl;
  M.toIdentity();
  cout << "M.toIdentity() => M = " << M << endl;

  cout << endl << "INVERSION" << endl;
  cout << "setting M rotation to 90 degrees clockwise round Z" << endl;
  M = fM44::zRotMatrix(angle);
  cout << "M = " << M << endl;
  M.col[3] = fV4(4, 2, 3, 1);
  cout << "M.col[3] = (4, 2, 3, 1) => M = " << M << endl;
  fM44 Minv = M.getInverse();
  cout << "M.getInverse() = Minv = " << Minv << endl;
  cout << "Minv*M = " << Minv*M << endl << " M*Minv = " << M*Minv << endl;

  cout << endl << "ORTHOGONALISATION" << endl;
  M.col[0][0] = M.col[1][1] = 0.1;
  cout << M << endl;
  M.orthogonalise();
  cout << M << endl;

  return EXIT_SUCCESS;
}
