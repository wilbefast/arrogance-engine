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

#include "Game.hpp"

#include "../io/file.hpp"           // ASSET_PATH
#include "../assert.hpp"            // ASSERT
#include "MainMenu.hpp"             // previous scene

#include "ModelViewState.hpp"
#include "../tp1/TP1State.hpp" //! FIXME
#include "../tp3/TP3State.hpp" //! FIXME

/// CREATION & DESTRUCTION

Game::Game(bool tutorial) :
Scene(new TP3State())
{
}

int Game::startup()
{
  ASSERT(Scene::startup() == EXIT_SUCCESS, "Game generic startup");
  // all clear !
  return EXIT_SUCCESS;
}

Game::~Game()
{
}

Scene* Game::previous()
{
  return new MainMenu();
}
