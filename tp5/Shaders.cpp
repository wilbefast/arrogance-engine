/*#include "Shaders.hpp"

int Shaders::startup()
{
  const char* sExtensions = (const char *)glGetString(GL_EXTENSIONS);

  if (strstr(sExtensions, "GL_ARB_vertex_shader")
      && strstr(sExtensions, "GL_ARB_fragment_shader")
      && strstr(sExtensions, "GL_ARB_shader_objects")
      && strstr(sExtensions, "GL_ARB_shading_language_100"))
  {
    glCreateProgramObjectARB
      = (fnglCreateProgramObjectARB)wglGetProcAddress("glCreateProgramObjectARB");
    glCreateShaderObjectARB
      = (fnglCreateShaderObjectARB)wglGetProcAddress("glCreateShaderObjectARB");
    glDeleteObjectARB
      = (fnglDeleteObjectARB)wglGetProcAddress("glDeleteObjectARB");
    glDetachObjectARB
      = (fnglDetachObjectARB)wglGetProcAddress("glDetachObjectARB");
    glAttachObjectARB
      = (fnglAttachObjectARB)wglGetProcAddress("glAttachObjectARB");

    glShaderSourceARB
      = (fnglShaderSourceARB)wglGetProcAddress("glShaderSourceARB");
    glCompileShaderARB
      = (fnglCompileShaderARB)wglGetProcAddress("glCompileShaderARB");

    glLinkProgramARB
      = (fnglLinkProgramARB)wglGetProcAddress("glLinkProgramARB");
    glGetInfoLogARB
      = (fnglGetInfoLogARB)wglGetProcAddress("glGetInfoLogARB");
    glUseProgramObjectARB
      = (fnglUseProgramObjectARB)wglGetProcAddress("glUseProgramObjectARB");

    glGetProgramivARB
      = (fnglGetProgramivARB)wglGetProcAddress("glGetProgramivARB");

    glGetObjectParameterivARB
      = (fnglGetObjectParameterivARB)wglGetProcAddress("glGetObjectParameterivARB");
    glGetObjectParameterfvARB
      = (fnglGetObjectParameterfvARB)wglGetProcAddress("glGetObjectParameterfvARB");
    glGetUniformLocationARB
      = (fnglGetUniformLocationARB)wglGetProcAddress("glGetUniformLocationARB");
    glGetAttribLocationARB
      = (fnglGetAttribLocationARB)wglGetProcAddress("glGetAttribLocationARB");

    glBindAttribLocationARB
      = (fnglBindAttribLocationARB)wglGetProcAddress("glBindAttribLocationARB");

    glUniform1fARB = (fnglUniform1fARB)wglGetProcAddress("glUniform1fARB");
    glUniform2fARB = (fnglUniform2fARB)wglGetProcAddress("glUniform2fARB");
    glUniform3fARB = (fnglUniform3fARB)wglGetProcAddress("glUniform3fARB");
    glUniform4fARB = (fnglUniform4fARB)wglGetProcAddress("glUniform4fARB");

    glUniform1iARB = (fnglUniform1iARB)wglGetProcAddress("glUniform1iARB");
    glUniform2iARB = (fnglUniform2iARB)wglGetProcAddress("glUniform2iARB");
    glUniform3iARB = (fnglUniform3iARB)wglGetProcAddress("glUniform3iARB");
    glUniform4iARB = (fnglUniform4iARB)wglGetProcAddress("glUniform4iARB");

    glUniform1fvARB = (fnglUniform1fvARB)wglGetProcAddress("glUniform1fvARB");
    glUniform2fvARB = (fnglUniform2fvARB)wglGetProcAddress("glUniform2fvARB");
    glUniform3fvARB = (fnglUniform3fvARB)wglGetProcAddress("glUniform3fvARB");
    glUniform4fvARB = (fnglUniform4fvARB)wglGetProcAddress("glUniform4fvARB");

    glUniform1ivARB = (fnglUniform1ivARB)wglGetProcAddress("glUniform1ivARB");
    glUniform2ivARB = (fnglUniform2ivARB)wglGetProcAddress("glUniform2ivARB");
    glUniform3ivARB = (fnglUniform3ivARB)wglGetProcAddress("glUniform3ivARB");
    glUniform4ivARB = (fnglUniform4ivARB)wglGetProcAddress("glUniform4ivARB");

    glVertexAttrib1fvARB
      = (fnglVertexAttrib1fvARB)wglGetProcAddress("glVertexAttrib1fvARB");
    glVertexAttrib2fvARB
      = (fnglVertexAttrib2fvARB)wglGetProcAddress("glVertexAttrib2fvARB");
    glVertexAttrib3fvARB
      = (fnglVertexAttrib3fvARB)wglGetProcAddress("glVertexAttrib3fvARB");
    glVertexAttrib4fvARB
      = (fnglVertexAttrib4fvARB)wglGetProcAddress("glVertexAttrib4fvARB");

    glVertexAttrib1fARB
      = (fnglVertexAttrib1fARB)wglGetProcAddress("glVertexAttrib1fARB");
    glVertexAttrib2fARB
      = (fnglVertexAttrib2fARB)wglGetProcAddress("glVertexAttrib2fARB");
    glVertexAttrib3fARB
      = (fnglVertexAttrib3fARB)wglGetProcAddress("glVertexAttrib3fARB");
    glVertexAttrib4fARB
      = (fnglVertexAttrib4fARB)wglGetProcAddress("glVertexAttrib4fARB");

    glUniformMatrix4fvARB
      = (fnglUniformMatrix4fvARB)wglGetProcAddress("glUniformMatrix4fvARB");
}
*/
