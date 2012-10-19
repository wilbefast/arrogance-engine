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
#include <fstream>

#include "Mesh3D.hpp"

#include "../debug/assert.h"            // for ASSERT macro
#include "../io/file.hpp"               // for ASSET_PATH
#include "../math/V4.hpp"               // used to store quads temporarily

#define TAG_VERTEX "v "
#define TAG_FACE "f "
#define TAG_GROUP "g "
#define TAG_NORMAL "vn"
#define TAG_TEXTURE_COORDINATES "vt"
#define TAG_MATERIAL_LIBRARY "mt"
  #define STR_MATERIAL_LIBRARY "mtllib"
#define TAG_USE_MATERIAL "us"
  #define STR_USE_MATERIAL "usemtl"

using namespace std;

//! PRIVATE FUNCTIONS

static inline bool seperatorSymbol(char c)
{
  return (c == ' ' || c == '\t' || c == '/');
}

static inline bool numberSymbol(char c)
{
  return (c >= '0' && c <= '9');
}

// in the case of a quad, indices are arranged as follows (at least when the
// exporter is wings3D).
//  0--1
//  |\ |
//  | \|
//  3--2

// triangle (0,1,2)
template <typename T>
static inline V3<T> toFirstV3(V4<T> v)
{
  return V3<T>(v.x, v.y, v.z);
}

//triangle (0,2,3)
template <typename T>
static inline V3<T> toSecondV3(V4<T> v)
{
  return V3<T>(v.x, v.z, v.w);
}

//! CREATION, DESTRUCTION

Mesh3D::Mesh3D() :
vertices(),
faces(),
normals(),
texture_coordinates(),
min(),
max(),
first_group(new Group()),
current_group(first_group)
{
}

int Mesh3D::load_obj(const char* filename)
{
  // open the file
  ifstream in(filename, ios::in);
  ASSERT(in, "'Mesh3D::load_obj' opening Wavefront object file");

  // read each line
  string line;
  while (getline(in, line))
  {
    // lop off the first two characters, used to determine how we read the line
    if(line.size() < 2 || line[0] == '#')
      continue;
    string key(line.substr(0, 2));
    istringstream s(line.substr(2));

    // vertex
    if(key == TAG_VERTEX)
      add_vertex(vertex_t(s));
    // vertex normal
    else if(key == TAG_NORMAL)
      normals.push_back(normal_t(s));
    // texture coordinates
    else if(key == TAG_TEXTURE_COORDINATES)
      add_texture_coordinate(tex_coord_t(s));
    // face
    else if(key == TAG_FACE)
      parse_faces(s);
    // group
    else if (key == TAG_GROUP)
    {
      if(faces.size()) // never close empty groups
      {
        // close current group
        current_group->last_face = faces.size()-1;
        // start new group
        current_group = (Group*)current_group->newNext(new Group());
        current_group->first_face = faces.size();
      }

    }
    // material library
    else if(key == TAG_MATERIAL_LIBRARY &&
            !line.substr(0, 6).compare(STR_MATERIAL_LIBRARY))
    {
      // parse the material filename
      string mtl_file = line.substr(7).insert(0, ASSET_PATH);
      mtl_file = mtl_file.substr(0, mtl_file.find_first_of('\r'));
      // read the material file
      ASSERT(current_group->material.load_mtl(mtl_file.c_str()) == EXIT_SUCCESS,
        "'Mesh3D::load_obj' loading associated material file");
    }

    // use material from library
    else if (key == TAG_USE_MATERIAL &&
             !line.substr(0, 6).compare(STR_USE_MATERIAL))
    {
      cout << "use material!\n";
    }
    else
      cout << line << ": unrecognised\n";
  }
  current_group->last_face = faces.size()-1;

  // file is no longer needed
  in.close();

  // optimise (shrink-wrap) the resulting mesh, connect group list, etc
  finalise();

  // all clear!
  return EXIT_SUCCESS;
}

Mesh3D::~Mesh3D()
{
  current_group->deleteConnections();
  delete current_group;
}


/* BUILD ITERATIVELY */

void Mesh3D::add_vertex(vertex_t new_vertex)
{
  // reset minimum and maximum coordinates of this mesh
  if(vertices.size() == 0)
    min = max = new_vertex;
  else for(size_t i = 0; i < 3; i++)
  {
    if(new_vertex[i] > max[i])
      max[i] = new_vertex[i];
    else if(new_vertex[i] < min[i])
      min[i] = new_vertex[i];
  }

  vertices.push_back(new_vertex);
}

void Mesh3D::add_texture_coordinate(tex_coord_t new_tx_c)
{
  texture_coordinates.push_back(tex_coord_t(new_tx_c.x, 1.0f - new_tx_c.y));
}

