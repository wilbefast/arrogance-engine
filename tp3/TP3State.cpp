#include "TP3State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/Mesh3D.hpp"
#include "../graphics/draw.hpp"

#include "OBJLoader.hpp"
#include "../debug/log.h"

#define USE_MOULIS false

using namespace std;


/// CREATION, DESTRUCTION

TP3State::TP3State() :
GameState(),
camera_angle(0.0f),
camera_offset(),
engine(),
left(false),
right(false),
up(false),
down(false),
alt(false),
space(false),
ctrl(false)
{
}

int TP3State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "TP3State starting GameState");

  // load the 3D scene
  draw::use3D();
  if(USE_MOULIS)
    engine.setup();
  else
    MeshManager::getInstance()->startup();

  // all clear
  return EXIT_SUCCESS;
}

int TP3State::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "TP3State stopping GameState");

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int TP3State::update(float delta)
{
  // move camera
  static fV3 camera_move;
  camera_move = fV3(0, 0, 0);

  if(space) camera_move.y -= 0.5f;
  if(ctrl) camera_move.y += 0.5f;
  if(up) camera_move.z += 0.5f;
  if(down) camera_move.z -= 0.5f;

  if(left)
  {
    if(alt)
      camera_move.x += 0.5f;
    else
    {
      if(USE_MOULIS)
        engine.turnCamera(-1.0f);
      else
        camera_angle += 1.0f;
    }
  }
  if(right)
  {
    if(alt)
      camera_move.x -= 0.5f;
    else
    {
      if(USE_MOULIS)
        engine.turnCamera(1.0f);
      else
        camera_angle -= 1.0f;
    }
  }
  if(USE_MOULIS)
    engine.moveCamera(camera_move);
  else
    camera_offset += camera_move;

  // Update dynamic game objects
  int result = GameState::update(delta);
  if(result != CONTINUE)
    return result;

  // All clear
  return CONTINUE;
}

int TP3State::trigger(int which, bool pressed)
{
  switch(which)
  {
		case SDLK_SPACE: space = pressed; break;
		case SDLK_LCTRL: ctrl = pressed; break;
		case SDLK_LALT: alt = pressed; break;
		case SDLK_UP: up = pressed; break;
		case SDLK_DOWN: down = pressed; break;
		case SDLK_LEFT: left = pressed; break;
		case SDLK_RIGHT: right = pressed; break;
  }

  // All clear
  return CONTINUE;
}

void TP3State::draw()
{
  if(USE_MOULIS)
    engine.render();
  else
  {
    // clear and reset
    glPushMatrix();
      glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
      glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);
      MeshManager::getInstance()->mesh.draw();
    glPopMatrix();
  }

  // Draw dynamic game objects
  GameState::draw();
}
