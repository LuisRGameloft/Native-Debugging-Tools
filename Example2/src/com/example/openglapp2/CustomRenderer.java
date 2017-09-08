package com.example.openglapp2;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;;
import javax.microedition.khronos.opengles.GL10;

public class CustomRenderer implements GLSurfaceView.Renderer {
    public void onDrawFrame(GL10 gl) {
        Native.update();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Native.resize(width, height);
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Native.init();
    }
}
