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

#ifndef PLATFORM_HPP_INCLUDED
#define PLATFORM_HPP_INCLUDED

#ifdef __ANDROID__
	#include <GLES/gl.h>    	//OpenGL ES rather than OpenGL

	#define GL_V_MAJOR 2    	//GLES 2.0
	#define GL_V_MINOR 0
	#define glOrtho(a,b,c,d,e,f) glOrthof(a,b,c,d,e,f) // Renaming hack

#else // LINUX, MAC, WINDOWS

  #ifdef WIN32
    #include <windows.h>      // must be included before GL
    //#define GL_V_MAJOR 1    // OpenGL 1.1
    //#define GL_V_MINOR 1
    #define GL_V_MAJOR 2    // OpenGL 2.0 required for GLEW
    #define GL_V_MINOR 0
  #else
    //#define GL_V_MAJOR 1    // OpenGL 1.5
    //#define GL_V_MINOR 5
    #define GL_V_MAJOR 2    // OpenGL 2.0 required for GLEW
    #define GL_V_MINOR 0
  #endif // WIN32

  #include <GL/glew.h>  // GLEW must be included before OpenGL
	#include <GL/gl.h>    // PC uses OpenGL rather than OpenGL ES


#endif // __ANDROID__

#endif // PLATFORM_HPP_INCLUDED
