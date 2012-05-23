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

#ifndef GRAPHICELEMENT_HPP_INCLUDED
#define GRAPHICELEMENT_HPP_INCLUDED

#include "ThingElement.hpp"
#include "../../../graphics/Graphic.hpp"

class GraphicElement : public ThingElement
{
    /// ATTRIBUTES
    protected:
    Graphic* sprite;
    fRect destination;
    fV2 offset;
    float angle;

    /// METHODS
    public:
    // contructors, destructors
    GraphicElement(Thing* init_owner, fV2 size, fV2 _offset = fV2(0.0f, 0.0f));
    ~GraphicElement();
    // accessors
    void setAngle(float new_angle);
    bool setSprite(Graphic* new_sprite);
    // overrides
    virtual int update(GameState* context);     // ThingElement
    virtual void draw();                        // ThingElement
    // subroutines
    protected:
    void centreFrame();

};

#endif // GRAPHICELEMENT_HPP_INCLUDED
