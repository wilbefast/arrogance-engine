#ifndef COLLISIONEVENT_HPP_INCLUDED
#define COLLISIONEVENT_HPP_INCLUDED

#include "ThingEvent.hpp"
#include "../Thing.hpp"

class CollisionEvent : public ThingEvent
{
    /// FUNCTIONS
    public:
    static void generate(Thing* a, Thing* b);

    /// ATTRIBUTES
    private:
    Thing* other;
    iV2 collision_side;

    /// METHODS
    public:
    // constructors, destructor
    CollisionEvent(Thing* other, iV2 side);
    // accessors
    Thing* getOther();
    iV2 getSide() const;
};

#endif // COLLISIONEVENT_HPP_INCLUDED
