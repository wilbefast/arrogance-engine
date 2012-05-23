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

#include "file.hpp"
#include "../assert.hpp"
#include "../warn.hpp"

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
started(false),
music(NULL),
music_file(NULL),
sounds()
{
}

int AudioManager::startup()
{
  if(started)
    WARN_RTN("AudioManager::startup","already started!", EXIT_SUCCESS);

  //Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
  ASSERT_MIX(Mix_OpenAudio(DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
           MIX_DEFAULT_CHANNELS, DEFAULT_CHUNK_SIZE) != -1,
          "Starting SDL Mixer");

  // load sounds
  ASSERT(load_xml(GET_ASSET("audio.xml")) == EXIT_SUCCESS,
    "Loading audio assets based on 'audio.xml'");

  // All good!
  started = true;
  return EXIT_SUCCESS;
}

int AudioManager::load_xml(const char* xml_file)
{
  /// TODO
  return EXIT_SUCCESS;
}

int AudioManager::shutdown()
{
  if(!started)
    WARN_RTN("AudioManager::shutdown","already shutdown!", EXIT_SUCCESS);

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
  started = false;
  return EXIT_SUCCESS;
}


AudioManager::~AudioManager()
{
  if(started)
    shutdown();
}

/// MUSIC

int AudioManager::load_music(const char* source_file)
{
  // Attempt to open the music file
  music_file = SDL_RWFromFile(source_file, "rb"); // read binary
  ASSERT(music_file, source_file);

  // Attempt to read the file contents as music
  ASSERT_MIX(music = Mix_LoadMUS_RW(music_file),
              "Extracting music from SDL_RWops structure");

  /// NB - file is NOT closed as music data must be streamed

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
  if(music)
  {
    Mix_FreeMusic(music);
    music = NULL;
  }

  // Close music stream
  if(music_file)
  {
    //SDL_RWclose(music_file);
    /// FIXME
    music_file = NULL;
  }
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
  // search for the resource
  str_id hash = numerise(name);
  SoundI i = sounds.find(hash);
  // make sure that it is found
  if(i == sounds.end())
    WARN_RTN("AudioManager::play_sound invalid identifier", name, EXIT_FAILURE);
  // attempt to play the sound if it is
  if(Mix_PlayChannel(-1, (*i).second, 0) == -1)
    WARN_RTN("AudioManager::play_sound", Mix_GetError(), EXIT_FAILURE);

  // All clear !
  return EXIT_SUCCESS;
}
