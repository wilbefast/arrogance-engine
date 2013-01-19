#include "ParticleTest.hpp"

#include "../global.hpp"

#include "../debug/assert.h"
#include "../debug/log.h"

#include "../graphics/draw.hpp"

#include "../io/GraphicsManager.hpp"

#include "../math/opengl_matrix.hpp"

using namespace std;


//!-----------------------------------------------------------------------------
//! GLOBAL
//!-----------------------------------------------------------------------------

static Texture* t;

//!-----------------------------------------------------------------------------
//! PARTICLES
//!-----------------------------------------------------------------------------

Particle::Particle(fV3 p) :
age(0)
{
  position.x = p.x;
  position.y = p.y;
  position.z = p.z;

  speed.y = -40 + rand()%20 - rand()%20;
  speed.x = rand()%20 - rand()%20;
  speed.z = rand()%20 - rand()%20;
  speed *= 0.01f;
}

void Particle::update(float delta)
{
  age += delta;
  position += speed;
  speed.y += delta;
}

void Particle::draw()
{
  M44<GLfloat> const* mv = getGLMatrix(GL_MODELVIEW_MATRIX);

  V4<GLfloat> ox = mv->row(0), oy = mv->row(1);


  float half_size = HALF_SIZE * (0.1 + 0.9*(MAX_AGE - age)/MAX_AGE);

  V4<GLfloat> p1 = position - ox*half_size + oy*half_size,
              p2 = position + ox*half_size + oy*half_size,
              p3 = position + ox*half_size - oy*half_size,
              p4 = position - ox*half_size - oy*half_size;

  glBindTexture(GL_TEXTURE_2D, t->getHandle());
  glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3fv(p1.front());
    glTexCoord2f(1, 0);
    glVertex3fv(p2.front());
    glTexCoord2f(1, 1);
    glVertex3fv(p3.front());
    glTexCoord2f(0, 1);
    glVertex3fv(p4.front());
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
}

bool Particle::isOld() const
{
  return (age > Particle::MAX_AGE);
}

//!-----------------------------------------------------------------------------
//! PARTICLE TEST
//!-----------------------------------------------------------------------------

/// CREATION, DESTRUCTION

ParticleTest::ParticleTest() :
GameState(),
camera_angle(180.0f),
camera_offset(0, 0, 10),
left(false),
right(false),
up(false),
down(false),
alt(false),
space(false),
ctrl(false),
first_particle(NULL),
current_particle(NULL),
particle_timer(0.0f)
{
}

int ParticleTest::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "ParticleTest starting GameState");


  t = GraphicsManager::getInstance()->get_texture("sprite");

  // all clear
  return EXIT_SUCCESS;
}

int ParticleTest::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "ParticleTest stopping GameState");

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int ParticleTest::update(float delta)
{
  // create particles
  particle_timer += delta;
  if(particle_timer >= ParticleTest::PARTICLE_PERIOD)
  {
    particle_timer = 0;

    Particle* newbie = new Particle(fV3());

    if(first_particle)
      first_particle->newNext(newbie);
    else
      first_particle = newbie;

  }


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
  camera_offset += camera_move * 0.3f;

  // update particles
  if(first_particle)
  {
    current_particle = first_particle;
    do
    {
      Particle* prev_particle = current_particle;
      current_particle->update(delta);
      current_particle = (Particle*)current_particle->getNext();
      if(prev_particle->isOld())
      {
        if(prev_particle == first_particle)
        {
          if(current_particle == first_particle)
            first_particle = NULL;
          else
            first_particle = current_particle;
        }
        delete prev_particle;
      }
    }
    while(first_particle && current_particle != first_particle);
  }

  // All clear
  return CONTINUE;
}

int ParticleTest::trigger(int which, bool pressed)
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

void ParticleTest::draw()
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();


    glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      GLdouble height = tan(60.0f / 360 * PI),
                width = height;
      glFrustum(-width, width, height, -height, 1, 10000);



    glMatrixMode(GL_MODELVIEW);
      // take camera into account
      glRotatef(camera_angle, 0, 1, 0);
      glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);



      // draw particles
      if(first_particle)
      {
        current_particle = first_particle;
        do
        {
          current_particle->draw();
          current_particle = (Particle*)current_particle->getNext();
        }
        while(first_particle && current_particle != first_particle);
      }


  glPopMatrix();
}
