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

#include <stdarg.h>

#include "log.hpp"

#ifdef __ANDROID__

void log(int level, const char* format, ...)
{
  // get arguments
  va_list arguments;
  va_start(arguments, format);

  // send to android logger
  __android_log_vprint(level, APP_NAME, format, arguments);

    // don't forget to clean up!
  va_end(arguments);
}

#else // LINUX, MAC, WINDOWS

void log(int level, const char* format, ...)
{
  // get arguments
  va_list arguments;
  va_start(arguments, format);

  // get the string corresponding to logger level
  switch(level)
  {
    case LOG_INFO:
      printf(STR_LOG_INFO " - ");
    break;

    case LOG_WARN:
      printf(STR_LOG_WARN " - ");
    break;

    case LOG_ERROR:
      printf(STR_LOG_ERROR " - ");
    break;

    default:
    break;
  }

  // send to standard output and flush
  vprintf(format, arguments);
  puts("");

  // don't forget to clean up!
  va_end(arguments);
}

#endif // #ifdef __ANDROID__
