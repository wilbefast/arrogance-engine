#include "math.h"

#include "Puppet.hpp"

#include "../graphics/draw.hpp"

/// CONSTRUCTORS, DESTRUCTORS

Puppet::Puppet(const char* _name, float _t_ref, float _y, bool _interpolate) :
name(_name),
where_now(0, Puppet::MIN_X, _y),
where_last(where_now) ,
where_before_last(where_now),
t_ref(_t_ref),
t_total(0),
t_partiel(0),
interpolate(_interpolate)
{
	// set initial position
	where_now.rotation = t_total;
	where_now.position.x = MIDDLE_X + cos(t_total)*HALF_SPAN_X;

  // recalculate visual position
	where_now.recalculateBox();

  // copy values to previous
	where_last = where_now;
	where_before_last = where_now;
}


Puppet::~Puppet(void)
{
}

/// POLLING

void Puppet::update(float delta)
{
	// update timers
	t_total += delta;	// not actually used
	t_partiel += delta;

	// constant updates
	if(t_ref <= 0)
	{
		// move and rotate the puppet
		where_now.rotation = t_total;
		where_now.position.x = MIDDLE_X + cos(t_total)*HALF_SPAN_X;

		// recalculate box position
    where_now.recalculateBox();
	}
	// timed updates
	else if(t_partiel > t_ref)
	{
		// decrement partial timer
		t_partiel -= t_ref;

		// move and rotate the puppet
		where_now.rotation = t_total;
		where_now.position.x = MIDDLE_X + cos(t_total)*HALF_SPAN_X;

    // recalculate box position
    where_now.recalculateBox();

		// update previous position holders
		where_before_last = where_last;
		where_last = where_now;
	}

	// interpolate if requested
  if(interpolate)
	{
		float frac = t_partiel/t_ref;
		where_now.position.x =
		(where_last.position.x - where_before_last.position.x)*frac
      + where_before_last.position.x,
		where_now.rotation = (where_last.rotation - where_before_last.rotation)*frac + where_before_last.rotation;

		// recalculate box position
		where_now.interpolateBox(where_before_last, where_last, frac);
	}
}

void Puppet::draw()
{
	// draw rectangle
	draw::line_loop(where_now.box, 4, Colour(255, 255, 255));

	// draw text
	/*RECT R;
	R.left = where_now.x;
	R.top = where_now.y;
	DrawText(context, name, 4, &R, DT_SINGLELINE|DT_NOCLIP);*/
}

void Puppet::Spatial::recalculateBox()
{
  // reset box position
  /*
	Rectangle layout is assumed to be:

	A--B
	|  |
	D--C

	Where values are stored in arrays: [x, y]
	*/


	// positions relative to the centre of the object
	static const fV2 REL[4] =
	{
	  fV2(-Puppet::HALF_SIZE, -Puppet::HALF_SIZE),
    fV2(Puppet::HALF_SIZE, -Puppet::HALF_SIZE),
    fV2(Puppet::HALF_SIZE, Puppet::HALF_SIZE),
    fV2(-Puppet::HALF_SIZE, Puppet::HALF_SIZE)
	};

	// cache trignometric values
	float cosr = cos(rotation), sinr = sin(rotation);

  // calculate positions relative to world origin
	for(size_t i = 0; i < 4; i++)
	  // apply rotations
    box[i] = fV2(REL[i].x*cosr - REL[i].y*sinr,
                    REL[i].x*sinr + REL[i].y*cosr)
                // offset to position of Puppet
                + position;
}

void Puppet::Spatial::interpolateBox(Spatial const& start, Spatial const& end,
                                     float frac)
{
  for(size_t i = 0; i < 4; i++)
    box[i] = start.box[i] + (end.box[i] - start.box[i])*frac;
}
