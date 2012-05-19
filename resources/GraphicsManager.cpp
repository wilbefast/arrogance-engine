#include "GraphicsManager.hpp"

#include "tinyxml/tinyxml_dump.h"
#include "file.hpp"
#include "../assert.hpp"
#include "../warn.hpp"


/// SINGLETON

GraphicsManager* GraphicsManager::instance = NULL;

GraphicsManager* GraphicsManager::getInstance()
{
  if(!instance)
    instance = new GraphicsManager();
  return instance;
}

/// CREATION & DESTRUCTION

GraphicsManager::GraphicsManager() :
started(false),
textures(),
animations()
{
}

int GraphicsManager::startup()
{
  // don't start twice!
  if(started)
    WARN_RTN("GraphicsManager::startup","already started!", EXIT_SUCCESS);

  // load graphics
  ASSERT(load_xml(GET_ASSET("graphics.xml")) == EXIT_SUCCESS,
        "Loading graphical assets based on 'graphics.xml'");

  // All good!
  started = true;
  return EXIT_SUCCESS;
}

int GraphicsManager::load_xml(const char* xml_file)
{
  // pass string to the TinyXML document
  TiXmlDocument doc;
  ASSERT_AUX(io::read_xml(xml_file, &doc) == EXIT_SUCCESS,
            "Opening graphics pack XML file", doc.ErrorDesc());

  // create local variables for searching document tree
  TiXmlHandle doc_handle(&doc);
  TiXmlElement* element = NULL;

  // the root is a 'graphics' tag
  element = doc_handle.FirstChildElement("graphics").Element();
  TiXmlHandle root_handle = TiXmlHandle(element);

  // load textures
  element = root_handle.FirstChild("texture_list").FirstChild().Element();
  while(element)
  {
    // get the name of the texture and deduce its filename
    const char* name = element->Attribute("name");
    if(!name)
      WARN_RTN("GraphicsManager::load_xml", "malformed texture tag", EXIT_FAILURE);
    string filename(ASSET_PATH);
      filename.append(name);
      filename.append(".png");

    // load the texture
    load_texture(filename.c_str(), name);

    // continue to the next sprite
    element = element->NextSiblingElement();
  }

  // all good
  return EXIT_SUCCESS;
}

int GraphicsManager::shutdown()
{
  if(!started)
    WARN_RTN("GraphicsManager::shutdown","already shutdown!", EXIT_SUCCESS);

  // Clean up the animations
  for(AnimationI i = animations.begin(); i != animations.end(); i++)
    delete (*i).second;

  // Clean up the textures
  for(TextureI i = textures.begin(); i != textures.end(); i++)
  {
    if((*i).second->unload() != EXIT_SUCCESS)
      WARN_RTN("GraphicsManager::shutdown", "Failed to unload texture", EXIT_FAILURE);
    delete (*i).second;
  }

  // All good!
  started = false;
  return EXIT_SUCCESS;
}


GraphicsManager::~GraphicsManager()
{
  if(started)
    shutdown();
}


/// TEXTURES

int GraphicsManager::load_texture(const char* source_file, const char* name)
{
  // load image file
  Texture* new_texture = new Texture();
  ASSERT(new_texture->load(source_file) == EXIT_SUCCESS, source_file);

  // save under requested name
  str_id hash = numerise(name);
  textures[hash] = new_texture;

  // All clear !
  return EXIT_SUCCESS;
}

Texture* GraphicsManager::get_texture(const char* name)
{
  // search for the resource
  str_id hash = numerise(name);
  TextureI i = textures.find(hash);
  // make sure that it is found
  if(i == textures.end())
    WARN_RTN("GraphicsManager::get_texture invalid identifier", name, NULL);

  // return the texture we recovered
  return (*i).second;
}


/// ANIMATIONS

int GraphicsManager::create_animation(const char* texture_name,
                          iRect frame, int n_frames, const char* animation_name)
{
  // attempt to retrieve the desired texture
  Texture* animation_texture = get_texture(texture_name);
  ASSERT(animation_texture, "Looking for animation source texture");

  // create animation
  Animation* new_animation = new Animation();
  new_animation->init(animation_texture, frame, n_frames);

  // save under requested name
  str_id hash = numerise(animation_name);
  animations[hash] = new_animation;

  // All clear !
  return EXIT_SUCCESS;
}

Animation* GraphicsManager::get_animation(const char* name)
{
  // search for the resource
  str_id hash = numerise(name);
  AnimationI i = animations.find(hash);
  // make sure that it is found
  if(i == animations.end())
    WARN_RTN("GraphicsManager::get_animation invalid identifier", name, NULL);

  // return the animation we recovered
  return (*i).second;
}
