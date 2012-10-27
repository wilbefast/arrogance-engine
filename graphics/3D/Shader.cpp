#include "Shader.hpp"

#include "../../io/file.hpp"
#include "../../debug/assert.h"

#include "../../opengl.h"


using namespace std;

//! CONSTRUCTORS, DESTRUCTORS

Shader::Shader() :
program_id(0),
vertex_id(0),
pixel_id(0),
geometry_id(0)
{
}

Shader::~Shader()
{
}

//! IMPORT

// import subroutines
static Shader::Type file_type(const char* file_path)
{
  // retrieve the file extension
  string extension = io::path_to_extension(file_path);

  // return the corresponding type token
  if(!extension.compare(STR_PIXEL))
    return Shader::PIXEL;
  else if(!extension.compare(STR_VERTEX))
    return Shader::VERTEX;
  else if(!extension.compare(STR_GEOMETRY))
    return Shader::GEOMETRY;
  else
    return Shader::INVALID;
}

static int compile(GLenum shader_type, unsigned long* handle, char** source)
{
  // bind and compile the shader from the given source
  (*handle) = glCreateShader(shader_type);
	glShaderSource((*handle), 1, (const GLchar**)source, NULL);
	glCompileShader((*handle));

  // all clear
  return EXIT_SUCCESS;
}

// import function
int Shader::load(const char* file)
{
  return load_all(1, file);
}

int Shader::load_all(unsigned int n_files, ...)
{
  // load a shader from each file given in the arguments
  va_list arguments;
  va_start(arguments, n_files);
  for(size_t i = 0; i < n_files; i++)
    load_aux(va_arg(arguments, char*));
  va_end(arguments);

  // get a handle for this program
  program_id = glCreateProgram();
  // attach shaders
  if(vertex_id)
    glAttachShader(program_id, vertex_id);
  if(pixel_id)
    glAttachShader(program_id, pixel_id);
  if(geometry_id)
    glAttachShader(program_id, geometry_id);
  // link the program
	glLinkProgram(program_id);

  // all clear!
  return EXIT_SUCCESS;
}

//! USE

void Shader::activate()
{
  glUseProgram(program_id);
}

void Shader::deactivate()
{
  glUseProgram(0);
}

//! SUBROUTINES

#include <iostream> /// FIXME

int Shader::load_aux(const char* file_path)
{
  // get the type of shader based on the extension ('.vert', '.frag' or '.geom')
  Type type = file_type(file_path);
  ASSERT_AUX(type, file_path, "invalid extension in 'Shader::load_from'");

  // create an empty character buffer
  char* source = NULL;

  // read source file into buffer -- ALLOCATES MEMORY
  ASSERT_AUX(io::read_text(file_path, &source) == EXIT_SUCCESS,
         file_path, "'io::read_text' failed in 'Shader::load_from'");

  // continue based on the type of shader
  switch(type)
  {
    case VERTEX:    return compile(GL_VERTEX_SHADER, &vertex_id, &source);
    case PIXEL:     return compile(GL_FRAGMENT_SHADER, &pixel_id, &source);
    case GEOMETRY:  return compile(GL_GEOMETRY_SHADER, &geometry_id, &source);
    default:        return EXIT_FAILURE;
  }

  // free the memory allocated by io::read_text
  delete[] source;
}

