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

#include "SceneState.hpp"

#include <stdlib.h>

/// CONSTRUCTORS DESTRUCTORS
SceneState::SceneState() :
input()
{
}

SceneState::~SceneState()
{
}

/// STARTUP AND SHUTDOWN

int SceneState::startup()
{
    return EXIT_SUCCESS;
}

int SceneState::shutdown()
{
    return EXIT_SUCCESS;
}

/// UPDATE AND DRAW

int SceneState::update()
{
    // Nothing to report
    return EXIT_SUCCESS;
}

void SceneState::draw()
{
}

/// ACCESSORS

SceneState::Input* SceneState::getInput()
{
    return &input;
}

void SceneState::setCursor(uV2 _position, bool _clicking)
{
  // save the previous state
  input.clicking_previous = input.clicking;
  // set to the new state

  input.cursor_position = _position;
  if((input.clicking = _clicking))
      input.last_touch = _position;
}

str_id SceneState::releasedOnButton()
{
    if(!input.clicking && input.clicking_previous && input.last_button
    && input.last_button->contains(input.last_touch))
    {
        input.clicking_previous = false; // clear input
        return input.last_button->getTag();
    }
    else
        return 0;
}

bool SceneState::newClick() const
{
    return (!input.clicking && input.clicking_previous);
}
