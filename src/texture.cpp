#include "texture.h"
namespace pathTracer {
	Vector4f Texture2D::mapping(Vector2f uv, Vector2f pixelSize)
	{
		float u = uv.x() * width;
		float v = uv.y() * width;
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
		if (v > width) {
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
		//if(pixelSize.x() > 1)
	}
}