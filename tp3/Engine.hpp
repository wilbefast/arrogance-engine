#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "OBJLoader.hpp"

class Engine
{
	//! NESTED DEFINITIONS
public:
	struct camera_t
	{
		float target_x, target_y, target_z,
          distance,
          elevation, // between 0 and 90
          rotation; // between 0 and 360
	};

	//! ATTRIBUTES
private:
	SCENE* scene;
	camera_t camera;

  //! METHODS
public:
  void render(unsigned int u32Width, unsigned int u32Height);
  void setup();
	void keydown(int key);
};

#endif // ENGINE_HPP_INCLUDED
