

#include <Scene.hpp>
#include <InputManager/InputManager.hpp>
#include <Utils/gl.h>

Monkey Scene::monkey;
Camera Scene::cam;

void Scene::init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    monkey.init();
    cam.init();
    cam.position.z = 5.0f;
}

void Scene::update(float dt)
{
    monkey.rotation.z += dt * 1.00f;
   float rot_speed = InputManager::keyboard.lctrl? 6.0f : 3.0f;

   if (InputManager::keyboard.right) cam.rotation.y -= rot_speed * dt;
   if (InputManager::keyboard.left) cam.rotation.y += rot_speed * dt;
   if (InputManager::keyboard.up) cam.rotation.x += rot_speed * dt;
   if (InputManager::keyboard.down) cam.rotation.x -= rot_speed * dt;


   if (InputManager::keyboard.rshift) cam.scale.z += rot_speed * dt;
   if (InputManager::keyboard.rctrl) cam.scale.z -= rot_speed * dt;

	// TRASLADAR LA CAMARA
	// movimiento del jugador
  glm::vec4 tmp_movement(0.0f, 0.0f, 0.0f, 0.0f);
  if (InputManager::keyboard.w) tmp_movement.z -= rot_speed * dt;
  if (InputManager::keyboard.s) tmp_movement.z += rot_speed * dt;
  if (InputManager::keyboard.a) tmp_movement.x -= rot_speed * dt;
  if (InputManager::keyboard.d) tmp_movement.x += rot_speed * dt;
  if (InputManager::keyboard.lshift) tmp_movement.y -= rot_speed * dt;
  if (InputManager::keyboard.space) tmp_movement.y += rot_speed * dt;

  tmp_movement = glm::rotate(cam.rotation.y, glm::vec3(0.0f, 1.0f, 0.0)) * tmp_movement;
  cam.position += glm::vec3(tmp_movement.x, tmp_movement.y, tmp_movement.z);
}

void Scene::render()
{
    // It clears the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    monkey.render(cam);
}

void Scene::cleanUp()
{

}
