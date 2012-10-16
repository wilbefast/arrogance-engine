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

#ifdef __ANDROID__
  #include <android/log.h>    // for __android_log_print
#else // LINUX, MAC, WINDOWS
	#include <stdio.h>          // for printf
#endif // #ifdef __ANDROID__

#include "log.h"

// private function
void log(unsigned int level, const char* format, va_list& arguments)
{
#ifdef __ANDROID__
  // send to android logger
  __android_log_vprint(level, APP_NAME, format, arguments);
#else // LINUX, MAC, WINDOWS
  // print a header depending on the level
  static const char* LOG_STR[3] = { "INFO - ", "WARN - ", "ERROR - " };
  printf("%s", LOG_STR[(level > 2) ? 2 : level]);

  // send to standard output and flush
  vprintf(format, arguments);
  puts("");
#endif // #ifdef __ANDROID__
}

void log(unsigned int level, const char* format, ...)
{
#ifdef DEBUG
  // get arguments
  va_list arguments;
  va_start(arguments, format);

  // send to auxillary function
  log(level, format, arguments);

  // don't forget to clean up!
  va_end(arguments);
#endif // #ifdef DEBUG
}
