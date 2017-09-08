#pragma once
#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_
#include <ShaderProgram/ShaderProgram.hpp>
#include <Camera/Camera.hpp>

class Drawable // abstract class
{
    public:
    virtual void render(const Camera &cam) = 0;
    virtual void init() = 0;
};


#endif // _DRAWABLE_H_
