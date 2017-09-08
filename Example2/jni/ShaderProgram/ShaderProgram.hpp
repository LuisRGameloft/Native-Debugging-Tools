#pragma once
#ifndef _SHADERPROGRAM_SHADERPROGRAM_H
#define _SHADERPROGRAM_SHADERPROGRAM_H_

#include <Utils/gl.h>

class ShaderProgram {
 private:
  GLuint m_vertex_shader;
  GLuint m_fragment_shader;
  GLuint m_geometry_shader;
  GLuint m_program;

 public:
  ShaderProgram();
  void init(const char *vertex_shader, const char *fragment_shader, const char *geometry_shader = NULL);
  void makeCurrent();
  void useProgram();
  GLint getAttribLocation(const char *attrib);
  GLint getUniformLocation(const char *uniform);
  ~ShaderProgram();

  private:
  	char * readFile(const char *filename);
    void printCompilationError(GLuint shader_handler, GLenum target);
};

#endif  // _SHADERPROGRAM_SHADERPROGRAM_H
