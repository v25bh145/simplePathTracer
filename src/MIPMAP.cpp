#include "MIPMAP.h"
namespace pathTracer {
	Vector4f MIPMAP::lookUp(int u, int v, float mipmapWidth)
	{
		if(this->mipmapLevel == 0) return getColor(u, v, 0);

		float l = float(this->mipmapLevel) - 1.f + log2(mipmapWidth);
		l = clampf(l, float(0), float(this->mipmapLevel));
		int floorL = floor(l);
		float decimalL = l - float(floorL);
		//cout << l << endl;
		if (decimalL < EPSILON) {
			// only bottom
			int uBottom = u / pow(2, floorL);
			int vBottom = v / pow(2, floorL);
			Vector4f colorBottom = getColor(uBottom, vBottom, floorL);
			return colorBottom;
		}
		if (1.f - decimalL < EPSILON) {
			// only top
			int uTop = u / pow(2, floorL + 1);
			int vTop = v / pow(2, floorL + 1);
			Vector4f colorTop = getColor(uTop, vTop, floorL + 1);
			return colorTop;
		}
		// lerp
		int uBottom = u / pow(2, floorL);
		int vBottom = v / pow(2, floorL);
		Vector4f colorBottom = getColor(uBottom, vBottom, floorL);
		int uTop = u / pow(2, floorL + 1);
		int vTop = v / pow(2, floorL + 1);
		Vector4f colorTop = getColor(uTop, vTop, floorL + 1);
		Vector4f lastColor = colorBottom * decimalL + colorTop * (1.f - decimalL);
		//cout << vector3fToString({ lastColor.x(), lastColor.y(), lastColor.z() });
		return lastColor;
	}
	// private
	Vector4f MIPMAP::getColor(int uLevel, int vLevel, int mipmapLevel)
	{
		Vector4f color = { 0.f, 0.f, 0.f, 0.f };
		int levelWidth = width / pow(2, mipmapLevel);
		unsigned char* data = pyramid[mipmapLevel];
		int index = (uLevel * levelWidth + vLevel) * nrComponents;
		if (nrComponents == 1)
			color = { (float)data[index], 1.f, 1.f, 1.f };
		else if (nrComponents == 3)
			color = { (float)data[index], (float)data[index + 1], (float)data[index + 2], 1.f };
		else if (nrComponents == 4)
			color = { (float)data[index], (float)data[index + 1], (float)data[index + 2], (float)data[index + 3] };
		return color;
	}
}