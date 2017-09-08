////////////////////////////////////////////////////////////////////////////////
//        Class name : ModelLoader
//           Authors : David Landeros and Omar Gaylord
//              Date : 04/April/2016
//       Description : a static class that loads 3D models into the RAM
// Supported formats : Standform Polygon format (*.ply)
//
// Usage:
//   1. Model m;
//   2. if (ModelLoader::loadModel( &m, "./monkey.ply") )
//   3.     cout << "model succesfully loaded" << endl;
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _MODEL_LOADER_MODEL_LOADER_H_
#define _MODEL_LOADER_MODEL_LOADER_H_

class ModelLoader
{
    public:
    struct Model
    {
        float *data;
        short vertex_count;
        short vertex_stride;
        short vertex_offset;
        short color_offset;
        short normal_offset;
        short uv_offset;
        int indices_count;
        short *indices;
    };
    static bool loadModel(Model *model, const char* filename);
    static void freeModel(Model *model);
};

#endif //_MODEL_LOADER_MODEL_LOADER_H_
