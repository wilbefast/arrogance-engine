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

#include "Button.hpp"

#include "../warn.hpp"

/// CONSTRUCTORS, DESTRUCTORS

Button::Button(const char* name, Texture init_texture, fRect init_destination,
               fRect init_source) :
tag(numerise(name)),
texture(init_texture),
destination(init_destination),
source_off(init_source),
source_on(init_source + uV2(init_source.w,0)),
pressed(false)
{
    correctAspect(init_source, init_destination);
}

Button::Button(str_id init_tag, Texture init_texture, fRect init_destination,
               fRect init_source) :
tag(init_tag),
texture(init_texture),
destination(init_destination),
source_off(init_source),
source_on(init_source + uV2(init_source.w,0)),
pressed(false)
{
    correctAspect(init_source, init_destination);
}

void Button::correctAspect(fRect src_area, fRect dest_area)
{
    destination.setRatio(src_area.getRatio());
    destination.centreWithin(dest_area);
}

Button::~Button()
{
}

/// EACH UPDATE

void Button::draw()
{
    if(pressed)
        texture.draw(&source_on, &destination);
    else
        texture.draw(&source_off, &destination);
}

/// ACCESSORS

bool Button::contains(fV2 position) const
{
    return destination.contains(position);
}

bool Button::press(fV2 position, bool clicking)
{
    // switch off button if not touching, otherwise turn or or off based on
    // position on touch.
    return (pressed = (clicking && contains(position)));
}

str_id Button::getTag() const
{
    return tag;
}
