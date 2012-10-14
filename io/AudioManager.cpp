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

#include "AudioManager.hpp"

#include "../debug/assert.h"
#include "../debug/warn.h"

#include "../math/wjd_math.h"

#include "file.hpp"                       // for io::MAX_BLOCKS
#include "tinyxml/tinyxml.h"

using namespace std;

/// SINGLETON

AudioManager* AudioManager::instance = NULL;

AudioManager* AudioManager::getInstance()
{
  if(!instance)
    instance = new AudioManager();
  return instance;
}

/// CREATION & DESTRUCTION

AudioManager::AudioManager() :
ResourceManager(),
volume(MIX_MAX_VOLUME/2),
music(NULL),
music_file(NULL),
#ifdef __ANDROID__
  sdcard_file(NULL),
#endif // #ifdef __ANDROID__
sounds()
{
}

AudioManager::~AudioManager()
{
}

/// LOADING

int AudioManager::load()
{
  //Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
  ASSERT_MIX(Mix_OpenAudio(DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
           MIX_DEFAULT_CHANNELS, DEFAULT_CHUNK_SIZE) != -1,
          "Starting SDL Mixer");

  // load audio
  ASSERT(load_xml(GET_ASSET("audio.xml")) == EXIT_SUCCESS,
        "Loading audio assets based on 'audio.xml'");

  // All good!
  return EXIT_SUCCESS;
}

int AudioManager::unload()
{
  // Clean up the sound
  for(SoundI i = sounds.begin(); i != sounds.end(); i++)
    Mix_FreeChunk((*i).second);

  // Clean up the music
  unload_music();

  // Shut down audio -- this may take a few seconds
  log(LOG_INFO, "SDL Mixer shutdown pending...");
  Mix_CloseAudio();
  log(LOG_INFO, "SDL Mixer shutdown : Okay");

  // All good!
  return EXIT_SUCCESS;
}

int AudioManager::parse_root(void* root_handle)
{
  // cast handle to TinyXML handle
  TiXmlHandle* txml_root = (TiXmlHandle*)root_handle;

  // load music
  ASSERT(parse_element(txml_root->FirstChild("music").Element())
         == EXIT_SUCCESS,
            "AudioManager parsing music element");

  // load sound effects
  ASSERT(parse_list(txml_root, "sound_list") == EXIT_SUCCESS,
              "AudioManager parsing list of sound effects");

  // all clear!
  return EXIT_SUCCESS;
}

int AudioManager::parse_element(void* element)
{
  // cast element to TinyXML element
  TiXmlElement* txml_element = (TiXmlElement*)element;
  if(!txml_element)
  {
    WARN("AudioManager::parse_element", "Non-existent element");
    return EXIT_SUCCESS;
  }

  // music element
  else if(!strcmp(txml_element->Value(), "music"))
  {
    // conver the name attribute to a file name
    const char* name = txml_element->Attribute("name");
    if(!name)
      WARN_RTN("AudioManager::load_xml", "malformed music tag (name)", EXIT_FAILURE);
    string filename = io::name_to_path(name, MUSIC_FILETYPE);
    // load and play music from the specified file
    ASSERT(load_music(filename.c_str()) == EXIT_SUCCESS, "Loading initial music track");
    ASSERT(play_music(true) == EXIT_SUCCESS, "Setting initial music track to loop");
  }

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


/// GLOBAL VOLUME

void AudioManager::volume_up()
{
  set_volume((volume < MIX_MAX_VOLUME - VOLUME_STEP) ? volume + VOLUME_STEP
                                                      : MIX_MAX_VOLUME);
}

void AudioManager::volume_down()
{
  set_volume((volume > VOLUME_STEP) ? volume - VOLUME_STEP : 0);
}


/// MUSIC

int AudioManager::load_music(const char* source_file)
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

int AudioManager::play_music(bool loop)
{
  // Check that music is indeed loaded
  ASSERT(music && music_file, "Checking that music is loaded");

  // Try to play music
  ASSERT_MIX(Mix_PlayMusic(music, (loop)?0:-1) != -1, "Setting music to loop");

  // Success !
  return EXIT_SUCCESS;
}

void AudioManager::stop_music()
{
  Mix_HaltMusic();
}

void AudioManager::unload_music()
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


/// SOUND

int AudioManager::load_sound(const char* source_file, const char* name)
{
  // load wave file
  Mix_Chunk* new_sound = Mix_LoadWAV(source_file);
  // check that the sound was loaded successfully
  ASSERT_MIX(new_sound, source_file);
  // save under requested name
  str_id hash = numerise(name);
  sounds[hash] = new_sound;

  // All clear !
  return EXIT_SUCCESS;
}

int AudioManager::play_sound(const char* name)
{
  // hash the string to find the resource
  if(play_sound(numerise(name)) == EXIT_FAILURE)
    WARN_RTN("AudioManager::play_sound invalid identifier", name, EXIT_FAILURE);

  return EXIT_SUCCESS;
}

int AudioManager::play_sound(str_id id)
{
  // search for the resource
  SoundI i = sounds.find(id);
  // make sure that it is found
  if(i == sounds.end())
    return EXIT_FAILURE;
  // attempt to play the sound if it is
  int channel = Mix_PlayChannel(-1, (*i).second, 0);
  if(channel == -1)
  {
    WARN_RTN("AudioManager::play_sound", Mix_GetError(), EXIT_FAILURE);
  }
  // if no error has occured, set the channel volume to the global volume
  else
    Mix_Volume(channel, volume);

  // All clear !
  return EXIT_SUCCESS;
}

/// SUBROUTINES

void AudioManager::set_volume(unsigned short new_volume)
{
  if(new_volume != volume)
  {
    // change volume
    volume = new_volume;
    // reset music volume
    if(music)
      Mix_VolumeMusic(volume);
    // play notification sound to indicate current volume
    play_sound("ui_interact");
  }
}
