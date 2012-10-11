#ifndef TP3STATE_HPP_INCLUDED
#define TP3STATE_HPP_INCLUDED

#include "../scenes/GameState.hpp"
#include "../math/V3.hpp"

class TP3State : public GameState
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
  TP3State();
  int startup();
  int shutdown();
  // Overrides GameState
  int update(float delta);
  void draw();
};

#endif // TP3STATE_HPP_INCLUDED
