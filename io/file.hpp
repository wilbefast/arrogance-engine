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

#ifndef FILE_HPP_INCLUDED
#define FILE_HPP_INCLUDED

#include <string>

#ifdef __ANDROID__
  #define ASSET_PATH "" // use a bit of SDL RWops "magic" ;)
  #define BUFFER_XML 1
#else // LINUX, MAC, WINDOWS
  #define ASSET_PATH "./assets/"
  #define BUFFER_XML 1
#endif // #ifdef __ANDROID__

#define GET_ASSET(x) ASSET_PATH x

namespace io
{
  const int MAX_BLOCKS =  1024;

  std::string path_to_name(const char* filepath);
  std::string name_to_path(const char* name, const char* extension = NULL);
  int read_text(const char* file_path, char** destination);
  int read_xml(const char* file_path, void* document);
}


#endif // FILE_HPP_INCLUDED
