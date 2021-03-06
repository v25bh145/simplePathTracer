#include "texture.h"
namespace pathTracer {
	void Texture2D::deepCopy(Texture2D*& texture2D)
	{
		texture2D = new Texture2D();
		texture2D->mipmap = this->mipmap;
		texture2D->format = this->format;
		texture2D->height = this->height;
		texture2D->width = this->width;
		texture2D->SAxisBorderStrategy = this->SAxisBorderStrategy;
		texture2D->TAxisBorderStrategy = this->TAxisBorderStrategy;
		texture2D->nrComponents = this->nrComponents;
		texture2D->filterMode = this->filterMode;
	}
	Vector4f Texture2D::mapping(Vector2f uv, Vector2f du, Vector2f dv)
	{
		float u = uv.x() * width;
		float v = uv.y() * height;
		int floorU = floor(u);
		if (u > (float)width - 1.f) {
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
		if (v > (float)height - 1.f) {
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
		//return { du.x(), 0.f, 0.f, 0.f };

		// MIPMap level
		// TODO: ???????Թ???
		float mipmapWidth = max(abs(du.x()), max(abs(du.y()), max(abs(dv.x()), abs(dv.y()))));

		//string info = "";
		//ostringstream buffer(info);
		//buffer << "pixel: uv=" << vector2fToString(uv) << "du = " << vector2fToString(du) << ", || du || = " << du.norm() << ", ";
		//buffer << "dv= " << vector2fToString(dv) << ", ||dv||=" << dv.norm() << ", texelSize=" << texelSize << endl;
		//cout << buffer.str() << endl;

		//cout << mipmapWidth << endl;

		Vector4f color = { 0.f, 0.f, 0.f, 0.f };
		switch (filterMode) {
		case FILTERMODE::NEAREST: {
			int uInt = round(u), vInt = round(v);
			uInt = clampi(uInt, 0, width - 1);
			vInt = clampi(vInt, 0, height - 1);
			//cout << "uInt=" << uInt << ", vInt=" << vInt << endl;
			color = getColor(uInt, vInt, mipmapWidth);
			break;
		}
		case FILTERMODE::BILERP: {
			// can't use the previous floorU & floorV, because at that time u & v may be out of range 
			float floorU = floor(u), floorV = floor(v);
			float decimalU = u - floorU, decimalV = v - floorV;
			Vector4f colorA = getColor(floorU, floorV, mipmapWidth);
			Vector4f colorB = getColor(floorU + 1, floorV, mipmapWidth);
			Vector4f colorC = getColor(floorU, floorV + 1, mipmapWidth);
			Vector4f colorD = getColor(floorU + 1, floorV + 1, mipmapWidth);
			Vector4f colorLerpAB = colorA * decimalU + colorB * (1.f - decimalU);
			Vector4f colorLerpCD = colorC * decimalU + colorD * (1.f - decimalU);
			color = colorLerpAB * decimalV + colorLerpCD * (1.f - decimalV);
			break;
		}
		}
		//cout << vector3fToString({color.x(), color.y(), color.z()}) << endl;
		color /= 255.f;
		return color;
	}
	Vector4f Texture2D::getColor(int u, int v, float mipmapLevel = 0.f)
	{
		return this->mipmap->lookUp(u, v, mipmapLevel);
		//Vector4f color = { 0.f, 0.f, 0.f, 0.f };
		//int index = (u * width + v) * nrComponents;
		//if (nrComponents == 1)
		//	color = { (float)data[index], 1.f, 1.f, 1.f };
		//else if (nrComponents == 3)
		//	color = { (float)data[index], (float)data[index + 1], (float)data[index + 2], 1.f };
		//else if (nrComponents == 4)
		//	color = { (float)data[index], (float)data[index + 1], (float)data[index + 2], (float)data[index + 3] };
		//return color;
	}
}