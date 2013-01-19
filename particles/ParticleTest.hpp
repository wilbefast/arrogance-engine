#ifndef PARTICLETEST_HPP_INCLUDED
#define PARTICLETEST_HPP_INCLUDED

#include "../scenes/GameState.hpp"

#include "../math/V3.hpp"
#include "../math/V4.hpp"

#include "../utils/IntrusiveLinked.hpp"

class Particle : public IntrusiveLinked
{
  /// CONSTANTS
private:
  static const size_t MAX_AGE = 1;
  static const float HALF_SIZE = 2;
  static Texture *texture;

  /// ATTRIUBTES
private:
  fV4 position, speed;
  float age;

  /// METHODS
public:
  Particle(fV3 p);
  void draw();
  void update(float delta);
  bool isOld() const;
};

class ParticleTest : public GameState
{
  /// CONSTANTS
private:
  static const float CAMERA_MAX_ANGLE = 90.0f;
  static const float PARTICLE_PERIOD = 0.1f;

  /// ATTRIBUTES
private:
  float camera_angle;
  fV3 camera_offset;
  bool left, right, up, down, alt, space, ctrl;
  Particle *first_particle, *current_particle;
  float particle_timer;

  /// METHODS
public:
  // Creation, destruction
  ParticleTest();
  int startup();
  int shutdown();
  // Overrides GameState
  int update(float delta);
  int trigger(int which, bool pressed);
  void draw();
};

#endif // PARTICLETEST_HPP_INCLUDED
