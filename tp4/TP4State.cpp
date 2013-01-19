#include "TP4State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/draw.hpp"
#include "../debug/log.h"

#define TURN_SPEED 1


using namespace std;

static M44<GLfloat> BASECUBE[CUBES_PER_SIDE][CUBES_PER_SIDE][CUBES_PER_SIDE];

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

  // set up cube positions
  for(int x = 0; x < CUBES_PER_SIDE; x++)
  for(int y = 0; y < CUBES_PER_SIDE; y++)
  for(int z = 0; z < CUBES_PER_SIDE; z++)
  {
    BASECUBE[x][y][z].toIdentity();
    BASECUBE[x][y][z].col[3] = V4<GLfloat>((x-1)*CUBE_SPACING,
                                       (y-1)*CUBE_SPACING,
                                       (z-1)*CUBE_SPACING, 1);
    cube[x][y][z] = BASECUBE[x][y][z];
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
  static fM44 turn_mat[3] = { fM44::xRotMatrix(DEG2RAD(TURN_SPEED)),
                            fM44::yRotMatrix(DEG2RAD(TURN_SPEED)),
                            fM44::zRotMatrix(DEG2RAD(TURN_SPEED)) };

  static bool first = true;
  if(first)
    cout << turn_mat[0] << turn_mat[1] << turn_mat[2] << endl;
  first = false;


  cube_turn += TURN_SPEED;

  if(cube_turn.getDeg()%90 == 0)
  {
    // choose new axis upon which to turn
    size_t axis_i = rand()%3;
    cube_axis = iV3(0,0,0);
    cube_axis[axis_i] = 1;

    // choose which rank of cubes upon this axis will turn
    cube_rank = rand()%3;
  }

  /*for(size_t i = 0; i < CUBES_PER_SIDE; i++)
  for(size_t j = 0; j < CUBES_PER_SIDE; j++)
  {
    if(cube_axis.x)
      cube[cube_rank][i][j] = turn_mat[0]*cube[cube_rank][i][j];

    else if(cube_axis.y)
      cube[i][cube_rank][j] = turn_mat[1]*cube[i][cube_rank][j];

    else if(cube_axis.z)
      cube[i][j][cube_rank] = turn_mat[2]*cube[i][j][cube_rank];
  }*/
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
      //! DRAW MESH HERE
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
