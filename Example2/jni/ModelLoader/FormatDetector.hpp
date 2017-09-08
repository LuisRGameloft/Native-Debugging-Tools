////////////////////////////////////////////////////////////////////////////////
//        Class name : FormatDetector
//           Authors : David Landeros and Omar Gaylord
//              Date : 04/April/2016
//       Description : a static class that detects the format of a 3D model
// Supported formats : Standform Polygon format (*.ply)
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _MODEL_LOADER_FORMAT_DETECTOR_H_
#define _MODEL_LOADER_FORMAT_DETECTOR_H_

class FormatDetector
{
    public:
    enum FDenum { UNKNOWN_FORMAT, PLY_FORMAT };
    static FDenum detectFormat(const char* filename);
};

#endif //_MODEL_LOADER_FORMAT_DETECTOR_H_
