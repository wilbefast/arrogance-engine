#include <iostream>
#include <fstream>
#include <sstream>

#include "../assert.hpp"
#include "../warn.hpp"
#include "../math/V3.hpp"

#include "../graphics/Mesh3D.hpp"

#include "MeshManager.hpp"

using namespace std;

/// SINGLETON<

MeshManager* MeshManager::instance = NULL;

MeshManager* MeshManager::getInstance()
{
  if(!instance)
    instance = new MeshManager();
  return instance;
}

/// CREATION & DESTRUCTION

MeshManager::MeshManager() :
started(false),
mesh()
{
}

int MeshManager::startup()
{
  // don't start twice!
  if(started)
    WARN_RTN("MeshManager::startup","already started!", EXIT_SUCCESS);

  // load models
  /// TODO
  load_obj("assets/knight.obj");

  // All good!
  started = true;
  return EXIT_SUCCESS;
}


/* LOAD EXTERNAL RESOURCES */

// object loading subroutine
void parse_face(istringstream& s, Mesh3D& mesh)
{
  // we'll consider faces with 3 (triangle) or 4 (quad) faces
  int vertices[4] = {-1, -1, -1, -1};

  // skip past all the '/' symbols as for now we're ignoring normals
  size_t get = 0;
  for(size_t v_i = 0; v_i < 4  && s.peek() >= ' '; v_i++)
  {
    s >> vertices[v_i];
    for( ; s.peek() > ' '; s.seekg(get, ios_base::beg), get++);
  }

  // always add a first triangle
  face_t triangle(vertices[0], vertices[1], vertices[2]);
  // remember: OBJ indices start at 1 rather than 0 !
  mesh.add_face(--triangle);

  // add a second triangle only if the face was a quad
  if(vertices[3] == -1)
    return;

  // in the case of a quad, indices are arranged as follows (at least when the
  // exporter is wings3D).
  //  0--1
  //  |\ |
  //  | \|
  //  3--2
  triangle = face_t(vertices[0], vertices[2], vertices[3]);
  mesh.add_face(--triangle);
}

// main object loading method
int MeshManager::load_obj(const char* filename)
{
  // open the file
  ifstream in(filename, ios::in);
  ASSERT(in, "'MeshManager::load_obj' opening object file");

  // read each line
  string line;
  while (getline(in, line))
  {
    // lop off the first two characters, used to determine how we read the line
    if(line.size() < 2)
      continue;
    string key(line.substr(0, 2));
    istringstream s(line.substr(2));

    // vertex
    if(key == "v ")
      mesh.add_vertex(vertex_t(s));
    // face
    else if(key == "f ")
      parse_face(s, mesh);
    // vertex normal
    else if(key == "vn")
      mesh.add_normal(normal_t(s));
    // texture coordinates
    else if(key == "vt")
      mesh.add_texture_coordinate(vertex_t(s));
  }

  // optimise (shrink-wrap), translate and normalise the resulting mesh
  mesh.finalise();

  // all clear!
  return EXIT_SUCCESS;
}




/*

{
  numfaces++;
  if( numfaces > faceuse.size() )
  {
    faceuse.resize( faceuse.size() * 2 );
    facenormals.resize( faceuse.size() * 2 );
    facevertexnormals.resize( faceuse.size() * 2 );
    if( usetexture )
      facevertextextcoords.resize( faceuse.size() * 2 );
  }

  getline( file, buffer );

  istringstream bufferinput( buffer );

  while( bufferinput >> part ) {

    // Replace double slash so we know there is no texture vertex
    slash = (int)part.find("//");
    if( slash != string::npos )
      part.replace( slash, 2, " 0 " );
    else {
      // With a double slash, there won't be any others

      // Replace any remaining slashes with spaces
      slash = (int)part.find("/");
      while( slash != string::npos ) {
        part.replace( slash, 1, " " );
        slash = (int)part.find("/");
      }
    }

    // Now, we can get the values
    istringstream partinput( part );

    vector< int > partdata( 3, 0 );

    for( int i = 0; i < 3 && partinput >> temp; i++ )
      partdata.at( i ) = temp;

    // Deal with negative references
    if( partdata.at( 0 ) < 0 )
      partdata.at( 0 ) += numvertices + 1;

    if( partdata.at( 0 ) > (int)numvertices )
      // Tried to access an illegal vertex
      return false;

    x = vertices.at( partdata.at( 0 ) - 1 ).x;
    y = vertices.at( partdata.at( 0 ) - 1 ).y;
    z = vertices.at( partdata.at( 0 ) - 1 ).z;
    // Check for the min and max coord to do a bounding box
    if( x > max.x )
      max.x = x;
    if( y > max.y )
      max.y = y;
    if( z > max.z )
      max.z = z;

    if( x < min.x )
      min.x = x;
    if( y < min.y )
      min.y = y;
    if( z < min.z )
      min.z = z;

    vertexuse.at( partdata.at( 0 ) - 1 ).push_back( numfaces - 1 );
    faceuse.at( numfaces - 1 ).push_back( partdata.at( 0 ) - 1);

    // Texture coord was specified
    if( partdata.at( 1 ) != 0 && usetexture ) {
      // Deal with negative references
      if( partdata.at( 1 ) < 0 )
        partdata.at( 1 ) += numtextcoords + 1;

      if( partdata.at( 1 ) > (int)numtextcoords )
        // Illegal index
        return false;

      facevertextextcoords.at( numfaces - 1 ).push_back(
        textcoords.at( partdata.at( 1 ) - 1 ) );
    }

    // Normal was specified
    if( partdata.at( 2 ) != 0 ) {
      // Deal with negative references
      if( partdata.at( 2 ) < 0 )
        partdata.at( 2 ) += numnormals + 1;

      if( partdata.at( 2 ) > (int)numnormals )
        // Tried to access an illegal normal
        return false;

      facevertexnormals.at( numfaces - 1 ).push_back(
          normals.at( partdata.at( 2 ) - 1 ) );
    }

    // Currently only allow triangles
//				if( faceuse.at( numfaces - 1 ).size() > 3 )
//					return false;

    if( faceuse.at( numfaces - 1 ).size() >= 3 ) {
      // Calculate face normal
      Vector v1, v2;
      Point p1, p2, p3;

      p1 = vertices.at( faceuse.at( numfaces - 1 ).at( 0 ) );
      p2 = vertices.at( faceuse.at( numfaces - 1 ).at( 1 ) );
      p3 = vertices.at( faceuse.at( numfaces - 1 ).at( 2 ) );

      v1.x =  p1.x - p2.x;
      v1.y =  p1.y - p2.y;
      v1.z =  p1.z - p2.z;

      v2.x =  p3.x - p2.x;
      v2.y =  p3.y - p2.y;
      v2.z =  p3.z - p2.z;

      facenormals.at( numfaces -1 ) = v2.cross( v1 );
    }
  }
}
*/
