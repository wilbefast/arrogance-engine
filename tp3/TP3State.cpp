#include "TP3State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/draw.hpp"

#include "OBJLoader.hpp"
#include "../debug/log.h"

//#define USE_MOULIS
#define OBJ_FILE "assets/Island_001.obj"//"assets/cube.obj"

using namespace std;


/// CREATION, DESTRUCTION

TP3State::TP3State() :
GameState(),
camera_angle(180.0f),
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
  #ifdef USE_MOULIS
    engine.setup(OBJ_FILE);
  #else
    MeshManager::getInstance()->mesh.load_obj(OBJ_FILE);
  #endif // ifdef USE_MOULIS

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
  // cache trigonometry
  float camera_cos_radians = cos(0.0174532925*camera_angle);
  float camera_sin_radians = sin(0.0174532925*camera_angle);

  // move camera
  static fV3 camera_move;
  camera_move = fV3(0, 0, 0);

  if(space) camera_move.y -= 1;
  if(ctrl) camera_move.y += 1;
  if(up)
  {
    camera_move.x -= camera_sin_radians;
    camera_move.z += camera_cos_radians;
  }
  if(down)
  {
    camera_move.x += camera_sin_radians;
    camera_move.z -= camera_cos_radians;
  }

  if(left)
  {
    if(alt)
    {
      camera_move.x += camera_cos_radians;
      camera_move.z += camera_sin_radians;
    }
    else
    {
      #ifdef USE_MOULIS
        engine.turnCamera(-1.0f);
      #else
        camera_angle -= 1.0f;
      #endif
    }
  }
  if(right)
  {
    if(alt)
    {
      camera_move.x -= camera_cos_radians;
      camera_move.z -= camera_sin_radians;
    }
    else
    {
      #ifdef USE_MOULIS
        engine.turnCamera(1.0f);
      #else
        camera_angle += 1.0f;
      #endif
    }
  }
  #ifdef USE_MOULIS
    engine.moveCamera(camera_move);
  #else
    camera_offset += camera_move * 0.3f;
  #endif // ifdef USE_MOULIS

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
  #ifdef USE_MOULIS
    engine.render();
  #else
    glPushMatrix();
      glRotatef(camera_angle, 0, 1, 0);
      glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
      MeshManager::getInstance()->mesh.draw();
    glPopMatrix();
  #endif // ifdef USE_MOULIS

  // Draw dynamic game objects
  GameState::draw();
}
