#include "CollisionEvent.hpp"

/// FUNCTIONS

void CollisionEvent::generate(Thing* a, Thing* b)
{
    // Save the side upon which the event occured
    iV2 side;

    // Check for collisions
    if(!a->isDead() && !b->isDead() && a->isColliding(b, &side))
    {
        // Generate collision events
        a->addEvent(new CollisionEvent(b, side));
        b->addEvent(new CollisionEvent(a, -side));
    }
}

/// METHODS

// constructors, destructor

CollisionEvent::CollisionEvent(Thing* init_other, iV2 init_side) :
ThingEvent("collision"),
other(init_other),
collision_side(init_side)
{
}

// accessors
Thing* CollisionEvent::getOther()
{
    return other;
}

iV2 CollisionEvent::getSide() const
{
    return collision_side;
}
