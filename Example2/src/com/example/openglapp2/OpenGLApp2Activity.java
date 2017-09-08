package com.example.openglapp2;

import android.app.Activity;
import android.os.Bundle;

public class OpenGLApp2Activity extends Activity
{
    CustomGLSurfaceView mView;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        mView = new CustomGLSurfaceView(getApplication());
        setContentView(mView);
    }

    @Override
    protected void onPause() {
       super.onPause();
       mView.onPause();
    }

    @Override
    protected void onResume() {
      super.onResume();
      mView.onResume();
    }
}
