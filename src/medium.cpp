//
// Created by v25bh on 2021/12/20.
//
#include "medium.h"

namespace pathTracer {
	float Medium::p_surf(float tMax)
	{
		return  (exp(-sigma_t.x() * tMax) + exp(-sigma_t.y() * tMax) + exp(-sigma_t.z() * tMax)) / 3;
	}
	float Medium::sample_t(float& p_t, Vector3f& tr)
	{
		RandomGenerator randomGenerator;
		int channal = randomGenerator.uniformNToM(0, 2);
		float sigma_t_array[3] = {sigma_t.x(), sigma_t.y(), sigma_t.z()};
		float sigma_t_channal = sigma_t_array[channal];
		float u = randomGenerator.uniform0To1();
		return -log(1 - u) / sigma_t_channal;
	}
	void Medium::sample_phase(Interaction* interaction, Ray* wi)
	{
		Vector3f n = interaction->ray->direction;
		Vector3f s, t;
		orthogonal(n, s, t);
		Matrix3f trans_obj_world;
		trans_obj_world <<
			s.x(), t.x(), n.x(),
			s.y(), t.y(), n.y(),
			s.z(), t.z(), n.z();

		RandomGenerator randomGenerator;
		float u = randomGenerator.uniform0To1(), v = randomGenerator.uniform0To1();
		float alpha = u * 2 * PI;
		float cosalpha = cos(alpha), sinalpha = sin(alpha);
		float costheta;
		if (g == 0) {
			costheta = 1 - 2 * v;
		}
		else {
			costheta = (1 + g * g - pow((1 - g * g) / (1 - g + 2 * g * v), 2)) / (2 * g);
		}
		float theta = acos(costheta);
		float sintheta = sin(theta);
		Vector3f wi_obj = { cosalpha * sintheta , sinalpha * sintheta , costheta };
		Vector3f wi_world = trans_obj_world * wi_obj;

		*wi = Ray(interaction->p, { wi_world.x(), wi_world.y(), wi_world.z() }, 0);
	}
	Vector3f Medium::tr(float t)
	{
		return {
			exp(-sigma_t.x() * t),
			exp(-sigma_t.y() * t),
			exp(-sigma_t.z() * t)
		};
	}
	void Medium::deepCopy(Medium*& medium)
	{
		medium = new Medium(this->sigma_t, this->sigma_s, this->g);
	}
	Medium* sameSideMedium(Interaction* interaction)
	{
		Vector3f dir = interaction->ray->direction;
		Vector3f outsideNormal = interaction->geometry->getOutsideNormal();
		if (vector3fEqualTo0(outsideNormal)) return nullptr;
		if (dir.dot(outsideNormal) > 0) return interaction->geometry->getOutsideMedium();
		else return interaction->geometry->getInsideMedium();
	}
	Medium* otherSideMedium(Interaction* interaction)
	{
		Vector3f dir = interaction->ray->direction;
		Vector3f outsideNormal = interaction->geometry->getOutsideNormal();
		if (vector3fEqualTo0(outsideNormal)) return nullptr;
		if (dir.dot(outsideNormal) > 0) return interaction->geometry->getInsideMedium();
		else return interaction->geometry->getOutsideMedium();
	}
}