#include "GameState.hpp"

#include "../warn.hpp"
#include "../assert.hpp"            // for platform specific ASSERT macros
#include "../global.hpp"

#include "../resources/numerise.hpp"
#include "../resources/file.hpp"              // for loading assets

#include "../platform.hpp"             // for ASSET_PATH
//#include "../tinyxml/tinyxml.h"      // for loading game objects

#include "../gameplay/things/events/CollisionEvent.hpp" // for generating events
#include "../gameplay/things/events/BoundaryEvent.hpp"  // for generating events

/// CONSTRUCTORS, DESTRUCTORS

GameState::GameState() :
things(),
level_bounds(global::viewport)
{

}

GameState::~GameState()
{
}


/// ACCESSORS

int GameState::countThings(const char* name)
{
    if(name)
    {
        // count the Things of a given type
        str_id type = numerise(name);
        int result = 0;
        for(ThingIter i = things.begin(); i!= things.end(); i++)
            if (!(*i)->isDead() && (*i)->getType() == type)
                result++;

        return result;
    }
    else
        // return the total number of things
        return things.size();
}

void GameState::killThings(const char* name)
{
    if(name)
    {
        // kill Things of a given type
        str_id type = numerise(name);
        for(ThingIter i = things.begin(); i!= things.end(); i++)
            if (!(*i)->isDead() && (*i)->getType() == type)
                (*i)->die();
    }
    else
    {
        // kill all Things of a given type
        for(ThingIter i = things.begin(); i!= things.end(); i++)
            if(!(*i)->isDead())
                (*i)->die();
    }
}

void GameState::addThing(Thing* t)
{
    things.push_back(t);
}

void GameState::deleteThing(ThingIter* i)
{
    // Copy then advance the original iterator (to safety)
    ThingIter j = (*i);
    (*i)++;
    // Erase and remove the cell it was originally pointing to
    delete (*j);
    things.erase(j);
}

Thing* GameState::getHero()
{
    // Hero is always the first Thing in the list
    return (*(things.begin()));
}

/// OVERRIDES SCENE STATE

int GameState::startup()
{
    // Set the position of the mouse to the centre of the screen
    input.last_touch = global::viewport.getSize()/2;

    // All clear!
    return EXIT_SUCCESS;
}

int GameState::shutdown()
{
    // Destroy all game objects
    for(ThingIter i = things.begin(); i!= things.end(); i++)
        delete (*i);

    // All clear!
    return EXIT_SUCCESS;
}

int GameState::update()
{
  // For each game object
  for(ThingIter i = things.begin(); i!= things.end(); )
  {
      // Update the object
      int update_result = (*i)->update(this);

      // Delete the object if nessecary
      switch(update_result)
      {
          case DELETE_ME:
              deleteThing(&i);
              continue;

          case LOSE_LEVEL:
              return EXIT_FAILURE;

          case WIN_LEVEL:
              return EXIT_FAILURE;
      }

      /// Check for collisions between this object and subsequent ones
      ThingIter j = i;
      // Unfortunately we can't write "j = i+1"
      for(j++; j!= things.end(); j++)
          CollisionEvent::generate((*i), (*j));

      /// Generate out of or intersect bounds events for the current object
      BoundaryEvent::generate((*i), &level_bounds);

      // manual iteration
      i++;
  }

  // All clear
  return EXIT_SUCCESS;
}

void GameState::draw()
{
  // Draw all the game objects
  for(ThingIter i = things.begin(); i!= things.end(); i++)
      (*i)->draw();
}
