#include "Shader.hpp"

#include "../../io/file.hpp"
#include "../../debug/assert.h"

#include "../../opengl.h"



#include <iostream>

using namespace std;

///------------------------------------------- FIXME
void printLog(GLuint obj)
{
	int infologLength = 0;
	int maxLength;

	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

	char infoLog[maxLength];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0)
		printf("%s\n",infoLog);
}
///-------------------------------------------


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
  else if(!extension.compare(STR_PIXEL))
    return Shader::VERTEX;
  else if(!extension.compare(STR_PIXEL))
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
int Shader::load_from(const char* file, ...)
{
  // all clear!
  return EXIT_SUCCESS;

  // get a handle for this program
  program_id = glCreateProgram();
  // attach shaders
  if(vertex_id)    { glAttachShader(program_id, vertex_id); printLog(vertex_id); } /// FIXME
  if(pixel_id)     { glAttachShader(program_id, pixel_id); printLog(pixel_id); } /// FIXME
  if(geometry_id)  { glAttachShader(program_id, geometry_id); printLog(geometry_id); } /// FIXME
  // link the program
	glLinkProgram(program_id);
	printLog(program_id); /// FIXME
}

//! USE

void Shader::activate()
{
  glUseProgram(program_id);
}

void deactivate()
{
  glUseProgram(0);
}

//! SUBROUTINES

int Shader::load_from(const char* file_path)
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

