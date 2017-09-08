#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include <Camera/Camera.hpp>
#include <Monkey.hpp>

class Scene
{
    private:
        static Monkey monkey;
        static Camera cam;
    public:
        static void init();
        static void update(float dt = 0.0f);
        static void render();
        static void cleanUp();
};

#endif //_SCENE_H_
