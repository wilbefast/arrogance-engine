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

#include "MainMenu.hpp"

#include "../graphics/draw.hpp" // for draw::rectangle
#include "../global.hpp"
#include "../debug/warn.h"
#include "../debug/assert.h"
#include "../io/GraphicsManager.hpp"

#include "Game.hpp"

/// CONSTRUCTORS, DESTRUCTORS

MainMenu::MainMenu() :
Scene(),
title_src(0.0f, 0.0f, 0.0f, 0.0f),
title_dest(0.0f, 0.0f, 0.0f, 0.0f)
{
}

MainMenu::~MainMenu()
{
}

int MainMenu::startup()
{
  // pass up hierarchy
  ASSERT(Scene::startup() == EXIT_SUCCESS, "MainMenu generic startup");

  // set 2D mode
  draw::use2D();

  // get the texture from the resource manager
  texture = GraphicsManager::getInstance()->get_texture("menus");

  /// 1. TITLE IMAGE (TOP THIRD)
  // set the area of the screen to use (the top third)
  fRect top_third =
    fRect(0.0f, 0.0f, WINDOW_DEFAULT_W, WINDOW_DEFAULT_H / 3.0f);
  // set the area of texture to draw
  title_src = fRect(0, 0, 512, 128);
  title_dest = fRect(fV2(0, 0), global::viewport);

  // maintain aspect ratio
  title_dest.setRatio(title_src.getRatio());
  // centre the title image within the top area of the screen
  title_dest.centreWithin(top_third);

  /// 2. BUTTONS (BUTTON SIX NINTHS)
  // set the area of the screen to use (bottom two top thirds)
  fRect bottom_thirds =
    squashRect(fV2(WINDOW_DEFAULT_W, WINDOW_DEFAULT_H), fV2(0, top_third.h));
  fRect top_ninths = bottom_thirds / fV2(1,3);
  fRect mid_ninths = top_ninths + fV2(0, top_ninths.h);
  fRect low_ninths = mid_ninths + fV2(0, mid_ninths.h);
  // the top and bottom buttons are smaller than the middle one
  top_ninths.stretch_centered(0.7);
  mid_ninths.stretch_centered(1.2);
  low_ninths.stretch_centered(0.7);
  // set the area of texture to draw (on top of eachother)
  fRect play_src(0, title_src.h, 256, 128);
  fRect exit_src = play_src + fV2(0, play_src.h);
  fRect options_src = exit_src + fV2(0, exit_src.h);
  // add the buttons to the scene
  buttons.push_back(new Button("play", *texture, top_ninths, play_src));
  buttons.push_back(new Button("options", *texture, mid_ninths, options_src));
  buttons.push_back(new Button("exit", *texture, low_ninths, exit_src));

  /// 3. ALL CLEAR!
  return EXIT_SUCCESS;
}

int MainMenu::shutdown()
{
  // all clear
  return Scene::shutdown();
}


/// LOOP

int MainMenu::update(Scene** next, float delta)
{
  // Pass up hierarchy
  Scene::update(next, delta);

  // Move to the next screen on click
  str_id button_tag;
  if((button_tag = state->releasedOnButton()))
  {
    // select world
    if(button_tag == numerise("play"))
      (*next) = new Game(false); // go straight to the game
    // change options
    else if(button_tag == numerise("tutorial"))
      (*next) = new Game(true); // play tutorial before starting game proper
    // exit the game
    else if(button_tag == numerise("exit"))
      (*next) = NULL; // exit

    // change scene
    return Scene::CHANGE;
  }
  else
    // keep same scene
    return Scene::NO_CHANGE;
}

void MainMenu::draw()
{
  // Draw the background colour
  draw::rectangle(fRect(fV2(0, 0), global::viewport), Colour(242, 68, 5));

  // draw title
  texture->draw(&title_src, &title_dest);

  // draw buttons
  Scene::draw();
}

Scene* MainMenu::previous()
{
    return NULL;
}
