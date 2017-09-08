#include <Monkey.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <SharedConstants.hpp>
#include <Utils/debug.h>

Monkey::Monkey() : vbo_indices(0), vbo_data(0), rotation(0.0f,0.0f,0.0f), position(0.0f,-0.5f,0.0f), scale(0.5f,0.5f,0.5f)
{
    rotation.x = glm::radians(-90.0f);
}

Monkey::~Monkey()
{
    glDeleteBuffers(1, &vbo_data);
    glDeleteBuffers(1, &vbo_indices);
}


void Monkey::init()
{
    shader.init( RESOURCE_PATH "/shaders/monkey.vs",  RESOURCE_PATH "/shaders/monkey.fs");
    if(!ModelLoader::loadModel(&model,  RESOURCE_PATH "/models/skull.ply"))
    {
        debug("No se pudo cargar el modelo\n");
    }
    glGenBuffers(1, &vbo_data);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_data);
    glBufferData(GL_ARRAY_BUFFER, model.vertex_count * model.vertex_stride, model.data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices_count * sizeof(short), model.indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Monkey::render(const Camera& cam)
{
    shader.useProgram();
    
    GLint loc = shader.getUniformLocation("mvp");
    glm::mat4 mvp = cam.getProjectionMatrix() * cam.calcViewMatrix() * calcModelMatrix();
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));

    loc = shader.getAttribLocation("pos");
    glEnableVertexAttribArray(loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_data);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, model.vertex_stride, (void*)model.vertex_offset);

    loc = shader.getAttribLocation("norm");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, model.vertex_stride, (void*)model.normal_offset);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glDrawElements(GL_TRIANGLES, model.indices_count, GL_UNSIGNED_SHORT, 0);
}

glm::mat4 Monkey::calcModelMatrix()
{
    return glm::translate(position) *
           glm::scale(scale) *
           glm::rotate(rotation.x, glm::vec3(1.0f,0.0f,0.0f)) *
           glm::rotate(rotation.y, glm::vec3(0.0f,1.0f,0.0f)) *
           glm::rotate(rotation.z, glm::vec3(0.0f,0.0f,1.0f));
}