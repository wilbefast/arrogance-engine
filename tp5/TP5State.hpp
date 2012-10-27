#ifndef TP5STATE_HPP_INCLUDED
#define TP5STATE_HPP_INCLUDED

#include "../math/V3.hpp"

#include "../scenes/GameState.hpp"


class TP5State : public GameState
{
  /// CONSTANTS
private:
  static const float CAMERA_MAX_ANGLE = 90.0f;

  /// ATTRIBUTES
private:
  float camera_angle;
  fV3 camera_offset;
  bool left, right, up, down, alt, space, ctrl;

  /// METHODS
public:
  // Creation, destruction
  TP5State();
  int startup();
  int shutdown();
  // Overrides GameState
  int update(float delta);
  int trigger(int which, bool pressed);
  void draw();
};

#endif // TP5STATE_HPP_INCLUDED
