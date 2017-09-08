#pragma once
#ifndef _MODEL_LOADER_PLY_LOADER_H_
#define _MODEL_LOADER_PLY_LOADER_H_

#include <ModelLoader/ModelLoader.hpp>

const unsigned char X_COMPONENT       = 0x1; // 00000001
const unsigned char Y_COMPONENT       = 0x2; // 00000010
const unsigned char Z_COMPONENT       = 0x4; // 00000100
const unsigned char ENABLED_ATTRIBUTE = 0x7; // 00000111
const unsigned char ENABLED_UV        = 0x3; // 00000011

struct PLYVertexHelper
{
    float x;  float y;  float z;
    float nx; float ny; float nz;
    int   r;  int   g;  int   b;
    float u;  float v;
};

struct PLYVertexStatus
{
    unsigned char position;
    unsigned char normals;
    unsigned char colors;
    unsigned char uvs;
};

class PLYFormatHelper
{
  public:
    enum PLYEnum {
      FLOAT, INT, X, Y, Z, NX, NY, NZ, R, G, B, U, V
    };
    PLYFormatHelper();
    void addProperty(int type, int name);
    PLYVertexHelper vertex;
    PLYVertexStatus status;
    int format_len;
    char format[500];
    int addr_len;
    void *addresses[11];
};

class PLYLoader
{
    public:
      static bool loadModel(ModelLoader::Model *m, const char *filename);
};

#endif //_MODEL_LOADER_PLY_LOADER_H_
