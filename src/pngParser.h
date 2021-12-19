//
// Created by v25bh on 2021/11/22.
//

#ifndef SIMPLEPATHTRACER_PNGPARSER_H
#define SIMPLEPATHTRACER_PNGPARSER_H

#include "default.h"

namespace pathTracer {
    class PngParser {
    private:
        Vector3f** pixels;
        pair<int, int> resolution;
    public:
       PngParser(Vector3f** pixels, pair<unsigned, unsigned> resolution);
       void writeToFile(string fileName);
    };
}

#endif //SIMPLEPATHTRACER_PNGPARSER_H
