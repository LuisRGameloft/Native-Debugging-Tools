#include "Native.hpp"
#include <GLES2/gl2.h>
#include <Scene.hpp>

#include "../../InAppRemoteShell/InAppRemoteShell.hpp"

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    /* This is the InAppRemoteShell implementation */
    InAppRemoteShell::Init();
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    return JNI_VERSION_1_6;
}

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