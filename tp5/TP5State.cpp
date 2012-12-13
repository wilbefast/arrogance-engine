#include "TP5State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/draw.hpp"
#include "../debug/log.h"


using namespace std;


/// CREATION, DESTRUCTION

TP5State::TP5State() :
GameState(),
camera_angle(0.0f),
camera_offset(),
left(false),
right(false),
up(false),
down(false),
alt(false),
space(false),
ctrl(false),
shader()
{
}

int TP5State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "TP5State starting GameState");

  // load the 3D scene
  draw::use3D();
  ASSERT(MeshManager::getInstance()->mesh.
    load_obj("assets/BerkeleyDragon.obj") == EXIT_SUCCESS, "Loading mesh");

  // load the shader
  //shader.load("assets/shaders/green.frag");
  //shader.load_all(2, "assets/shaders/normals.vert", "assets/shaders/normals.frag");
  shader.load_all(2, "assets/shaders/wobble.vert", "assets/shaders/wobble.frag");


  // all clear
  return EXIT_SUCCESS;
}

int TP5State::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "TP5State stopping GameState");

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int TP5State::update(float delta)
{
  static float time = 0;
  time ++;
  shader.parameter("time", time);

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
      camera_angle -= 1.0f;
  }
  if(right)
  {
    if(alt)
    {
      camera_move.x -= camera_cos_radians;
      camera_move.z -= camera_sin_radians;
    }
    else
      camera_angle += 1.0f;
  }
    camera_offset += camera_move;

  // Update dynamic game objects
  int result = GameState::update(delta);
  if(result != CONTINUE)
    return result;

  // All clear
  return CONTINUE;
}

int TP5State::trigger(int which, bool pressed)
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
		case 's': shader.activate(); break;
		case 'x': shader.deactivate(); break;
  }

  // All clear
  return CONTINUE;
}


void TP5State::draw()
{

  // clear and reset
  glPushMatrix();
    glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
    MeshManager::getInstance()->mesh.draw();
  glPopMatrix();

  // Draw dynamic game objects
  GameState::draw();
}
