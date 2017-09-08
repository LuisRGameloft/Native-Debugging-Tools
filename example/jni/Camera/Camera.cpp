#include <Camera/Camera.hpp>

glm::vec3 Camera::axisX(1.0f,0.0f,0.0f);
glm::vec3 Camera::axisY(0.0f,1.0f,0.0f);
glm::vec3 Camera::axisZ(0.0f,0.0f,1.0f);

Camera::Camera() : position(0.0f,0.0f,0.0f), rotation(0.0f,0.0f,0.0f),
                   scale(1.0f,1.0f,1.0f), projectionMatrix(1.0f)
{

}

void Camera::init(float px, float py, float pz,
                  float rx, float ry, float rz)
{
    position.x = px;
    position.y = py;
    position.z = pz;

    rotation.x = rx;
    rotation.y = ry;
    rotation.z = rz;

    projectionMatrix = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 200.0f);
}

glm::mat4 Camera::calcViewMatrix() const
{
	return
		glm::scale(scale) *
		glm::rotate(-rotation.x, axisX) *
		glm::rotate(-rotation.y, axisY) *
		glm::rotate(-rotation.z, axisZ) *
		glm::translate(-position);
}


const glm::mat4& Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}
