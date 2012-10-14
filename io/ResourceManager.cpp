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
#include "ResourceManager.hpp"

#include "file.hpp"
#include "../debug/assert.h"
#include "../debug/warn.h"

#include "tinyxml/tinyxml.h"

using namespace std;

/// CREATION, DESTRUCTION

ResourceManager::ResourceManager() :
loaded(false)
{
}

int ResourceManager::startup()
{
  // check that we're not already initialised
  if(loaded)
    WARN_RTN("ResourceManager::startup","already started!", EXIT_SUCCESS);

  // if this is that case, load everything
  ASSERT(load() == EXIT_SUCCESS, "ResourceManager::load()");

  // all clear !
  loaded = true;
  return EXIT_SUCCESS;
}

int ResourceManager::shutdown()
{
  // check that we're initialised
  if(!loaded)
  {
    WARN_RTN("ResourceManager::shutdown","already shut down!", EXIT_SUCCESS);
  }
  // if so, unload everything
  ASSERT(unload() == EXIT_SUCCESS, "ResourceManager::unload()");

  // all clear !
  loaded = true;
  return EXIT_SUCCESS;
}

ResourceManager::~ResourceManager()
{
  // Force clean if nessecary
  if(loaded)
  {
    WARN("ResourceManager::~ResourceManager",
          "Destructor forcing shutdown sequence");
    shutdown();
  }
}

/// LOADING

int ResourceManager::load_xml(const char* xml_file)
{
  // read string into TiXmlDocument object
  TiXmlDocument doc;
  ASSERT_AUX(io::read_xml(xml_file, &doc) == EXIT_SUCCESS,
            "Opening graphics pack XML file", doc.ErrorDesc());

  // get the root element
  TiXmlHandle root_element = TiXmlHandle(&doc).FirstChildElement().Element();

  // parse the document as befits the sub-class
  ASSERT(parse_root(&root_element) == EXIT_SUCCESS, "Parsing XML document");

  // all clear!
  return EXIT_SUCCESS;
}

int ResourceManager::parse_list(void* root_handle, const char* list_name)
{
  // get the first element of the list
  TiXmlElement* element =
    ((TiXmlHandle*)root_handle)->FirstChild(list_name).FirstChild().Element();

  // treat each element in turn
  while(element)
  {
    // parse the element as the subclass requires
    if(parse_element(element) != EXIT_SUCCESS)
      WARN_RTN("ResourceManager::parse_list failed", list_name, EXIT_FAILURE);

    // continue to the next element
    element = element->NextSiblingElement();
  }

  return EXIT_SUCCESS;
}