void Mesh3D::parse_faces(istringstream& s)
{
  // the position in the stream
  size_t str_i = 0;

  // we'll consider only faces with 3 (triangle) or 4 (quad) faces
  iV4 vert_indices(-1, -1, -1, -1),
      uv_indices(-1, -1, -1, -1),
      norm_indices(-1, -1, -1, -1);

  /*! the OBJ format has defines faces:
        f A B C D ...
      A, B, C, D and so on are triplets of values seperated by slashes:
        A = Vi/UVi/Ni
      The texture coordinate index (UVi) and normal index (Ni) are optional,
      so we might have something of the form:
        A = Vi//
      */

  size_t vtx_i;
  for(vtx_i = 0; vtx_i < 4  && s.peek() >= ' '; vtx_i++)
  {
    // 1. read the vertex index
    s >> vert_indices[vtx_i];

    // 2. read the (optional) texture coordinate index
    do { s.seekg(str_i++, ios_base::beg); } while(numberSymbol(s.peek()));
    do { s.seekg(str_i++, ios_base::beg); } while(seperatorSymbol(s.peek()));
    s >> uv_indices[vtx_i];

    // 3. read the (optional) normal index
    do { s.seekg(str_i++, ios_base::beg); } while(numberSymbol(s.peek()));
    do { s.seekg(str_i++, ios_base::beg); } while(seperatorSymbol(s.peek()));
    s >> norm_indices[vtx_i];

    // 4. move on to the next block
    do { s.seekg(str_i++, ios_base::beg); } while(numberSymbol(s.peek()));
    do { s.seekg(str_i++, ios_base::beg); } while(seperatorSymbol(s.peek()));
  }

  // always add a first triangle
  face_t triangle(toFirstV3(vert_indices),
                  toFirstV3(uv_indices),
                  toFirstV3(norm_indices));
  // remember: OBJ indices start at 1 rather than 0 !
  faces.push_back(--triangle);

  // add a second triangle only if the face was a quad
  if(vert_indices[3] == -1)
    return;

  triangle = face_t(toSecondV3(vert_indices),
                    toSecondV3(uv_indices),
                    toSecondV3(norm_indices));
  // remember: OBJ indices start at 1 rather than 0 !
  faces.push_back(--triangle);
}

/* FINISHED BUILDING */

void Mesh3D::finalise()
{
  // shrink-wrap the containers by copying elements into a new vector which
  // fits them all exactly (capacity = size)
  vertex_list_t(vertices).swap(vertices);
  face_list_t(faces).swap(faces);
  normal_list_t(normals).swap(normals);
  tex_coord_list_t(texture_coordinates).swap(texture_coordinates);

  // Move group pointer back to the first group
  current_group = first_group;

  ofstream f;
  f.open ("arrogance_mesh.obj");
  f << (*this);
}

void Mesh3D::unitise()
{
  // maximum size of any dimension should be 1
  vertex_t size = max - min;
  float inv_max_side = 1.0f / (MAX(MAX(size.x, size.y), size.z));

  // center around the origin (0, 0, 0) and normalise
  vertex_t mid = (max + min) / 2.0f;
  for(vertex_list_it i = vertices.begin(); i != vertices.end(); i++)
  {
    (*i) -= mid;
    (*i) *= inv_max_side;
  }
  max = (max - mid)*inv_max_side;
  min = (min - mid)*inv_max_side;

}

//! DRAW


void Mesh3D::draw()
{
	// for each object
  do
	{
		// activate the current group's material
    //current_group->material.activate();

		// draw the group's faces
		glBegin(GL_TRIANGLES);
			// for each triangle in this object
			for(size_t face_i = current_group->first_face;
          face_i <= current_group->last_face; face_i++)
			{
				// cache the current triangle
				face_t const& face = faces[face_i];

				// first vertex
				glNormal3fv(normals[face.normal_i[0]].front());
				//glTexCoord2fv(&(tc[0]->fU));
				glVertex3fv(vertices[face.vertex_i[0]].front());

				// second vertex
				glNormal3fv(normals[face.normal_i[1]].front());
				//glTexCoord2fv(&(tc[0]->fU));
				glVertex3fv(vertices[face.vertex_i[1]].front());

				// third vertex
				glNormal3fv(normals[face.normal_i[2]].front());
				//glTexCoord2fv(&(tc[0]->fU));
				glVertex3fv(vertices[face.vertex_i[2]].front());
			}
			// finished drawing the triangles
		glEnd();

		// deactivate texture
		//if(image)
			//glBindTexture(GL_TEXTURE_2D, 0);

    // move on to next group
    current_group = (Group*)current_group->getNext();
	}
	while(current_group != first_group);

}


/*
{
  // Material
  material.activate();
  // Vertices
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices.front());
  // Triangles
  glEnableClientState(GL_INDEX_ARRAY);
  glIndexPointer(GL_UNSIGNED_BYTE, 0, &faces.front());
  // Normals
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 0, &normals.front());

  // Draw the triangles using the specified normals
  glDrawElements(GL_TRIANGLES, 3*faces.size(), GL_UNSIGNED_BYTE, &faces.front());

  // Shut down
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  //material.deactivate();
  glLoadIdentity();
}*/


//! FOR DEBUGGING

void Mesh3D::print(ostream& out) const
{
  // print all the vertices
  for(size_t v_i = 0; v_i < vertices.size(); v_i++)
    out << TAG_VERTEX << vertices[v_i] << endl;

  // print all the texture (UV) coordinates
  //! TODO

  // print all the normals
  for(size_t n_i = 0; n_i < normals.size(); n_i++)
    out << TAG_NORMAL << ' ' << normals[n_i] << endl;

  // print each group
  Group* group_i = first_group;
  do
  {
    // start group
    out << TAG_GROUP << endl;

    // print the group's faces
    for(size_t face_i = group_i->first_face;
    face_i <= group_i->last_face; face_i++)
    {
      out << TAG_FACE;
      for(size_t v_i = 0; v_i < 3; v_i++)
       out << faces[face_i].vertex_i[v_i]+1 << '/'
            << faces[face_i].uv_i[v_i]+1 << '/'
            << faces[face_i].normal_i[v_i]+1 << ' ';
        out << endl;
    }

    // move on to next group
    group_i = (Group*)group_i->getNext();
	}
	while(group_i != first_group);
}

std::ostream& operator<<(std::ostream& stream, Mesh3D const& m)
{
  m.print(stream);
  return stream;
}
