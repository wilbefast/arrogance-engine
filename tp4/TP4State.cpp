#include "TP4State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/draw.hpp"
#include "../debug/log.h"


using namespace std;


/// CREATION, DESTRUCTION

TP4State::TP4State() :
GameState(),
camera(),
left(false),
right(false),
up(false),
down(false),
alt(false),
space(false),
ctrl(false),
cube_turn(0),
cube_axis(1,0,0),
cube_rank(0)
{
}


//! FIXME - EXPORT THIS TO A NEW COMPILATION UNIT
//! ----------------------------------------------------------------------------
static GLfloat a[16];
static M44<GLfloat> m;

void print_glmatrix(GLenum which_matrix)
{
  glGetFloatv(which_matrix, a);
  m.importArray(a);
  cout << m << endl;
}

void mult_glmatrix(M44<GLfloat>& transform)
{
  transform.exportArray(a);
  glMultMatrixf(a);
}
//! ----------------------------------------------------------------------------

int TP4State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "TP4State starting GameState");

  // load the 3D scene
  draw::use3D();
  MeshManager::getInstance()->mesh.load_obj("assets/rubik.obj");

  // set up cube positions
  for(int x = 0; x < CUBES_PER_SIDE; x++)
  for(int y = 0; y < CUBES_PER_SIDE; y++)
  for(int z = 0; z < CUBES_PER_SIDE; z++)
  {
    cube[x][y][z].toIdentity();
    cube[x][y][z].col[3] = V4<GLfloat>((x-1)*CUBE_SPACING,
                                       (y-1)*CUBE_SPACING,
                                       (z-1)*CUBE_SPACING, 1);
  }

  // all clear
  return EXIT_SUCCESS;
}

int TP4State::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "TP4State stopping GameState");

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int TP4State::update(float delta)
{
  // turn the appropriate side
  cube_turn += 1;
  if(cube_turn.getDeg()%90 == 0)
  {
    // choose new axis upon which to turn
    size_t axis_i = rand()%3;
    cube_axis = iV3(0,0,0);
    cube_axis[axis_i] = 1;

    // choose which rank of cubes upon this axis will turn
    cube_rank = rand()%3;
  }

  // move camera
  static fV3 camera_move;
    camera_move = fV3(0, 0, 0);
  static float camera_angle;
    camera_angle = 0.0f;

  if(space) camera_move.y -= 0.5f;
  if(ctrl) camera_move.y += 0.5f;
  if(up) camera_move.z += 0.5f;
  if(down) camera_move.z -= 0.5f;

  if(left)
  {
    if(alt)
      camera_move.x += 0.5f;
    else
      camera_angle -= 1.0f;
  }
  if(right)
  {
    if(alt)
      camera_move.x -= 0.5f;
    else
      camera_angle += 1.0f;
  }

  // Apply the camera movement
  camera.turn(camera_angle);
  camera.pan(camera_move);

  // Update dynamic game objects
  int result = GameState::update(delta);
  if(result != CONTINUE)
    return result;

  // All clear
  return CONTINUE;
}

int TP4State::trigger(int which, bool pressed)
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

void TP4State::draw_rubik()
{
  for(size_t x = 0; x < CUBES_PER_SIDE; x++)
  for(size_t y = 0; y < CUBES_PER_SIDE; y++)
  for(size_t z = 0; z < CUBES_PER_SIDE; z++)
  {
    glPushMatrix();
      if((cube_axis.x && x == cube_rank)
      || (cube_axis.y && y == cube_rank)
      || (cube_axis.z && z == cube_rank))
        glRotatef(cube_turn.getDeg(), cube_axis.x, cube_axis.y, cube_axis.z);
      mult_glmatrix(cube[x][y][z]);
      MeshManager::getInstance()->mesh.draw();
    glPopMatrix();
  }
}

void TP4State::draw()
{
  // clear and reset
  glPushMatrix();
    camera.lookThrough();
    draw_rubik();
  glPopMatrix();

  // Draw dynamic game objects
  GameState::draw();
}
