/*
Black Dog: a game for Android and Desktop platforms using SDL and OpenGLES.
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
#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

class ResourceManager
{
  /// ATTRIBUTES
private:
  bool loaded;
  /// METHODS
protected:
  // creation & destruction
  ResourceManager();
  virtual int load() = 0;
  virtual int unload() = 0;
  virtual ~ResourceManager();
public:
  // ensure coherency of state (ie. don't load if loaded or unload if unloaded)
  int startup();
  int shutdown();
  // loading
  int load_xml(const char* xml_file);
protected:
  virtual int parse_root(void* root_handle) = 0;
  int parse_list(void* root_handle, const char* list_name);
  virtual int parse_element(void* element) = 0;
};

#endif // RESOURCEMANAGER_HPP_INCLUDED
