#include "TP3State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/Mesh3D.hpp"
#include "../graphics/draw.hpp"

#include "OBJLoader.hpp"
#include "../debug/log.h"

#define USE_MOLIS false

using namespace std;


/// CREATION, DESTRUCTION

TP3State::TP3State() :
GameState(),
camera_angle(),
camera_offset(),
engine()
{
}

int TP3State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "TP3State starting GameState");

  // load the 3D scene
  draw::use3D();
  if(USE_MOLIS)
    engine.setup();
  else
    MeshManager::getInstance()->startup();
  puts("ready");

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
  if(input.clicking)
  {
    if(USE_MOLIS)
      engine.keydown(SDLK_SPACE);
  }

  // Update dynamic game objects
  int result = GameState::update(delta);
  if(result != CONTINUE)
    return result;

  // All clear
  return CONTINUE;
}

void TP3State::draw()
{
  draw::use3D();
  if(USE_MOLIS)
    engine.render();
  else
    MeshManager::getInstance()->mesh.draw();

  // Draw dynamic game objects
  GameState::draw();
}
