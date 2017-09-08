#pragma once
#ifndef _MONKEY_H_
#define _MONKEY_H_

#include <Drawables/Drawable.hpp>
#include <ShaderProgram/ShaderProgram.hpp>
#include <ModelLoader/ModelLoader.hpp>

class Monkey : Drawable
{
   public:
       Monkey();
       ~Monkey();
       void init();
       void render(const Camera& cam);
       glm::vec3 position;
       glm::vec3 rotation;
       glm::vec3 scale;

   private:
       ShaderProgram shader;
       ModelLoader::Model model;

       GLuint vbo_data;
       GLuint vbo_indices;

       glm::mat4 calcModelMatrix();
};

#endif //_MONKEY_H_