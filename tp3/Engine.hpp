#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include "../math/V3.hpp"
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
  void render();
  void setup(const char* obj_file);
	void keydown(int key);
	void print(std::ostream& out) const;

	void moveCamera(fV3 amount);
	void turnCamera(float amount);
};

#endif // ENGINE_HPP_INCLUDED
