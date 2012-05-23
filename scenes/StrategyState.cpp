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

#include "StrategyState.hpp"

#include "../global.hpp"                          // for viewport
#include "../assert.hpp"

#include "../resources/MeshManager.hpp"

/// CREATION, DESTRUCTION

StrategyState::StrategyState() :
GameState(),
camera_angle(),
camera_offset()
{
}

int StrategyState::startup()
{
  // basic startup
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "StrategyState starting GameState");

  // Occlusion on
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
  // Lighting on
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // all clear
  return EXIT_SUCCESS;
}

int StrategyState::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "StrategyState stopping GameState");

  // Occlusion off
  glDisable(GL_DEPTH_TEST);
  // Lighting off
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int StrategyState::update()
{
  int result;

  // Update camera angle and position
  uV2 const& p = input.cursor_position;
  camera_angle.x = CAMERA_MAX_ANGLE*p.x/global::viewport.x*2 - CAMERA_MAX_ANGLE;
	camera_angle.y = CAMERA_MAX_ANGLE*p.y/global::viewport.y*2 - CAMERA_MAX_ANGLE;

  // Update dynamic game objects
  result = GameState::update();
  if(result != CONTINUE)
    return result;


  // All clear
  return EXIT_SUCCESS;
}

void StrategyState::draw()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glTranslatef(global::viewport.x/2, global::viewport.y/2, 0.0f);
  //glScalef(5.0f, 5.0f, 1.0f);
  glScalef(global::viewport.x, global::viewport.y, 1.0f);

  glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
  glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);
	glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
	glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
	glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);

  MeshManager::getInstance()->mesh.draw();

  // Draw dynamic game objects
  GameState::draw();
}
