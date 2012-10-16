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

#include "Application.hpp"

#include "SDL_image.h"                  // IMG_Load

#include "global.hpp"         // needed for WINDOW_DEFAULT_W and MAX_FPS
#include "debug/assert.h"   // needed for platform specific ASSERT macros
#include "debug/warn.h"     // needed for WARN

#include "io/AudioManager.hpp"         // audio subsystem (singleton)
#include "io/GraphicsManager.hpp"      // graphics subsystem (singleton)
#include "io/FontManager.hpp"          // font subsystem (singleton)
#include "io/MeshManager.hpp"          // 3D mesh subsystem (singleton)

#define WINDOW_DEFAULT_FLAGS SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN

#ifdef __ANDROID__
	#define WINDOW_FLAGS WINDOW_DEFAULT_FLAGS|SDL_WINDOW_BORDERLESS
  #define WINDOW_C_DEPTH 32
  #define USE_TOUCH 1
  #define USE_MOUSE 0
  #define KEY_EXIT SDLK_ESCAPE
  #define KEY_BACK 1073742094   // FIXME - use SDL macro: SDL_SCANCODE_AC_BACK?
  #define KEY_MENU 1073741942   // FIXME - use SDL macro: SDL_SCANCODE_AC_HOME?
  #define KEY_VOLUME_UP 1073741952
  #define KEY_VOLUME_DOWN 1073741953
#else // LINUX, MAC, WINDOWS
	#define WINDOW_FLAGS WINDOW_DEFAULT_FLAGS
	#define WINDOW_C_DEPTH 32
  #define USE_TOUCH 0
  #define USE_MOUSE 1
	#define KEY_EXIT SDLK_ESCAPE
	#define KEY_MENU SDLK_ENTER
	#define KEY_BACK SDLK_SPACE
  #define KEY_VOLUME_UP SDLK_PLUS
	#define KEY_VOLUME_DOWN SDLK_MINUS
#endif // #ifdef __ANDROID__

/// CONSTRUCTOR & DESTRUCTOR (public)

Application::Application(Scene* first_scene) :
initialised(false),
prev_tick(0),
this_tick(0),
next_tick(0),
scene(first_scene)
{
}

Application::~Application()
{
  // Force clean if nessecary
  if(initialised)
  {
    WARN("Application::~Application","Destructor forcing shutdown sequence");
    shutdown();
  }
}

/// METHODS (public)

int Application::startup()
{
  // Already initialised!
  if(initialised)
  {
    log(LOG_WARN, "Application already loaded!");
    return EXIT_SUCCESS;
  }

  // Set up SDL (create window and context for OpenGL)
  ASSERT(startSDL() == EXIT_SUCCESS, "Starting SDL");

  // Set up OpenGL/GLES "propre"
  ASSERT(startGL() == EXIT_SUCCESS, "Starting OpenGL/GLES");

  // Start up resource subsystems
  ASSERT(GraphicsManager::getInstance()->startup()
    == EXIT_SUCCESS, "Starting Graphics Manager");
  // Start the Audio Manager
  ASSERT(AudioManager::getInstance()->startup()
    == EXIT_SUCCESS, "Starting Audio Manager");
  // Start the Font Manager
  ASSERT(FontManager::getInstance()->startup()
    == EXIT_SUCCESS, "Starting Font Manager");
  // Start the Mesh Manager
  /*ASSERT(MeshManager::getInstance()->startup()
    == EXIT_SUCCESS, "Starting Mesh Manager"); */ //! FIXME

// Load the initial scene
  ASSERT(scene->startup() == EXIT_SUCCESS, "Loading initial Scene");

  // Initialisation successful!
  initialised = true;
  return EXIT_SUCCESS;
}

// Perform a single application step, return exit flag if encountered
int Application::run()
{
  // Update current scene, switch to the next scene if
  // need be (in which case we break from the loop and restart).
  Scene* next = NULL;
  if(scene->update(&next, (this_tick - prev_tick) / 1000.0f)
      != Scene::NO_CHANGE)
    return setScene(next);

  // Redraw everything, game objects included
  draw();

  // Regulate the number of frames per second, pausing if nessecary
  wait();

  // Treat input events, check for exit conditions.
  int flag = treatEvents();
  if(flag == Application::BACK)
    // Attempt to return to previous Inteface - if impossible, return EXIT
    return setScene(scene->previous());
  else
    // Pass on either EXIT or CONTINUE code
    return flag;
}

int Application::shutdown()
{
  // Check if not initialised
  if(!initialised)
  {
    log(LOG_WARN, "Application not loaded or already shut down!");
    return EXIT_SUCCESS;
  }

  // Close down the current scene
  ASSERT(scene->shutdown() == EXIT_SUCCESS, "Application shutting down Scene");
  delete scene;

  // Shut down subsystems
  //!! FIXME
  //! MeshManager::getInstance()->shutdown();
  FontManager::getInstance()->shutdown();
  AudioManager::getInstance()->shutdown();
  GraphicsManager::getInstance()->shutdown();

  // Destory application display and control structures
#ifdef SDL2
  SDL_GL_MakeCurrent(NULL, NULL);
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
#else // SDL1.6
  SDL_FreeSurface(screen);
#endif // SDL2

	// Shut down SDL
	SDL_Quit();

	// Flag uninitialised and signal success
  initialised = false;
	return EXIT_SUCCESS;
}


/// SUB-METHODS (private)

int Application::startSDL()
{
  // Initialise SDL
	ASSERT_SDL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0,
		"Initialising SDL video and audio");

  // Initialise timing
  prev_tick = this_tick = SDL_GetTicks();

	// Open the window where we will draw. NB: Android will override the
	// specified height and width no matter what they are!
