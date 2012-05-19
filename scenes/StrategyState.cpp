#include "StrategyState.hpp"

#include "../global.hpp"                          // for viewport

/// CREATION, DESTRUCTION

StrategyState::StrategyState() :
GameState()
{
}

/// OVERRIDES GAMESTATE

int StrategyState::update()
{
  int result;

  // Update dynamic game objects
  result = GameState::update();
  if(result != CONTINUE)
    return result;


  // All clear
  return EXIT_SUCCESS;
}

void StrategyState::draw()
{

  // Draw dynamic game objects
  GameState::draw();
}
