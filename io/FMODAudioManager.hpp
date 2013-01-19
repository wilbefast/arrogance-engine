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

#ifndef FMODAUDIOMANAGER_HPP_INCLUDED
#define FMODAUDIOMANAGER_HPP_INCLUDED

#include <fmodex/fmod.h>

#include <map>
#include "../math/numerise.h"         // for str_id

#include "ResourceManager.hpp"

#define SOUND_FILETYPE "wav"
#define MUSIC_FILETYPE "ogg"
#define VOLUME_STEP MIX_MAX_VOLUME/14

typedef std::map<str_id, FMOD_SOUND*> SoundMap;
typedef SoundMap::iterator SoundI;

class FMODAudioManager : public ResourceManager
{
  /// CONSTANTS
private:
  static const int MAX_CHANNELS = 16;

  /// SINGLETON
private:
  static FMODAudioManager* instance;
public:
  static FMODAudioManager* getInstance();

  /// ATTRIBUTES
private:
  unsigned short volume; // value between 0 and MIX_MAX_VOLUME = 128
  // music
  FMOD_SYSTEM *system;
  FMOD_RESULT result;

  #ifdef __ANDROID__
    const char* sdcard_file;
  #endif // #ifdef __ANDROID__
  // sound
  SoundMap sounds;

  /// METHODS
private:
  // creation & destruction
  FMODAudioManager();
public:
  ~FMODAudioManager();
  // loading -- overrides ResourceManager
  int load();
  int unload();
  int parse_root(void* root_handle);
  int parse_element(void* element);
  // music
  /*int load_music(const char* source_file);
  int play_music(bool loop);
  void stop_music();
  void unload_music();*/
  // sound
  int load_sound(const char* source_file, const char* name);
  int play_sound(const char* name);
  int play_sound(str_id id);
};

#endif // FMODAUDIOMANAGER_HPP_INCLUDED
