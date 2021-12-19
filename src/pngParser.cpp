//
// Created by v25bh on 2021/11/22.
//

#include "pngParser.h"

namespace pathTracer {
    PngParser::PngParser(Vector3f** inputPixels, pair<unsigned, unsigned> resolution) {
        this->resolution = resolution;
        this->pixels = inputPixels;
    }
    void PngParser::writeToFile(string fileName) {
        png_byte **buffer;

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        png_infop info_ptr = png_create_info_struct(png_ptr);
        FILE* f = fopen(fileName.c_str(), "wb");
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, resolution.first, resolution.second, 8,
                      PNG_COLOR_TYPE_RGB,
                      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                      PNG_FILTER_TYPE_DEFAULT);
        png_write_info(png_ptr, info_ptr);
        png_set_bgr(png_ptr);

        buffer = new png_byte*[resolution.second];
        for(int y = 0; y < resolution.second; ++y) {
            buffer[y] = new png_byte[resolution.first * 3];
            for(int x = 0; x < resolution.first; ++x) {
                // reverse
                buffer[y][3 * x + 0] = (png_byte)pixels[x][y].z();
                buffer[y][3 * x + 1] = (png_byte)pixels[x][y].y();
                buffer[y][3 * x + 2] = (png_byte)pixels[x][y].x();
            }
        }
        png_write_image(png_ptr, buffer);
        png_write_end(png_ptr, info_ptr);
        delete[] buffer;
        if(f) fclose(f);
    }
}