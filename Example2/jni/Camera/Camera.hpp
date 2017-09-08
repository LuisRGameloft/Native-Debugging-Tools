#pragma once
#ifndef _CAMERA_CAMERA_H_
#define _CAMERA_CAMERA_H_
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
    public:
        Camera();
        void init(float px = 0.0f, float py = 0.0f, float pz = 0.0f,
                  float rx = 0.0f, float ry = 0.0f, float rz = 0.0f );
        glm::mat4 calcViewMatrix() const;
        const glm::mat4& getProjectionMatrix() const;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

    private:
        glm::mat4 projectionMatrix;

        static glm::vec3 axisX;
        static glm::vec3 axisY;
        static glm::vec3 axisZ;
};

#endif // _CAMERA_CAMERA_H_
