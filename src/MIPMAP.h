#ifndef SIMPLEPATHTRACER_MIPMAP_H
#define SIMPLEPATHTRACER_MIPMAP_H

#include "default.h"
#include "mathHelper.h"

namespace pathTracer {
	class MIPMAP {
	public:
		int mipmapLevel, nrComponents, width, height;
		unsigned char** pyramid;
		MIPMAP() { pyramid = nullptr; mipmapLevel = 0; }
		MIPMAP(unsigned char* data0, int mipmapLevel, int nrComponents, int width, int height): nrComponents(nrComponents), width(width), height(height) {
			this->mipmapLevel = mipmapLevel < 0 ? 0 : mipmapLevel;
			pyramid = new unsigned char*[this->mipmapLevel + 1];
			pyramid[0] = data0;

			// construct
			int lastLevelWidth = width;
			for (int level = 1; level <= this->mipmapLevel; level++) {
				unsigned char* tmpData = new unsigned char[lastLevelWidth * lastLevelWidth * nrComponents / 2];
				// compress: width axis, from the last level to tmpData
				for (int w = 0; w < lastLevelWidth; w += 2) {
					for (int h = 0; h < lastLevelWidth; ++h) {
						int index = (w * lastLevelWidth + h) * nrComponents;
						int nextIndex = ((w + 1) * lastLevelWidth + h) * nrComponents;
						int compressIndex = ((w / 2) * lastLevelWidth + h) * nrComponents;
						for (int nr = 0; nr < nrComponents; ++nr) {
							tmpData[compressIndex + nr] = (pyramid[level - 1][index + nr] + pyramid[level - 1][nextIndex + nr]) / 2;
						}
					}
				}
				pyramid[level] = new unsigned char[lastLevelWidth * lastLevelWidth * nrComponents / 4];
				// compress: height axis, from tmpData to this level
				for (int w = 0; w < lastLevelWidth / 2; w++) {
					for (int h = 0; h < lastLevelWidth; h += 2) {
						int index = (w * lastLevelWidth + h) * nrComponents;
						int nextIndex = (w * lastLevelWidth + h + 1) * nrComponents;
						int compressIndex = (w * lastLevelWidth / 2 + h / 2) * nrComponents;
						for (int nr = 0; nr < nrComponents; ++nr) {
							const char d1 = tmpData[index + nr];
							const char d2 = tmpData[nextIndex + nr];
							pyramid[level][compressIndex + nr] = (tmpData[index + nr] + tmpData[nextIndex + nr]) / 2;
						}
					}
				}

				lastLevelWidth /= 2;
			}
		}
		Vector4f lookUp(int u, int v, float mipmapWidth);
	private:
		Vector4f getColor(int uLevel, int vLevel, int mipmapLevel);
	};
}

#endif