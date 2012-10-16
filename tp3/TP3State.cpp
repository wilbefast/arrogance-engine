#include "TP3State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/Mesh3D.hpp"
#include "../graphics/draw.hpp"

#include "OBJLoader.hpp"
#include "../debug/log.h"

using namespace std;


/// CREATION, DESTRUCTION

TP3State::TP3State() :
GameState(),
camera_angle(),
camera_offset(),
engine()
{
}

#include <iostream> /// FIXME
int TP3State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "TP3State starting GameState");

  // load the 3D scene
  draw::use3D();
  engine.setup();

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
  engine.render(global::viewport.x,global::viewport.y);

  // Draw dynamic game objects
  GameState::draw();
}
