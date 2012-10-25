#ifndef TP4STATE_HPP_INCLUDED
#define TP4STATE_HPP_INCLUDED

#include "../scenes/GameState.hpp"
#include "../math/V3.hpp"
#include "../math/M44.hpp"

#include "Engine.hpp"

#define N_CUBES 26

class TP4State : public GameState
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
  // rubik's cube
  M44<GLfloat> cube[N_CUBES];

  /// METHODS
public:
  // Creation, destruction
  TP4State();
  int startup();
  int shutdown();
  // Overrides GameState
  int update(float delta);
  int trigger(int which, bool pressed);
  void draw();

private:
  void draw_rubik();
};

#endif // TP4STATE_HPP_INCLUDED
