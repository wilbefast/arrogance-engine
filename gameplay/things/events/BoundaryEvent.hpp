#ifndef BOUNDARYEVENT_HPP_INCLUDED
#define BOUNDARYEVENT_HPP_INCLUDED

#include "ThingEvent.hpp"
#include "../../../math/V2.hpp"
#include "../../../math/Rect.hpp"

class Thing;

class BoundaryEvent : public ThingEvent
{
    /// FUNCTIONS
    public:
    static void generate(Thing* t, fRect* boundary);

    /// ATTRIBUTES
    private:
    iV2 boundary_side;

    /// METHODS
    public:
    // constructor, destructors
    BoundaryEvent(const char* init_name, iV2 init_side);
    // accessors
    iV2 getSide() const;
};

#endif // BOUNDARYEVENT_HPP_INCLUDED
