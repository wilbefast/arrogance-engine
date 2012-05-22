#ifndef MESHMANAGER_HPP_INCLUDED
#define MESHMANAGER_HPP_INCLUDED

#include "../graphics/Mesh3D.hpp"

class MeshManager
{
  /// CONSTANTS
private:

  /// SINGLETON
private:
  static MeshManager* instance;
public:
  static MeshManager* getInstance();

  /// ATTRIBUTES
private:
  bool started;
public: /// FIXME
  Mesh3D mesh;

  /// METHODS
public:
  // creation, destruction
  MeshManager();
  int startup();
  int shutdown();
  // load external resources
  int load_obj(const char* filename);
};

#endif // MESHMANAGER_HPP_INCLUDED
