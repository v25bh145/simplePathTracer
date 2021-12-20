//
// Created by v25bh on 2021/12/20.
//
#include "media.h"

namespace pathTracer {
	void Media::deepCopy(Media*& media)
	{
		media = new Media(this->sigma_t);
	}
}