#ifdef SDL2
	window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED, WINDOW_DEFAULT_W,
                           WINDOW_DEFAULT_H, WINDOW_FLAGS);
  ASSERT_SDL(window, "Opening SDL2.0 application window");

  // Since the window size can be overriden, check what it is actually
  SDL_GetWindowSize(window, &global::viewport.x, &global::viewport.y);

  // Create the OpenGL context for the window we just opened
  context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);

  // Configure SDL/OpenGL interface
  ASSERT_SDL(SDL_GL_SetSwapInterval(1) != -1, "Activating SDL V-sync");
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_V_MAJOR);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_V_MINOR);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#else // SDL1.6

  screen = SDL_SetVideoMode(WINDOW_DEFAULT_W, WINDOW_DEFAULT_H,
                            WINDOW_C_DEPTH, SDL_OPENGL);
  ASSERT_SDL(screen, "Creating SDL1.6 application screen-surface");
  global::viewport = fV2(screen->w, screen->h);
  SDL_WM_SetCaption(APP_NAME, NULL);

#endif // SDL2

  // Calculate the scale of the window relative the the default size
  global::scale = fV2(global::viewport.x / (float)WINDOW_DEFAULT_W,
                        global::viewport.y / (float)WINDOW_DEFAULT_H);

  // No problems, return success code!
  return EXIT_SUCCESS;
}

int Application::startGL()
{
  // Black background by default
  glClearColor(0, 0, 0, 255);

  // Texturing
  glEnable(GL_TEXTURE_2D);

  // Blending and anti-aliasing
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


  // Start in 2D mode by default
  draw::use2D();

  // No problems, return success code!
  return EXIT_SUCCESS;
}

void Application::draw()
{
  // Clear the entire screen, reset everything back to normal
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //! FIXME - only clear depth in 3D mode?
  glLoadIdentity();

  // Draw any objects required by the current scene
  scene->draw();

  // Flip the buffers to update the screen
#ifdef SDL2
  SDL_GL_SwapWindow(window);
#else // SDL1.6
  glFlush();
  SDL_GL_SwapBuffers();
#endif // SDL2
}

// Regulate the number of frames per second, pausing only if need be
void Application::wait()
{
  // Get the current time-stamp
  prev_tick = this_tick;
  this_tick = SDL_GetTicks();

  // If it's not yet time for the next update, wait a while
	if (this_tick < next_tick )
		SDL_Delay(next_tick - this_tick);

  // Calculate when the next update should be
	next_tick = this_tick + (1000/MAX_FPS);
}

// deal with input messages, return BACK, EXIT or CONTINUE
int Application::treatEvents()
{
  // local variables
  static iV2 cursor(0,0);
  static bool clicking(false);

  // static to avoid reallocating it ever time we run the function
  static SDL_Event event;

  // write each event to our static variable
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      // exit if the window is closed (ex: pressing the cross)
      case SDL_QUIT:
        return Application::EXIT;
        break;

      // check for keypresses
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
          case KEY_BACK:
            return Application::BACK;
          case KEY_EXIT:
            return Application::EXIT;
          case KEY_VOLUME_UP:
            AudioManager::getInstance()->volume_up();
            break;
          case KEY_VOLUME_DOWN:
            AudioManager::getInstance()->volume_down();
            break;
          default:
            // here we DO want a default break, as not all keys are needed
            break;
        }
        break;

      #if USE_TOUCH
      // touch events
      case SDL_FINGERDOWN:
        cursor = normaliseTouch(event.tfinger.touchId,
                                   iV2(event.tfinger.x, event.tfinger.y));
        clicking = true;
        break;

      case SDL_FINGERUP:
        clicking = false;
        break;

      case SDL_FINGERMOTION:
        cursor = normaliseTouch(event.tfinger.touchId,
                                iV2(event.tfinger.x, event.tfinger.y));
        break;
      #endif  // USE_TOUCH

      #if USE_MOUSE
      // mouse events
      case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_RIGHT)
          return Application::BACK;
        clicking = true;
        break;

      case SDL_MOUSEBUTTONUP:
        if(event.button.button == SDL_BUTTON_LEFT)
          clicking = false;
        break;

      case SDL_MOUSEMOTION:
        cursor = fV2(event.motion.x, event.motion.y) / global::scale;
        break;
      #endif  // USE_MOUSE

      default:
        // not all possible inputs are needed, so we DO want a default break
        break;
    }
  }

  // Update the scene with the new input
  scene->getState()->setCursor(cursor, clicking);

  // No exit events encountered, we can continue
  return Application::CONTINUE;
}

int Application::setScene(Scene* new_scene)
{
  // Check if there is a new scene to set
  if(!new_scene || (new_scene == scene))
    return Application::EXIT;

  // The new scene exists, but can it be started?
  if(new_scene->startup() != EXIT_SUCCESS)
  {
    // damage control: stay on the same scene
    new_scene->shutdown();
    return Application::CONTINUE;
  }


  // Out with the old...
  scene->shutdown();
  delete scene;

  // ... in with the new! That is, if possible
  scene = new_scene;

  // Success!
  return Application::CONTINUE;
}

/// CLASS NAMESPACE FUNCTIONS

#ifdef SDL2 // required for SDL_TouchID
iV2 Application::normaliseTouch(SDL_TouchID device_id, iV2 touch)
{
  // There's only 1 touch device: memorise itss resolution upon initial call
  static iV2 device_resolution = iV2(SDL_GetTouch(device_id)->xres,
                                     SDL_GetTouch(device_id)->yres);

 static iV2 default_window_size = iV2(WINDOW_DEFAULT_W, WINDOW_DEFAULT_H);

  // Normalise the touch position
  return touch * default_window_size / device_resolution;
}
#endif // SDL2
