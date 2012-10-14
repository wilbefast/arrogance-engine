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

//! CONSTRUCTORS, DESTRUCTORS

template<T>
IntrusiveLinked<T>::IntrusiveLinked() :
next(NULL),
prev(NULL)
{
}

template<T> IntrusiveLinked<T>::~IntrusiveLinked()
{
  if(next)
    next->prev = prev;
  if(prev)
    prev->next = next;
}

//! ADD A NEW LINK

template<T> void IntrusiveLinked<T>::linkBefore(IntrusiveLinked* newbie)
{
  prev->next = newbie;
  newbie->next = this;
  newbie->prev = prev;
  prev = newbie;
}

template<T> void IntrusiveLinked<T>::linkAfter(IntrusiveLinked* newbie)
{
  next->prev = newbie;
  newbie->next = next;
  newbie->prev = this;
  next = newbie;
}

//! VISITOR

template<T> void IntrusiveLinked<T>::receiveVisitor(Visitor* v)
{
  IntrusiveLinked* current = this;
  while (current != NULL)
  {
    v->visit(current);
    current = current->next;
  }
}
