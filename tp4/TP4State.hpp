#ifndef TP4STATE_HPP_INCLUDED
#define TP4STATE_HPP_INCLUDED

#include "../scenes/GameState.hpp"
#include "../math/V3.hpp"
#include "../math/M44.hpp"
#include "../graphics/3D/Camera.hpp"

#define CUBES_PER_SIDE 3
#define CUBE_SPACING 20

class TP4State : public GameState
{
  /// CONSTANTS
private:
  static const float CAMERA_MAX_ANGLE = 90.0f;

  /// ATTRIBUTES
private:
  Camera camera;
  bool left, right, up, down, alt, space, ctrl;
  // rubik's cube
  M44<GLfloat> cube[CUBES_PER_SIDE][CUBES_PER_SIDE][CUBES_PER_SIDE];
  Angle cube_turn;
  iV3 cube_axis;  // along which axis are we turning
  size_t cube_rank;  // which rank of cubes is being turned

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
