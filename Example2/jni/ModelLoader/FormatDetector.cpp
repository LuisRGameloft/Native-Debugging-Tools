#include <ModelLoader/FormatDetector.hpp>

FormatDetector::FDenum FormatDetector::detectFormat(const char* filename)
{
    return FormatDetector::PLY_FORMAT;
}
