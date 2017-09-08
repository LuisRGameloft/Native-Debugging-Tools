package com.example.openglapp2;

public class Native
{
    static
    {
        System.loadLibrary("Native");
    }
    public static native void init();
    public static native void update();
    public static native void resize(int width, int height);
}
