#include "TP3State.hpp"

#include "../assert.hpp"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

/// CREATION, DESTRUCTION

TP3State::TP3State() :
GameState(),
camera_angle(),
camera_offset()
{
}

int TP3State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "TP3State starting GameState");

  // Occlusion on
  //glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glDisable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
  // Lighting on
  //glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // all clear
  return EXIT_SUCCESS;
}

int TP3State::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "TP3State stopping GameState");

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

int TP3State::update(float delta)
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

void TP3State::draw()
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
