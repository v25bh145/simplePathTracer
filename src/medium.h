//
// Created by v25bh on 2021/12/20.
//

#ifndef MEDIA_H
#define MEDIA_H

#include "default.h"
#include "interaction.h"

namespace pathTracer {
	class Interaction;
	// constant medium[Õ¨÷ ΩÈ÷ ]
	class Medium {
	public:
		Vector3f sigma_t, sigma_s;
		float g;
		Medium(Vector3f sigma_t, Vector3f sigma_s, float g = 0):sigma_t(sigma_t), sigma_s(sigma_s), g(g){}

		float p_surf(float tMax);
		float sample_t(float& p_t, Vector3f& tr);
		void sample_phase(Interaction* interaction, Ray* wi);
		Vector3f phase(Interaction* interaction, Ray* wi, float& pdf);
		
		Vector3f tr(float t);

		void deepCopy(Medium*& medium);
	};
	Medium* sameSideMedium(Interaction* interaction);
	Medium* otherSideMedium(Interaction* interaction);
}

#endif