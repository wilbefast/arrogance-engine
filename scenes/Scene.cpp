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

#include "Scene.hpp"

#include "../global.hpp"
#include "../debug/assert.h"
#include "../debug/warn.h"

/// CONSTRUCTORS

Scene::Scene(SceneState* init_state) :
buttons()
{
  state = init_state ? init_state : new SceneState();
}

Scene::~Scene()
{
}

/// VIRTUAL

 void Scene::draw()
 {
  // Draw the buttons
  for(ButtonIter i = buttons.begin(); i != buttons.end(); i++)
    (*i)->draw();

  // Draw anything dynamic
  state->draw();
 }

int Scene::update(Scene** next, float delta)
{
  // Update buttons based on touch position
  for(ButtonIter i = buttons.begin(); i != buttons.end(); i++)
    if((*i)->press(state->input.last_hover, state->input.clicking))
    {
      // Save the first button we're touching and break
      state->input.last_button = (*i);
      break;
    }

  // update anything dynamic, go back to previous scene if nessecary
  if(state->update(delta) == SceneState::EXIT)
  {
    // Either continue (value of next is ignored) or go back to previous
    (*next) = previous();
    return Scene::CHANGE;
  }

  //continue
  return Scene::NO_CHANGE;
 }

int Scene::startup()
{
  // Start the state if applicable
  ASSERT(state->startup() == EXIT_SUCCESS, "Scene initialising state");

  // All clear
  return EXIT_SUCCESS;
}

int Scene::shutdown()
{
  // Delete buttons
  for(ButtonIter i = buttons.begin(); i != buttons.end(); i++)
    delete (*i);

  // Delete anything dynamic
  ASSERT(state->shutdown() == EXIT_SUCCESS, "Scene shutting down SceneState");
  delete state;

  // All clear
  return EXIT_SUCCESS;
}

Scene* Scene::previous()
{
  return NULL;
}

/// ACCESSORS

void Scene::addButton(Button* new_button)
{
  buttons.push_back(new_button);
}

SceneState* Scene::getState()
{
    return state;
}

/// FUNCTIONS

int Scene::splitRect(fRect src, fRect result[], uV2 n_divisions,
                         double scale)
{
  // Compute the split rectangle size
  fRect split = src / fV2(n_divisions);

  // Create each of the rectangles
  for(uV2 i(0,0); i.y < n_divisions.y; i.y++)
    for(i.x = 0; i.x < n_divisions.x; i.x++)
    {
      // Create and scale
      fRect new_rect = fRect(split + fV2(i)*split.getSize());
      new_rect.stretch_centered(scale);
      // Add to the resulting unroled matrix
      result[i.x + (i.y)*n_divisions.x] = new_rect;
    }
  return EXIT_SUCCESS;
}

fRect Scene::squashRect(fRect src, fV2 amount)
{
  // Squash horizontally
  if(amount.x > 0)
  {
    src.x += amount.x;
    src.w -= amount.x;
  }
  else
    src.w += amount.x;

  // Squash vertically
  if(amount.y > 0)
  {
    src.y += amount.y;
    src.h -= amount.y;
  }
  else
    src.h += amount.y;

  // Return result
  return src;
}
