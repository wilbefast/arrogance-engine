#ifndef STRATEGYSTATE_HPP_INCLUDED
#define STRATEGYSTATE_HPP_INCLUDED

#include "../math/V3.hpp"

#include "GameState.hpp"

class StrategyState : public GameState
{
  /// CONSTANTS
private:
  static const float CAMERA_MAX_ANGLE = 90.0f;

  /// ATTRIBUTES
private:
  iV3 camera_angle;
  iV3 camera_offset;

  /// METHODS
public:
  // Creation, destruction
  StrategyState();
  int startup();
  int shutdown();
  // Overrides GameState
  int update();
  void draw();
};

#endif // STRATEGYSTATE_HPP_INCLUDED
