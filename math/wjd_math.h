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

#ifndef WJD_MATH_HPP_INCLUDED
#define WJD_MATH_HPP_INCLUDED

#include <cmath>
#include <stdlib.h>

#define PI 3.14159265
#define RAD2DEG(r) (r*180/PI)
#define DEG2RAD(d) (d*PI/180)
#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x<y?x:y)
#define SIGN(x) (x>0?1:(x<0?-1:0))
#define ABS(x) ((x)*SIGN(x))
#define SQR(x) ((x)*(x))
#define RAND() (((double)rand())/RAND_MAX)
#define RAND_BETWEEN(x,y) (RAND()*ABS(x-y)+MIN(x,y))
#define RAND_SIGN() ((RAND()<0.5)?-1:1)
#define ISPWR2(n) !(n & (n-1))

int nextpwr2(int n);

#endif // WJD_MATH_HPP_INCLUDED
