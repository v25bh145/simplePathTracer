//
// Created by v25bh on 2021/11/22.
//
#include <iostream>
#include <string>

#include "../../src/pathTracer.h"
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;
using namespace pathTracer;
int main(int argc, char** argv) {
    cout << "outputPngTest.cpp" << endl;
    // mapping test
    Texture2D texture("../../../../assets/bricks2.jpg", BORDER_STRATEGY::BLACK, BORDER_STRATEGY::BLACK, FILTERMODE::BILERP, MIPMAP_AUTO);
    //texture.width = 512.f;
    //texture.height = 512.f;
    //texture.nrComponents = 3;
    //texture.format = IMAGEFORMAT::RED;
    //texture.SAxisBorderStrategy = texture.TAxisBorderStrategy = BORDER_STRATEGY::BLACK;
    //texture.mapping({ 0.5f, 0.665f }, { 1.f, 1.f });

    int w = texture.width, h = texture.height;
    auto** pixels = new Vector3f * [w];
    for (int i = 0; i < w; ++i) {
        pixels[i] = new Vector3f[h];
    }

    int width = texture.width;
    int height = texture.height;
    for (int level = 0; level <= texture.mipmap->mipmapLevel; ++level) {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                int index = (i * width + j) * texture.nrComponents;
                Vector4f color = { 0.f, 0.f, 0.f, 0.f };
                if (texture.nrComponents == 1)
                    color = { (float)texture.mipmap->pyramid[level][index], 1.f, 1.f, 1.f };
                else if (texture.nrComponents == 3)
                    color = { (float)texture.mipmap->pyramid[level][index], (float)texture.mipmap->pyramid[level][index + 1], (float)texture.mipmap->pyramid[level][index + 2], 1.f };
                else if (texture.nrComponents == 4)
                    color = { (float)texture.mipmap->pyramid[level][index], (float)texture.mipmap->pyramid[level][index + 1], (float)texture.mipmap->pyramid[level][index + 2], (float)texture.mipmap->pyramid[level][index + 3] };
                pixels[j][i] = { color.x(), color.y(), color.z() };
            }
        }
        //PngParser pngParser = PngParser(pixels, {w, h});
        PngParser pngParser = PngParser(pixels, { width, height });
        string name("mipmapLevel");
        name.append(1, (char)(level + 48));
        name.append(".png");
        cout << name << endl;
        //pngParser.writeToFile(name);
        width /= 2;
        height /= 2;
    }

    for(int i = 0; i < w; ++i)
        delete[] pixels[i];
    delete[] pixels;
    return 0;
}
