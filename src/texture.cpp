#include "texture.h"
namespace pathTracer {
	void Texture2D::deepCopy(Texture2D*& texture2D)
	{
		texture2D = new Texture2D();
		texture2D->data = this->data;
		texture2D->format = this->format;
		texture2D->height = this->height;
		texture2D->width = this->width;
		texture2D->SAxisBorderStrategy = this->SAxisBorderStrategy;
		texture2D->TAxisBorderStrategy = this->TAxisBorderStrategy;
		texture2D->nrComponents = this->nrComponents;
	}
	Vector4f Texture2D::mapping(Vector2f uv, float pixelSize)
	{
		float u = uv.x() * width;
		float v = uv.y() * height;
		int floorU = floor(u);
		if (u > width) {
			switch (SAxisBorderStrategy) {
			case BORDER_STRATEGY::BLACK:
				return { 0, 0, 0, 0 };
				break;
			case BORDER_STRATEGY::CLAMP_TO_BORDER:
				u = float(width - 1);
				break;
			case BORDER_STRATEGY::REPEAT:
				u = float(floorU % width) + (u - float(floor(u)));
				break;
			case BORDER_STRATEGY::REPEAT_MIRROR:
				u = float(floorU % (width * 2)) + (u - float(floor(u)));
				if (u > width)
					u = 2 * width - u;
				break;
			}
		}
		int floorV = floor(v);
		if (v > height) {
			switch (TAxisBorderStrategy) {
			case BORDER_STRATEGY::BLACK:
				return { 0, 0, 0, 0 };
				break;
			case BORDER_STRATEGY::CLAMP_TO_BORDER:
				v = float(height - 1);
				break;
			case BORDER_STRATEGY::REPEAT:
				v = float(floorV % height) + (v - float(floor(v)));
				break;
			case BORDER_STRATEGY::REPEAT_MIRROR:
				v = float(floorV % (height * 2)) + (v - float(floor(v)));
				if (v > height)
					v = 2 * height - v;
				break;
			}
		}
		int uInt, vInt;
		cout << "pixelSize=" << pixelSize << endl;
		//if(pixelSize.x() > 1)

		// no MIPMap
		uInt = round(u); vInt = round(v);
		uInt = clampi(uInt, 0, width - 1);
		vInt = clampi(vInt, 0, height - 1);
		//cout << "uInt=" << uInt << ", vInt=" << vInt << endl;
		int index = (uInt * width + vInt) * nrComponents;
		Vector4f color = { 0.f, 0.f, 0.f, 0.f };
		if (nrComponents == 1)
			color = { (float)data[index], 1.f, 1.f, 1.f};
		else if (nrComponents == 3)
			color = { (float)data[index], (float)data[index + 1], (float)data[index + 2], 1.f };
		else if (nrComponents == 4)
			color = { (float)data[index], (float)data[index + 1], (float)data[index + 2], (float)data[index + 3] };
		//cout << vector3fToString({color.x(), color.y(), color.z()}) << endl;
		color /= 255.f;
		return color;
	}
}