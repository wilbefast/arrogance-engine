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

#include <iostream>
#include "../platform.hpp"

#include "Joint.hpp"

#include "draw.hpp"
#include "../debug/warn.hpp"

using namespace std;

/// CREATION, DESTRUCTION

Joint::Joint(Joint* _father, vertex_t _position) :
father(_father),
child(NULL),
sibling(NULL),
position(_position)
{
  if(father)
    father->addChild(this);
}

Joint::~Joint()
{
}

/// DRAW

void Joint::print()
{
  cout << "Joint @" << this << endl
       << "position " << position << endl
       << "\tfather @" << father << endl
       << "\tsibling @" << sibling << endl;
}

void Joint::draw()
{
  // draw self
  draw::line(father->position, position);

  // draw children
  if(!child)
    return;
  Joint* current_child = child;
  do
  {
    current_child->draw();
    current_child = current_child->sibling;
  }
  while(current_child);
}

/// MODIFY ARMATURE

Joint* Joint::addChild(Joint* new_child)
{
  // no child
  if(!child)
  {
    child = new_child;
    // report that the child was added as the eldest sibling of the list
    return this;
  }

  // find the first child with no sibling (the youngest child)
  Joint* current_child = child;
  while(current_child->sibling)
    current_child = current_child->sibling;

  // the new bone is now the younger brother of the (previously) youngest
  current_child->sibling = new_child;

  // return the child immediately older than the new one added
  return current_child;
}

Joint* Joint::removeChild(Joint* remove)
{
  // no child => error
  if(!child)
    WARN_RTN("Joint::removeChild failed", "this joint has no children!", NULL);

  // child to remove is the eldest
  if(child == remove)
    // the second-eldest is now the eldest
    child = child->sibling;

  // find the first child with no sibling (the youngest child)
  Joint* current_child = child;
  while(current_child->sibling && current_child->sibling != remove)
    current_child = current_child->sibling;

  // reached the end of the sibling list => error
  if(!current_child->sibling)
    WARN_RTN("Joint::removeChild failed", "joint to remove not found!", NULL)
  else
    current_child->sibling = current_child->sibling->sibling;

  // the joint remove loses its father and its brothers
  remove->father = remove->sibling = NULL;

  // return the child immediately older than the old one removed
  return current_child;
}

void Joint::setPosition(vertex_t new_position)
{
  translate(new_position - position);
}

void Joint::translate(vertex_t amount)
{
  // move self
  position += amount;

  // move children
  if(!child)
    return;
  Joint* current_child = child;
  do
  {
    current_child->translate(amount);
    current_child = current_child->sibling;
  }
  while(current_child);
}
