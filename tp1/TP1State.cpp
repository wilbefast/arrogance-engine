#include "TP1State.hpp"

#include "../debug/assert.h"

/// CREATION, DESTRUCTION

#define N_PUPPETS 5
Puppet puppet[N_PUPPETS] =
{
  Puppet("CNST", 0.0f, Puppet::SIZE),
  Puppet("60Hz", 1.0f/60.0f, Puppet::SIZE*3),
  Puppet("30Hz", 1.0f/30.0f, Puppet::SIZE*5),
  Puppet("05Hz", 1.0f/5.0f, Puppet::SIZE*7),
  Puppet("INTP", 1.0f/5.0f, Puppet::SIZE*9, true)
};

TP1State::TP1State() :
GameState()
{
}

int TP1State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "StrategyState starting GameState");

  // Occlusion on
  /*glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
  // Lighting on
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);*/

  // all clear
  return EXIT_SUCCESS;
}

int TP1State::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "StrategyState stopping GameState");

  // Occlusion off
  /*glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  // Lighting off
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);*/

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int TP1State::update(float delta)
{
  // Update puppets
  for(size_t i = 0; i < N_PUPPETS; i++)
    puppet[i].update(delta);

  // Update dynamic game objects
  int result = GameState::update(delta);
  if(result != CONTINUE)
    return result;

  // All clear
  return CONTINUE;
}

void TP1State::draw()
{
  // Draw puppets
  for(size_t i = 0; i < N_PUPPETS; i++)
    puppet[i].draw();

  // Draw dynamic game objects
  GameState::draw();
}
