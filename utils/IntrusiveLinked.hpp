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

#ifndef INTRUSIVELINKED_HPP_INCLUDED
#define INTRUSIVELINKED_HPP_INCLUDED

class IntrusiveLinked
{
  //! ATTRIBUTES
public: /// FIXME
  IntrusiveLinked *next, *prev;

  //! METHODS
public:
  // constructors, destructors
  IntrusiveLinked();
  ~IntrusiveLinked();
  void deleteConnections();

  // navigation
  IntrusiveLinked* getNext() const;
  IntrusiveLinked* getPrev() const;

  // add a new link
  IntrusiveLinked* newPrev(IntrusiveLinked* newbie);
  IntrusiveLinked* newNext(IntrusiveLinked* newbie);
};

#endif // INTRUSIVELINKED_HPP_INCLUDED
