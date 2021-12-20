//
// Created by v25bh on 2021/12/20.
//

#ifndef MEDIA_H
#define MEDIA_H

#include "default.h"

namespace pathTracer {
	// constant media[Õ¨÷ ΩÈ÷ ]
	class Media {
	public:
		float sigma_t;
		Media(float sigma_t):sigma_t(sigma_t){}
		void deepCopy(Media*& media);
	};
}

#endif