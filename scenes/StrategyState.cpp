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



#include "../graphics/Joint.hpp"
Joint joint1(NULL, vertex_t(0.0, 0.0, 0.0));
Joint joint2(&joint1, vertex_t(0.5, 0.5, 0.0));
Joint joint3(&joint1, vertex_t(0.3, 0.8, 0.0));
Joint joint4(&joint3, vertex_t(0.9, 0.5, 0.0));


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
  // Materials on
  glEnable(GL_COLOR_MATERIAL);

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
  // Materials off
  glDisable(GL_COLOR_MATERIAL);

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int StrategyState::update()
{
  // Update camera angle and position
  uV2 const& p = input.cursor_position;
  camera_angle.x = CAMERA_MAX_ANGLE*p.x/global::viewport.x*2 - CAMERA_MAX_ANGLE;
	camera_angle.y = CAMERA_MAX_ANGLE*p.y/global::viewport.y*2 - CAMERA_MAX_ANGLE;

  // Update dynamic game objects
  int result = GameState::update();
  if(result != CONTINUE)
    return result;

  // All clear
  return EXIT_SUCCESS;
}


  GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0};
  GLfloat whiteSpecularMaterial[] = {0.0, 0.0, 1.0};
  GLfloat greenEmissiveMaterial[] = {0.0, 1.0, 0.0};

  GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0};
  GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0};
  GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0};
  GLfloat shininess[] = {128};
  GLfloat blankMaterial[] = {0.0, 0.0, 0.0};


void StrategyState::draw()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glTranslatef(global::viewport.x/2, global::viewport.y/2, 0.0f);
  glScalef(global::viewport.x, global::viewport.y, 1.0f);

  glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
  glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);
	glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
	glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
	glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);


  glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
  glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);


  /** SPECULAR **/
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);
  //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blankMaterial);

  /** DIFFUSE **/
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blankMaterial);
  //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);

  /** EMISSIVE **/
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, greenEmissiveMaterial);
  //glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blankMaterial);


  MeshManager::getInstance()->mesh.draw();

  // Draw dynamic game objects
  GameState::draw();
}
