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

#ifndef GAMESTATE_HPP_INCLUDED
#define GAMESTATE_HPP_INCLUDED

#include "SceneState.hpp"

class GameState;

#include "../gameplay/things/Thing.hpp"

class GameState : public SceneState
{
    /// NESTING
public:
    // FIXME -- add an "Updatable" interface class
    enum update_result
    {
        CONTINUE,
        LOSE_LEVEL,
        WIN_LEVEL,
        DELETE_ME
    };

    /// ATTRIBUTES
private:
    // Game objects
    ThingList things;
    // Level boundary
    fRect level_bounds;

    /// METHODS
public:
    // Constructors, destructors
    GameState();
    ~GameState();
    // Accessors
    int countThings(const char* name = NULL);
    void killThings(const char* name = NULL);
    void addThing(Thing*);
    void deleteThing(ThingIter* i);
    Thing* getHero();
    // Overrides SceneState
    virtual int startup();
    virtual int shutdown();
    virtual int update();
    virtual void draw();
};

#endif // GAMESTATE_HPP_INCLUDED
