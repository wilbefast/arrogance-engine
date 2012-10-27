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
camera_angle(0.0f),
camera_offset(),
left(false),
right(false),
up(false),
down(false),
alt(false),
space(false),
ctrl(false)
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
MeshManager::getInstance()->startup();

  // set up cube positions
  for(size_t i = 0; i < N_CUBES; i++)
  {
    cube[i].toIdentity();
    cube[i].col[3] = V4<GLfloat>(i*10, 0, 0, 1);

    cout << "\t--------CUBE[" << i << "]--------" << endl;
    cout << "matrix = " << cube[i] << endl;
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
      camera_angle -= 1.0f;
  }
  if(right)
  {
    if(alt)
      camera_move.x -= 0.5f;
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
  static bool first = true;

  for(size_t i = 0; i < N_CUBES; i++)
  {
    glPushMatrix();
    if(first && i == 1)
      print_glmatrix(GL_MODELVIEW_MATRIX);
    mult_glmatrix(cube[i]);
    //glTranslatef(10.0*i, 0.0f, 0.0f);
    if(first && i == 1)
      print_glmatrix(GL_MODELVIEW_MATRIX);

    MeshManager::getInstance()->mesh.draw();
    glPopMatrix();
  }

  first =  false;

}

void TP4State::draw()
{
  // clear and reset
  glPushMatrix();
    glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
    glRotatef(camera_angle, 0.0f, 1.0f, 0.0f);
    draw_rubik();
  glPopMatrix();

  // Draw dynamic game objects
  GameState::draw();
}
