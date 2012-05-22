#include "BoundaryEvent.hpp"
#include "../Thing.hpp"

/// FUNCTIONS

void BoundaryEvent::generate(Thing* t, fRect* boundary)
{
    // Save the side upon which the event occured
    iV2 side;

    // Generate out of bounds event for the Thing
    if(t->isOutside(boundary, &side))
        t->addEvent(new BoundaryEvent("out_of_bounds", side));

    else
    // Generate intersect boundary event for the Thing
    if(t->isLeaving(boundary, &side))
        t->addEvent(new BoundaryEvent("intersect_bounds", side));


}

/// METHODS

// constructors, destructors

BoundaryEvent::BoundaryEvent(const char* init_name, iV2 init_side) :
ThingEvent(init_name),
boundary_side(init_side)
{
}

// accessors

iV2 BoundaryEvent::getSide() const
{
    return boundary_side;
}
