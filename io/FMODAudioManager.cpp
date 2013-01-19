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

#include "FMODAudioManager.hpp"

#include "../debug/assert.h"
#include "../debug/warn.h"

#include "../math/wjd_math.h"

#include "file.hpp"                       // for io::MAX_BLOCKS
#include "tinyxml/tinyxml.h"

// custom assertion
#include <fmodex/fmod_errors.h>
#include "../debug/assert.h"
#define ASSERT_FMOD(fmod_result, what)                     \
    ASSERT_AUX(fmod_result == FMOD_OK, what, FMOD_ErrorString(fmod_result))




using namespace std;

/// SINGLETON

FMODAudioManager* FMODAudioManager::instance = NULL;

FMODAudioManager* FMODAudioManager::getInstance()
{
  if(!instance)
    instance = new FMODAudioManager();
  return instance;
}

/// CREATION & DESTRUCTION

FMODAudioManager::FMODAudioManager() :
ResourceManager(),
#ifdef __ANDROID__
  sdcard_file(NULL),
#endif // #ifdef __ANDROID__
sounds()
{
}

FMODAudioManager::~FMODAudioManager()
{
}

/// LOADING

int FMODAudioManager::load()
{
  result = FMOD_System_Create(&system);
  ASSERT_FMOD(result, "Creating FMOD system");
  result = FMOD_System_Init(system, MAX_CHANNELS, FMOD_INIT_NORMAL, NULL);
  ASSERT_FMOD(result, "Initialising FMOD system");

  // load audio
  ASSERT(load_xml(GET_ASSET("audio.xml")) == EXIT_SUCCESS,
        "Loading audio assets based on 'audio.xml'");

  // All good!
  return EXIT_SUCCESS;
}

int FMODAudioManager::unload()
{
  // Clean up the sound
  for(SoundI i = sounds.begin(); i != sounds.end(); i++)
    FMOD_Sound_Release((*i).second);

  // Clean up the music
  //! unload_music();

  // Shut down audio -- this may take a few seconds
  log(LOG_INFO, "FMOD shutdown pending...");
  FMOD_System_Close(system);
  FMOD_System_Release(system);
  log(LOG_INFO, "FMOD shutdown : Okay");

  // All good!
  return EXIT_SUCCESS;
}

int FMODAudioManager::parse_root(void* root_handle)
{
  // cast handle to TinyXML handle
  TiXmlHandle* txml_root = (TiXmlHandle*)root_handle;

  // load music
  ASSERT(parse_element(txml_root->FirstChild("music").Element())
         == EXIT_SUCCESS,
            "FMODAudioManager parsing music element");

  // load sound effects
  ASSERT(parse_list(txml_root, "sound_list") == EXIT_SUCCESS,
              "FMODAudioManager parsing list of sound effects");

  // all clear!
  return EXIT_SUCCESS;
}

int FMODAudioManager::parse_element(void* element)
{
  // cast element to TinyXML element
  TiXmlElement* txml_element = (TiXmlElement*)element;
  if(!txml_element)
  {
    WARN("FMODAudioManager::parse_element", "Non-existent element");
    return EXIT_SUCCESS;
  }

  // music element
  /*!else if(!strcmp(txml_element->Value(), "music"))
  {
    // conver the name attribute to a file name
    const char* name = txml_element->Attribute("name");
    if(!name)
      WARN_RTN("FMODAudioManager::load_xml", "malformed music tag (name)", EXIT_FAILURE);
    string filename = io::name_to_path(name, MUSIC_FILETYPE);
    // load and play music from the specified file
    ASSERT(load_music(filename.c_str()) == EXIT_SUCCESS, "Loading initial music track");
    ASSERT(play_music(true) == EXIT_SUCCESS, "Setting initial music track to loop");
  }*/

  // sound element
  else if(!strcmp(txml_element->Value(), "sound"))
  {
    // get the name of the sound effect and deduce its filename
    const char* name = txml_element->Attribute("name");
    if(!name)
      WARN_RTN("AudioManager::load_xml", "malformed sound tag (name)", EXIT_FAILURE);

    // load the sound effect
    string filename = io::name_to_path(name, SOUND_FILETYPE);
    load_sound(filename.c_str(), name);
  }

  // all clear!
  return EXIT_SUCCESS;
}

