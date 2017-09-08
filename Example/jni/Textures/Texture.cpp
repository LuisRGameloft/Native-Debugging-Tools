#include <Textures/Texture.hpp>
#include <Utils/tga.h>

Texture::Texture() : texture_handler(0) {}

void Texture::init(const char *filename, GLenum type) {
  texture_type = type;
  glGenTextures(1, &texture_handler);

  if (type == GL_TEXTURE_2D) {
    glBindTexture(GL_TEXTURE_2D, texture_handler);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // parametros de Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    int width, height, bpp;
    char *bufferTGA = LoadTGA(filename, &width, &height, &bpp);
    int format = bpp == 24 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, bufferTGA);
    delete[] bufferTGA;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else if(type == GL_TEXTURE_CUBE_MAP)
  {
       //// http://math.hws.edu/graphicsbook/c7/cubemap-layout.png
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_handler);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        int width, height, bpp;
        char *bufferTGA = LoadTGA(filename, &width, &height, &bpp);
        int format = bpp == 24 ? GL_RGB : GL_RGBA;
        int bytesPP = bpp/8;

        // 512px x 384pxs
        // 128 (512/4)
        int face_width = width / 4;
        char *face = new char[face_width*face_width*bytesPP];
        LoadFace(bufferTGA, width, GL_TEXTURE_CUBE_MAP_POSITIVE_X, bytesPP, face);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X , 0, format, face_width, face_width, 0, format, GL_UNSIGNED_BYTE, face);

        LoadFace(bufferTGA, width, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, bytesPP, face);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X , 0, format, face_width, face_width, 0, format, GL_UNSIGNED_BYTE, face);

        LoadFace(bufferTGA, width, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, bytesPP, face);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y , 0, format, face_width, face_width, 0, format, GL_UNSIGNED_BYTE, face);

        LoadFace(bufferTGA, width, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bytesPP, face);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y , 0, format, face_width, face_width, 0, format, GL_UNSIGNED_BYTE, face);

        LoadFace(bufferTGA, width, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, bytesPP, face);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z , 0, format, face_width, face_width, 0, format, GL_UNSIGNED_BYTE, face);

        LoadFace(bufferTGA, width, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, bytesPP, face);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z , 0, format, face_width, face_width, 0, format, GL_UNSIGNED_BYTE, face);

        delete[] bufferTGA;
        delete[] face;

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  }
}

void Texture::bindTexture() { glBindTexture(texture_type, texture_handler); }

void Texture::makeCurrent() { glBindTexture(texture_type, texture_handler); }

Texture::~Texture() { glDeleteTextures(1, &texture_handler); }
