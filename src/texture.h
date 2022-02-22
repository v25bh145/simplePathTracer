#ifndef SIMPLEPATHTRACER_TEXTURE_H
#define SIMPLEPATHTRACER_TEXTURE_H

#include "default.h"
#include "mathHelper.h"

namespace pathTracer {
    enum IMAGEFORMAT { RED = 0b1, RGB = 0b10, RGBA = 0b100};
    enum BORDER_STRATEGY {CLAMP_TO_BORDER = 0b1, REPEAT = 0b10, REPEAT_MIRROR = 0b100, BLACK = 0b1000};
    // mipmap寻找大小适合的纹理贴图，而在找到大小适合的纹理贴图后，像素匹配该贴图中纹素时的配置
    enum FILTERMODE {NEAREST = 0b1, BILERP = 0b10};
	class Texture2D {
    public:
        int width, height, nrComponents;
        unsigned char* data;
        IMAGEFORMAT format;
        BORDER_STRATEGY SAxisBorderStrategy, TAxisBorderStrategy;
        FILTERMODE filterMode;
        Texture2D() {
            data = nullptr;
        };
        Texture2D(string path, BORDER_STRATEGY SAxisBorderStrategy, BORDER_STRATEGY TAxisBorderStrategy, FILTERMODE filterMode/*, unsigned mipmapLevel*/):
            SAxisBorderStrategy(SAxisBorderStrategy), TAxisBorderStrategy(TAxisBorderStrategy), filterMode(filterMode) {
            data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
            if(!data) {
                cout << "Texture failed to load at path: " << path << endl;
                stbi_image_free(data);
                assert(false);
            }
            if (nrComponents == 1)
                format = IMAGEFORMAT::RED;
            else if (nrComponents == 3)
                format = IMAGEFORMAT::RGB;
            else if (nrComponents == 4)
                format = IMAGEFORMAT::RGBA;
        };
        //~Texture2D() {
        //    cout << "delete texture2D" << endl;
        //    stbi_image_free(data);
        //}
        void deepCopy(Texture2D*& texture2D);
        Vector4f mapping(Vector2f uv, Vector2f du, Vector2f dv, float texelSize);
        Vector4f getColor(int u, int v);
	};
};
#endif