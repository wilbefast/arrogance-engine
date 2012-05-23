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

#include <stdlib.h>

#include "AnimatedElement.hpp"
#include "../events/ThingEvent.hpp"       // for animation_end event

// Constructors, destructors

AnimatedElement::AnimatedElement(Thing* init_owner, fV2 size, fV2 offset) :
GraphicElement(init_owner, size, offset),
frame_current(0),
frame_speed(0)
{
}

AnimatedElement::~AnimatedElement()
{
    // NB - the sprite is NOT freed as it may in use by other components!
}

// Accessors

bool AnimatedElement::setSprite(Animation* new_sprite, float new_speed)
{
    // Standard setting operation
    if(!GraphicElement::setSprite(new_sprite))
        return false;

    // NB - speed is not reset if sprite is the same!
    frame_current = 0;
    frame_speed = new_speed;
    centreFrame();

    // Graphic was indeed changed
    return true;
}

void AnimatedElement::setFrame(float new_frame)
{
    frame_current = new_frame;
    loopAnim();
}

void AnimatedElement::setRandFrame()
{
    // pick random frame
    frame_current = rand() % ((Animation*)sprite)->getNumFrames();
}

void AnimatedElement::loopAnim()
{
    // Detect if we're over the maximum number of frames
    int frame_number = ((Animation*)sprite)->getNumFrames();

    if(frame_current >= frame_number)
    {
        // Loop animation
        frame_current -= frame_number;
        // Signal animation end
        owner->addEvent(new ThingEvent("animation_end"));
    }

}

int AnimatedElement::update(GameState* context)
{
    // Animate
    frame_current += frame_speed;
    loopAnim();

    // Move destination rectangle to position of object
    destination.setPosition(owner->getPosition() + offset);

    // No interruption
    return SceneState::CONTINUE;
}

void AnimatedElement::draw()
{
    // Get the source rectangle by cutting out the appropriate frame
    static fRect source;
    source = ((Animation*)sprite)->getFrame(frame_current);

    // Draw the graphic
    sprite->getTexture()->draw(&source, &destination, angle);
}
