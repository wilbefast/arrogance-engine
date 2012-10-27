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
#include "../../../graphics/2D/GraphicIncarnation.hpp"

class GraphicElement : public ThingElement, public GraphicIncarnation
{
  /// METHODS
public:
  // contructors, destructors
  GraphicElement(Thing* init_owner, fV2 _size = fV2(0.0f, 0.0f),
                                    fV2 _offset = fV2(0.0f, 0.0f),
                                    char _flags = CENTER_X|CENTER_Y);
  ~GraphicElement();
  // overrides
  virtual int update(GameState* context, float delta);     // ThingElement
  virtual void draw();                        // ThingElement
};

#endif // GRAPHICELEMENT_HPP_INCLUDED
