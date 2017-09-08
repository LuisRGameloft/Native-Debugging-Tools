
#include <ModelLoader/ModelLoader.hpp>
#include <ModelLoader/FormatDetector.hpp>
#include <ModelLoader/PLYLoader.hpp>

bool ModelLoader::loadModel(Model *m, const char* filename)
{
    FormatDetector::FDenum format = FormatDetector::detectFormat(filename);
    if(format == FormatDetector::PLY_FORMAT)
        return PLYLoader::loadModel(m, filename);
    return false; // FormatDetector::UNKNOWN_FORMAT
}