/// MUSIC
/*!
int FMODAudioManager::load_music(const char* source_file)
{
  // Attempt to open the music file
  music_file = SDL_RWFromFile(source_file, "rb"); // read binary
  ASSERT(music_file, source_file); // print the name of the file being opened

  #ifdef __ANDROID__
    // get the name of the file and prefix it with "/sdcard/"
    /// FIXME -- why doesn't append work on Android?
    //sdcard_file = (string("/sdcard/").append(source_file)).c_str();
    sdcard_file = "/sdcard/music.ogg";

    // initialise RWops structure from file in SD card
    FILE* sdcard = fopen(sdcard_file, "wb");
    ASSERT(sdcard, "Opening file to export music from APK to filesystem");
    SDL_RWops* sdcard_rw = SDL_RWFromFP(sdcard, SDL_TRUE); // autoclose
    ASSERT(sdcard_rw, "Creating SDL_RWops structure from file pointer");

    // externalise music data from APK assets folder to the SD card
    char buffer[io::MAX_BLOCKS];
    int read_amount = SDL_RWread(music_file, buffer, 1, io::MAX_BLOCKS);
    while(read_amount > 0)
    {
      SDL_RWwrite(sdcard_rw, buffer, 1, read_amount);
      SDL_RWseek(music_file, SEEK_CUR, read_amount*io::BLOCK_SIZE);
      read_amount = SDL_RWread(music_file, buffer, 1, io::MAX_BLOCKS);
    }
    SDL_RWclose(music_file);
    SDL_RWclose(sdcard_rw);

    // load the music from the filesystem, not the archive
    sdcard = fopen(sdcard_file, "rb");
    ASSERT(sdcard, "Opening file to import music from filesystem");
    music_file = SDL_RWFromFP(sdcard, SDL_TRUE); // autoclose
    ASSERT(music_file, "Creating SDL_RWops structure from file pointer");

  #endif //ifdef __ANDROID__

  // Attempt to read the file contents as music
  ASSERT_MIX(music = Mix_LoadMUS_RW(music_file),
              "Extracting music from SDL_RWops structure");

  /// NB - file is NOT closed as music data must be streamed

  /// NB (bis) - the RWops structure 'music_file' will be freed automatically
  /// when 'music' is closed.

  // Success !
  return EXIT_SUCCESS;
}

int FMODAudioManager::play_music(bool loop)
{
  // Check that music is indeed loaded
  ASSERT(music && music_file, "Checking that music is loaded");

  // Try to play music
  ASSERT_MIX(Mix_PlayMusic(music, (loop)?0:-1) != -1, "Setting music to loop");

  // Success !
  return EXIT_SUCCESS;
}

void FMODAudioManager::stop_music()
{
  //Mix_HaltMusic();
}

void FMODAudioManager::unload_music()
{
  // Stop and then free music
  stop_music();

  // Close music stream
  if(music_file)
  {
    if(!music)
      // this is closed automatically by Mix_FreeMusic
      SDL_RWclose(music_file);
    music_file = NULL;
  }

  if(music)
  {
    // should call SDL_RWclose(music_file)
    Mix_FreeMusic(music);
    music = NULL;
  }

  #ifdef __ANDROID__
  if(sdcard_file)
  {
    remove(sdcard_file);
    //free((char*)sdcard_file);
    sdcard_file = NULL;
  }
  #endif //ifdef __ANDROID__
}
*/

/// SOUND

int FMODAudioManager::load_sound(const char* source_file, const char* name)
{
  // load wave file
  FMOD_SOUND* new_sound;
  result =
    FMOD_System_CreateSound(system, source_file, FMOD_CREATESAMPLE, 0, &new_sound);

  // check that the sound was loaded successfully
  ASSERT_FMOD(result, source_file);
  // save under requested name
  str_id hash = numerise(name);
  sounds[hash] = new_sound;

  // All clear !
  return EXIT_SUCCESS;
}

int FMODAudioManager::play_sound(const char* name)
{
  // hash the string to find the resource
  if(play_sound(numerise(name)) == EXIT_FAILURE)
    WARN_RTN("AudioManager::play_sound invalid identifier", name, EXIT_FAILURE);

  return EXIT_SUCCESS;
}

int FMODAudioManager::play_sound(str_id id)
{
  // search for the resource
  SoundI i = sounds.find(id);
  // make sure that it is found
  if(i == sounds.end())
    return EXIT_FAILURE;
  // attempt to play the sound if it is
  result =
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, (*i).second, 0, NULL);
  WARN_IF(result != FMOD_OK, FMOD_ErrorString(result), EXIT_FAILURE);


  // All clear !
  return EXIT_SUCCESS;
}

