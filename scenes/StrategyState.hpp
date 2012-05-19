#ifndef STRATEGYSTATE_HPP_INCLUDED
#define STRATEGYSTATE_HPP_INCLUDED

#include "GameState.hpp"

class StrategyState : public GameState
{
  /// CONSTANTS
private:

  /// ATTRIBUTES
private:

  /// METHODS
public:
  // Creation, destruction
  StrategyState();
  // Overrides GameState
  int update();
  void draw();
};

#endif // STRATEGYSTATE_HPP_INCLUDED
