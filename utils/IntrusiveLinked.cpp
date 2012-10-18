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

#include "IntrusiveLinked.hpp"

#include <stdlib.h> // for NULL

//! CONSTRUCTORS, DESTRUCTORS

IntrusiveLinked::IntrusiveLinked() :
next(this),
prev(this)
{
}

IntrusiveLinked::~IntrusiveLinked()
{
  if(next != this)
    next->prev = prev;
  if(prev != this)
    prev->next = next;
}

void IntrusiveLinked::deleteConnections()
{
  while(next != this)
    delete next;
}

//! NAVIGATION

IntrusiveLinked* IntrusiveLinked::getNext() const
{
  return next;
}

IntrusiveLinked* IntrusiveLinked::getPrev() const
{
  return prev;
}

//! ADD A NEW LINK

IntrusiveLinked* IntrusiveLinked::newPrev(IntrusiveLinked* newbie)
{
  newbie->next = this;
  prev->next = newbie;
  newbie->prev = prev;
  return prev = newbie; // return the new elements
}

IntrusiveLinked* IntrusiveLinked::newNext(IntrusiveLinked* newbie)
{
  newbie->prev = this;
  next->prev = newbie;
  newbie->next = next;
  return next = newbie; // return the new elements
}
