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

#ifndef GLOBAL_HPP_INCLUDED
#define GLOBAL_HPP_INCLUDED

#include "math/Rect.hpp"

#define WINDOW_DEFAULT_FLAGS \
  SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN
#define WINDOW_DEFAULT_W 640    // size used by Desktop version
#define WINDOW_DEFAULT_H 480
#define MAX_FPS 30
#define APP_NAME "Arrogance"

namespace global
{
    extern iV2 viewport;
    extern fV2 scale;
};


#endif // GLOBAL_HPP_INCLUDED
