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
  int load(const char* file);
  int load_all(unsigned int n_files, ...);
  // use
  void activate();
  void deactivate();
  // send uniform values
  void parameter(const char* name, float value);

  //! SUBROUTINES
private:
  int load_aux(const char* file_path);
};

#endif // ifndef SHADER_HPP_INCLUDED
