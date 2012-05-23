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

#ifndef ANIMATEDELEMENT_HPP_INCLUDED
#define ANIMATEDELEMENT_HPP_INCLUDED

#include "GraphicElement.hpp"
#include "../../../graphics/Animation.hpp"

class AnimatedElement : public GraphicElement
{
    /// ATTRIBUTES
    private:
    float frame_current;
    float frame_speed;

    /// METHODS
    public:
    // contructors, destructors
    AnimatedElement(Thing* init_owner, fV2 size, fV2 offset = fV2(0.0f, 0.0f));
    ~AnimatedElement();
    // accessors
    void setFrame(float new_frame);
    void setRandFrame();
    bool setSprite(Animation* new_sprite, float new_speed = 0);
    // overrides
    int update(GameState* context);     // ThingElement
    void draw();                        // ThingElement

    /// SUBROUTINES
    private:
    void loopAnim();

};

#endif // ANIMATEDELEMENT_HPP_INCLUDED
