#pragma once
#ifndef _TEXTURES_TEXTURE_H_
#define _TEXTURES_TEXTURE_H_

#include <Utils/gl.h>

class Texture {
 public:
  Texture();
  void init(const char* filename, GLenum type = GL_TEXTURE_2D);
  void bindTexture();
  void makeCurrent();
  ~Texture();

 private:
  GLuint texture_handler;
  GLenum texture_type;
};

#endif  //_TEXTURES_TEXTURE_H_
