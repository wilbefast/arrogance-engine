#include "TP3State.hpp"

#include "../debug/assert.h"
#include "../global.hpp"
#include "../io/MeshManager.hpp"

#include "../graphics/Mesh3D.hpp"

#include "OBJLoader.hpp"
#include "../debug/log.h"

using namespace std;


/// CREATION, DESTRUCTION

TP3State::TP3State() :
GameState(),
camera_angle(),
camera_offset()
{
}

#include <iostream> /// FIXME
int TP3State::startup()
{
  // basic startup
  ASSERT(GameState::startup() == EXIT_SUCCESS,
        "TP3State starting GameState");

  Mesh3D& m = MeshManager::getInstance()->mesh;

  SCENE* scene = ReadOBJFile("assets/Island_001.obj");
  log(LOG_INFO, "scene = %p", scene);
  log(LOG_INFO, "\t objects = %d", scene->u32ObjectsCount); // 50
  log(LOG_INFO, "\t vertices = %d", scene->u32VerticesCount); // 86284
  log(LOG_INFO, "\t normals = %d", scene->u32NormalsCount); // 86268
  log(LOG_INFO, "\t texture coordinates = %d", scene->u32UVCount); // 94068
  log(LOG_INFO, "\t faces = %d", scene->u32FacesCount); // 59393
  log(LOG_INFO, "\t materials = %d", scene->u32MaterialsCount); //34

  //! CHECK UV COORDINATES
  /*puts("CHECKING UV COORDINATES");
  for(unsigned int i = 0; i < scene->u32UVCount; i++)
  {
    m.faces[i]
    fV3 my_v = m.vertices[i],
        other_v = fV3(scene->pVertices[i].fX,
                      scene->pVertices[i].fY, scene->pVertices[i].fZ);
    if(my_v != other_v)
      puts("UV problem!");
  }
  puts("DONE CHECKING UV COORDINATES");*/

  // Occlusion on
  //glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glDisable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
  // Lighting on
  //glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // all clear
  return EXIT_SUCCESS;
}

int TP3State::shutdown()
{
  // basic shutdown
  ASSERT(GameState::shutdown() == EXIT_SUCCESS,
        "TP3State stopping GameState");

  // Occlusion off
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  // Lighting off
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  // all clear
  return EXIT_SUCCESS;
}

/// OVERRIDES GAMESTATE

int TP3State::update(float delta)
{
  // Update camera angle and position
  uV2 const& p = input.last_hover;
  camera_angle.x = CAMERA_MAX_ANGLE*p.x/global::viewport.x*2 - CAMERA_MAX_ANGLE;
	camera_angle.y = CAMERA_MAX_ANGLE*p.y/global::viewport.y*2 - CAMERA_MAX_ANGLE;

  // Update dynamic game objects
  int result = GameState::update(delta);
  if(result != CONTINUE)
    return result;

  // All clear
  return CONTINUE;
}

void TP3State::draw()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glTranslatef(global::viewport.x/2, global::viewport.y/2, 0.0f);
  glScalef(global::viewport.x/2, global::viewport.y/2, 1.0f);

  glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
  glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);
	glTranslatef(camera_offset.x, camera_offset.y, camera_offset.z);
	glRotatef(camera_angle.x, 0.0f, 1.0f, 0.0f);
	glRotatef(-camera_angle.y, 0.0f, 0.0f, 1.0f);

  MeshManager::getInstance()->mesh.draw();

  // Draw dynamic game objects
  GameState::draw();
}
