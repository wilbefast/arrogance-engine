#ifndef TP3STATE_HPP_INCLUDED
#define TP3STATE_HPP_INCLUDED

#include "../scenes/GameState.hpp"
#include "../math/V3.hpp"

#include "Engine.hpp"

class TP3State : public GameState
{
  /// CONSTANTS
private:
  static const float CAMERA_MAX_ANGLE = 90.0f;

  /// ATTRIBUTES
private:
  float camera_angle;
  fV3 camera_offset;
  Engine engine;
  bool left, right, up, down, alt, space, ctrl;

  /// METHODS
public:
  // Creation, destruction
  TP3State();
  int startup();
  int shutdown();
  // Overrides GameState
  int update(float delta);
  int trigger(int which, bool pressed);
  void draw();
};

#endif // TP3STATE_HPP_INCLUDED
