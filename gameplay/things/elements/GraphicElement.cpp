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

#include "GraphicElement.hpp"

// Constructors, destructors

GraphicElement::GraphicElement(Thing* init_owner, fV2 size, fV2 _offset) :
ThingElement(init_owner),
sprite(NULL),
destination(size),
offset(_offset),
angle(0)
{
}

GraphicElement::~GraphicElement()
{
    // NB - the sprite is NOT freed as it may in use by other components!
}

// Accessors

void GraphicElement::setAngle(float new_angle)
{
    angle = new_angle;
}

bool GraphicElement::setSprite(Graphic* new_sprite)
{
    // Break if sprite is the same as before
    if(sprite == new_sprite)
        return false;

    // Reset everything only if a new Graphic has been selected
    sprite = new_sprite;
    centreFrame();

    // Graphic was indeed changed
    return true;
}

int GraphicElement::update(GameState* context)
{
    // Move destination rectangle to position of object
    destination.setPosition(owner->getPosition() + offset);

    // No interruption
    return SceneState::CONTINUE;
}

void GraphicElement::draw()
{
    // Get the source rectangle by cutting out the appropriate frame
    static fRect source;
    source = sprite->getFrame();

    // Draw the graphic
    sprite->getTexture()->draw(&source, &destination, angle);
}

// Subroutines

void GraphicElement::centreFrame()
{
    // Use the full frame size
    fV2 frame_size = sprite->getFrame().getSize();

    // Centre it
    offset = -frame_size/2.0f;
    destination = fRect(owner->getPosition() + offset, frame_size);

    // Maintain aspect ratio (stretch evenly along)
    destination.setRatio(sprite->getFrame().getRatio());
}

