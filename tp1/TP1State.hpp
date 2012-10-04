#ifndef TP1STATE_HPP_INCLUDED
#define TP1STATE_HPP_INCLUDED

#include "Puppet.hpp"
#include "../scenes/GameState.hpp"

class TP1State : public GameState
{
  /// CONSTANTS

  /// ATTRIBUTES

  /// METHODS
public:
  // Creation, destruction
  TP1State();
  int startup();
  int shutdown();
  // Overrides GameState
  int update(float delta);
  void draw();
};


#endif // TP1STATE_HPP_INCLUDED
