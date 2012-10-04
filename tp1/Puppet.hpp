#ifndef PUPPET_HPP_INCLUDED
#define PUPPET_HPP_INCLUDED

#include "../math/V2.hpp"

class Puppet
{
	/// CONSTANTS
private:
	const static int SIZE = 64,
										HALF_SIZE = SIZE/2,
										SPEED = 250,
										ROT_SPEED = 1,
										MIN_X = SIZE,
										MAX_X = MIN_X + SIZE*8,
										SPAN_X = MAX_X - MIN_X,
										HALF_SPAN_X = SPAN_X/2,
										MIDDLE_X = MIN_X + SPAN_X;

	/// NESTED DEFINITIONS
private:
	class Spatial
	{
		// attributes
	public:
		float rotation;
    fV2 position;
		int direction; // either 1 or -1

		// constructor
	public:
		Spatial(float _rotation, float _x, float _y) :
				rotation(_rotation), position(_x, _y), direction(1) {}
	};

	/// ATTRIBUTES
private:
	// debug information
	const char* name;
	// position and rotations
	Spatial where_now,
						where_last,
						where_before_last;
	// timing
	float t_ref,
				t_total,
				t_partiel;
	// perform interpolation ?
	bool interpolate;

	/// METHODS
public:
	// creation, destruction
	Puppet(const char* _name, float _t_ref, float _y = 0.0f,
        bool _interpolate = false);
	virtual ~Puppet();
	// polling
	void update(float delta);
	void draw();
};

#endif // PUPPET_HPP_INCLUDED

