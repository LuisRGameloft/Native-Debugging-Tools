#pragma once
#ifndef _UTILS_UTILS_H
#define _UTILS_UTILS_H

#include <Utils/gl.h>

char * LoadTGA( const char * szFileName, int * width, int * height, int * bpp );
void LoadFace(const char* bufferTGA, int width, GLenum type, int bytePP, char* face);

#endif // _UTILS_UTILS_H
