/*
Arrogance Engine: a simple SDL/OpenGL game engine for Desktop and Android.
Copyright (C) 2012 William James Dyce

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
