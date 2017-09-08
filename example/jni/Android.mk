
##############################################
#        3DBasicFramework for Android        #
#        date: 6/20/2016                     #
##############################################

LOCAL_PATH          := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE        := Native
LOCAL_SRC_FILES     := ./native.cpp\
                    ./Scene.cpp\
                    ./Monkey.cpp\
                    ./Utils/tga.cpp\
                    ./Textures/Texture.cpp\
                    ./ShaderProgram/ShaderProgram.cpp\
                    ./ModelLoader/FormatDetector.cpp\
                    ./ModelLoader/PLYLoader.cpp\
                    ./ModelLoader/ModelLoader.cpp\
                    ./InputManager/InputManager.cpp\
                    ./Camera/Camera.cpp
LOCAL_CFLAGS        += -I $(LOCAL_PATH)
LOCAL_CFLAGS        += -I "$(CC_COMMON_LIBS)/glm"
LOCAL_CFLAGS        += -DOS_ANDROID -DANDROID
LOCAL_CFLAGS        += -Wno-int-to-pointer-cast
LOCAL_CPPFLAGS      += -fno-rtti -fno-exceptions -std=c++11 -g
LOCAL_CFLAGS        += -fno-rtti -fno-exceptions -std=c++11 -g
LOCAL_LDLIBS        += -lGLESv3 -lEGL -llog
NDK_TOOLCHAIN_VERSION := clang
include $(BUILD_SHARED_LIBRARY)