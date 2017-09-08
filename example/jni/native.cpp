#include "Native.hpp"
#include <GLES2/gl2.h>
#include <Scene.hpp>

// #include <stdlib.h>
// #include "debug.hpp"
// 
// GLint shaderProgram;
// GLint vertexShader;
// GLint fragmentShader;
// 
// GLuint vbo_triangle;
// 
// void create_shader_program() {
//     const char * vertexShaderSource =
//         "#version 100\n"
//         "attribute vec3 pos;\n"
//         "void main() {\n"
//         "    gl_Position= vec4(pos, 1.0);\n"
//         "}\n"
//         "\n";
// 
//     const char * fragmentShaderSource =
//         "#version 100\n"
//         "precision mediump float;\n"
//         "void main() {\n"
//         "    gl_FragColor= vec4(1.0, 0.0, 0.0, 1.0);\n"
//         "}\n"
//         "\n";
// 
//         vertexShader = glCreateShader(GL_VERTEX_SHADER);
//         fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//         glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//         glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//     
//         GLint compiled = GL_FALSE;
//         glCompileShader(vertexShader);
//         glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
//         if (!compiled) {
//             GLint infoLogLen = 0;
//             glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLen);
//             if (infoLogLen > 0) {
//                 GLchar* infoLog = (GLchar*)malloc(infoLogLen);
//                 if (infoLog) {
//                     glGetShaderInfoLog(vertexShader, infoLogLen, NULL, infoLog);
//                     debug("Could not compile %s shader:\n%s\n", "vertex", infoLog);
//                     free(infoLog);
//                 }
//             }
//         }
// 
//         glCompileShader(fragmentShader);
//         glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
//         if (!compiled) {
//             GLint infoLogLen = 0;
//             glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLen);
//             if (infoLogLen > 0) {
//                 GLchar* infoLog = (GLchar*)malloc(infoLogLen);
//                 if (infoLog) {
//                     glGetShaderInfoLog(fragmentShader, infoLogLen, NULL, infoLog);
//                     debug("Could not compile %s shader:\n%s\n","fragment",infoLog);
//                     free(infoLog);
//                 }
//             }
//         }
// 
//         shaderProgram = glCreateProgram();
//         glAttachShader(shaderProgram, vertexShader);
//         glAttachShader(shaderProgram, fragmentShader);
//         glLinkProgram(shaderProgram);
// }
// 
// void init() {
//   glClearColor(0.0, 0.0, 0.2, 1.0);
//   create_shader_program();
// 
//   glGenBuffers(1, &vbo_triangle);
//   glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
//   float triangle[9] = {
//   	-0.5f, -0.5f, 0.0f,
//   	 0.5f, -0.5f, 0.0f,
//   	 0.0f,  0.5f, 0.0f,
//   };
//   glBufferData(GL_ARRAY_BUFFER, sizeof triangle, triangle, GL_STATIC_DRAW);
//   glBindBuffer(GL_ARRAY_BUFFER, 0);
// }
// 
// void draw() {
//     glClear(GL_COLOR_BUFFER_BIT);
//     glUseProgram(shaderProgram);
// 
//     GLint loc = glGetAttribLocation(shaderProgram, "pos");
//     if(loc == -1)
//     	debug("Error, loc : %i\n", loc);
//     glEnableVertexAttribArray(loc);
// 
//     glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
//     glVertexAttribPointer(	loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
// 
//     glDrawArrays(GL_TRIANGLES, 0, 3);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
// }
// 
// void update() {
// 
// }
// 



#include "../../InAppRemoteShell/InAppRemoteShell.hpp"

////////////////////////////////////////////////////////////////////////////////
// JNI FUNCTIONS, DO NOT TOUCH
////////////////////////////////////////////////////////////////////////////////
JNIEXPORT
void JNICALL Java_com_example_openglapp2_Native_init(JNIEnv *env, jclass jclass1) {
    Scene::init();
}

JNIEXPORT
void JNICALL Java_com_example_openglapp2_Native_update(JNIEnv *env, jclass jclass1) {
    Scene::update(0.05f);
    Scene::render();
}

JNIEXPORT
void JNICALL Java_com_example_openglapp2_Native_resize(JNIEnv *env, jclass jclass1, jint width, jint height) {
    glViewport(0, 0, width, height);
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	InAppRemoteShell::Init();
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    return JNI_VERSION_1_6;
}