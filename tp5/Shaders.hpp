#ifndef SHADERS_HPP_INCLUDED
#define SHADERS_HPP_INCLUDED

#include <GL/gl.h>
#include "./GLExt.h"

#define STDCALL
typedef GLuint (STDCALL *fnglCreateProgramObjectARB)(GLvoid);
typedef GLuint (STDCALL *fnglCreateShaderObjectARB)(GLenum);
typedef void (STDCALL *fnglDeleteObjectARB)(GLuint);
typedef void (STDCALL *fnglDetachObjectARB)(GLuint, GLuint);
typedef void (STDCALL *fnglAttachObjectARB)(GLuint, GLuint);
typedef void (STDCALL *fnglShaderSourceARB)(GLuint, GLsizei, const GLchar**, const GLint*);
typedef void (STDCALL *fnglCompileShaderARB)(GLuint);
typedef void (STDCALL *fnglLinkProgramARB)(GLuint);
typedef void (STDCALL *fnglGetInfoLogARB)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (STDCALL *fnglUseProgramObjectARB)(GLuint);
typedef void (STDCALL *fnglGetProgramivARB)(GLuint, GLenum, GLint*);
typedef void (STDCALL *fnglGetObjectParameterivARB)(GLuint, GLenum, GLint*);
typedef void (STDCALL *fnglGetObjectParameterfvARB)(GLuint, GLenum, GLfloat*);
typedef GLint (STDCALL *fnglGetUniformLocationARB)(GLuint, const GLchar*);
typedef GLint (STDCALL *fnglGetAttribLocationARB)(GLuint, const GLchar*);
typedef void (STDCALL *fnglBindAttribLocationARB)(GLuint, GLuint, const GLchar*);
typedef void (STDCALL *fnglUniform1fARB)(GLint, GLfloat);
typedef void (STDCALL *fnglUniform2fARB)(GLint, GLfloat, GLfloat);
typedef void (STDCALL *fnglUniform3fARB)(GLint, GLfloat, GLfloat, GLfloat);
typedef void (STDCALL *fnglUniform4fARB)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (STDCALL *fnglUniform1iARB)(GLint, GLint);
typedef void (STDCALL *fnglUniform2iARB)(GLint, GLint, GLint);
typedef void (STDCALL *fnglUniform3iARB)(GLint, GLint, GLint, GLint);
typedef void (STDCALL *fnglUniform4iARB)(GLint, GLint, GLint, GLint, GLint);
typedef void (STDCALL *fnglUniform1fvARB)(GLint, GLsizei, const GLfloat*);
typedef void (STDCALL *fnglUniform2fvARB)(GLint, GLsizei, const GLfloat*);
typedef void (STDCALL *fnglUniform3fvARB)(GLint, GLsizei, const GLfloat*);
typedef void (STDCALL *fnglUniform4fvARB)(GLint, GLsizei, const GLfloat*);
typedef void (STDCALL *fnglUniform1ivARB)(GLint, GLsizei, const GLint*);
typedef void (STDCALL *fnglUniform2ivARB)(GLint, GLsizei, const GLint*);
typedef void (STDCALL *fnglUniform3ivARB)(GLint, GLsizei, const GLint*);
typedef void (STDCALL *fnglUniform4ivARB)(GLint, GLsizei, const GLint*);
typedef void (STDCALL *fnglVertexAttrib1fvARB)(GLuint, const GLfloat*);
typedef void (STDCALL *fnglVertexAttrib2fvARB)(GLuint, const GLfloat*);
typedef void (STDCALL *fnglVertexAttrib3fvARB)(GLuint, const GLfloat*);
typedef void (STDCALL *fnglVertexAttrib4fvARB)(GLuint, const GLfloat*);
typedef void (STDCALL *fnglVertexAttrib1fARB)(GLuint, GLfloat);
typedef void (STDCALL *fnglVertexAttrib2fARB)(GLuint, GLfloat, GLfloat);
typedef void (STDCALL *fnglVertexAttrib3fARB)(GLuint, GLfloat, GLfloat, GLfloat);
typedef void (STDCALL *fnglVertexAttrib4fARB)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (STDCALL *fnglUniformMatrix4fvARB)(GLint, GLsizei, GLboolean, const GLfloat*);


class Shaders
{
  //! ATTRIBUT
private:


  //! METHODS
public:
  Shaders();
  ~Shaders();
  int startup();
  int shutdown();

  //! FUNCTION POINTERS
public:
  // manage shader objects (programs)
  fnglCreateProgramObjectARB glCreateProgramObjectARB;
  fnglCreateShaderObjectARB glCreateShaderObjectARB;
  fnglDeleteObjectARB glDeleteObjectARB;
  fnglDetachObjectARB glDetachObjectARB;
  fnglAttachObjectARB glAttachObjectARB;
  // manage source files and compilation
  fnglShaderSourceARB glShaderSourceARB;
  fnglCompileShaderARB glCompileShaderARB;
  // link and use the program
  fnglLinkProgramARB glLinkProgramARB;
  fnglGetInfoLogARB glGetInfoLogARB;
  fnglUseProgramObjectARB glUseProgramObjectARB;
  fnglGetProgramivARB glGetProgramivARB;

  fnglGetObjectParameterivARB glGetObjectParameterivARB;
  fnglGetObjectParameterfvARB glGetObjectParameterfvARB;
  fnglGetUniformLocationARB glGetUniformLocationARB;
  fnglGetAttribLocationARB glGetAttribLocationARB;
  fnglBindAttribLocationARB glBindAttribLocationARB;

  fnglUniform1fARB glUniform1fARB;
  fnglUniform2fARB glUniform2fARB;
  fnglUniform3fARB glUniform3fARB;
  fnglUniform4fARB glUniform4fARB;

  fnglUniform1iARB glUniform1iARB;
  fnglUniform2iARB glUniform2iARB;
  fnglUniform3iARB glUniform3iARB;
  fnglUniform4iARB glUniform4iARB;

  fnglUniform1fvARB glUniform1fvARB;
  fnglUniform2fvARB glUniform2fvARB;
  fnglUniform3fvARB glUniform3fvARB;
  fnglUniform4fvARB glUniform4fvARB;

  fnglUniform1ivARB glUniform1ivARB;
  fnglUniform2ivARB glUniform2ivARB;
  fnglUniform3ivARB glUniform3ivARB;
  fnglUniform4ivARB glUniform4ivARB;

  fnglVertexAttrib1fvARB glVertexAttrib1fvARB;
  fnglVertexAttrib2fvARB glVertexAttrib2fvARB;
  fnglVertexAttrib3fvARB glVertexAttrib3fvARB;
  fnglVertexAttrib4fvARB glVertexAttrib4fvARB;

  fnglVertexAttrib1fARB glVertexAttrib1fARB;
  fnglVertexAttrib2fARB glVertexAttrib2fARB;
  fnglVertexAttrib3fARB glVertexAttrib3fARB;
  fnglVertexAttrib4fARB glVertexAttrib4fARB;

  fnglUniformMatrix4fvARB glUniformMatrix4fvARB;
};

#endif // ifndef SHADERS_HPP_INCLUDED
