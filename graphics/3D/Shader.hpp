#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#define STR_VERTEX "vert"
#define STR_PIXEL "frag"
#define STR_GEOMETRY "geom"

class Shader
{
  //! NESTING
public:
  enum Type { INVALID = 0, VERTEX, PIXEL, GEOMETRY };

  //! ATTRIBUTES
private:
  unsigned long program_id, vertex_id, pixel_id, geometry_id;

  //! METHODS
public:
  // constructors, destructors
  Shader();
  ~Shader();
  // import
  int load_from(const char* file, ...);
  // use
  void activate();
  void deactivate();

  //! SUBROUTINES
private:
  int load_from(const char* file_path);
};

#endif // ifndef SHADER_HPP_INCLUDED
