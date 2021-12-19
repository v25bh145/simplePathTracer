//
// Created by v25bh on 2021/11/22.
//
#include <iostream>
#include <string>

#include "../../src/pngParser.h"
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;
using namespace pathTracer;
int main(int argc, char** argv) {
    cout << "outputPngTest.cpp" << endl;
    int w = 1024, h = 512;
    auto** pixels = new Vector3f*[w];
    for(int i = 0; i < w; ++i) {
        pixels[i] = new Vector3f[h];
        for(int j = 0; j < h; ++j) {
            pixels[i][j] = {255, 0, 0};
        }
        for (int j = h / 2; j < h; ++j) {
            pixels[i][j] = { 0, 0, 0 };
        }
    }
    for (int i = w / 2; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            pixels[i][j] = { 255, 182, 193 };
        }
        for (int j = h / 2; j < h; ++j) {
            pixels[i][j] = { 255, 255, 255 };
        }
    }

    PngParser pngParser = PngParser(pixels, {w, h});
    pngParser.writeToFile("result.png");

    for(int i = 0; i < w; ++i)
        delete[] pixels[i];
    delete[] pixels;
    return 0;
}
