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
#include <time.h>

#include "debug/assert.h"
#include "Application.hpp"
#include "scenes/MainMenu.hpp"
#include "scenes/Game.hpp"  /// FIXME
#include "tp2/math_test.hpp" /// FIXME

#include <iostream> /// FIXME

using namespace std;

int main(int argc, char* argv[])
{
  // Initialise random numbers
  srand(time(NULL));

  math_test();

  // Load up the application
  Application application(new Game());
  ASSERT_SDL(application.startup() == EXIT_SUCCESS, "Starting up application");

  // Run the application until an exit condition is encountered
  bool stop = false;
  do
    stop = application.run();
  while(!stop);

  // Shut down the application completely
	ASSERT_SDL(application.shutdown() == EXIT_SUCCESS, "Shutting down application");

  // Report a clean exit!
	return EXIT_SUCCESS;
}
