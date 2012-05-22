#ifndef COLLIDERELEMENT_HPP_INCLUDED
#define COLLIDERELEMENT_HPP_INCLUDED

class ColliderElement;

#include "ThingElement.hpp"
#include "../../../math/Rect.hpp"

class ColliderElement : public ThingElement
{
    /// ATTRIBUTES
    private:
    fRect hitbox;

    /// METHODS
    public:
    // constructors, destructors
    ColliderElement(Thing* init_owner, fV2 size, fV2 offset = fV2(0,0));
    ColliderElement(const ColliderElement& src);
    // main methods
    bool isColliding(ColliderElement* other, iV2* side = NULL) const;
    bool isOutside(fRect* bounds, iV2* side = NULL) const;
    bool isLeaving(fRect* bounds, iV2* side = NULL) const;
    bool doesContain(fV2 point) const;
    // accessors
    fRect getOffsetBox() const;
    iV2 boundarySide(fRect* bounds) const;
    iV2 collisionSide(ColliderElement* other) const;

};

#endif // COLLIDERELEMENT_HPP_INCLUDED
