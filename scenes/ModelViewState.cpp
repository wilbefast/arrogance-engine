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

#include "ModelViewState.hpp"

#include "../global.hpp"                          // for viewport
#include "../debug/assert.hpp"

#include "../io/MeshManager.hpp"



#include "../graphics/Joint.hpp"
Joint joint1(NULL, vertex_t(0.0, 0.0, 0.0));
Joint joint2(&joint1, vertex_t(0.5, 0.5, 0.0));
Joint joint3(&joint1, vertex_t(0.3, 0.8, 0.0));
Joint joint4(&joint3, vertex_t(0.9, 0.5, 0.0));


/// CREATION, DESTRUCTION

ModelViewState::ModelViewState() :
GameState(),
camera_angle(),
camera_offset()
{
}

int ModelViewState::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "ModelViewState starting GameState");

  // Occlusion on
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
  // Lighting on
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // all clear
  return EXIT_SUCCESS;
}

int ModelViewState::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "ModelViewState stopping GameState");

  // Occlusion off
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  // Lighting off
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int ModelViewState::update(float delta)
{
  // Update camera angle and position
  uV2 const& p = input.last_hover;
  camera_angle.x = CAMERA_MAX_ANGLE*p.x/global::viewport.x*2 - CAMERA_MAX_ANGLE;
	camera_angle.y = CAMERA_MAX_ANGLE*p.y/global::viewport.y*2 - CAMERA_MAX_ANGLE;

  // Update dynamic game objects
  int result = GameState::update(delta);
  if(result != CONTINUE)
    return result;

  // All clear
  return CONTINUE;
}

void ModelViewState::draw()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glTranslatef(global::viewport.x/2, global::viewport.y/2, 0.0f);
  glScalef(global::viewport.x/2, global::viewport.y/2, 1.0f);

  glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
  glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);
	glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
	glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
	glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);

  MeshManager::getInstance()->mesh.draw();

  // Draw dynamic game objects
  GameState::draw();
}
