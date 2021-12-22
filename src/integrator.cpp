//
// Created by v25bh on 2021/12/1.
//

#include "integrator.h"

namespace pathTracer {
    // wi = nipi/(nipi+njpj)
    float equilibriumHeuristic(int ni, float pi, int nj, float pj) {
        float nipi = (float)ni * pi, njpj = (float)nj * pj;
        return nipi / (nipi + njpj);
    }
    float powerHeuristic(int ni, float pi, int nj, float pj, int beta) {
        float nipibeta = pow((float)ni * pi, beta), njpjbeta = pow((float)nj * pj, beta);
        return nipibeta / (nipibeta + njpjbeta);
    }
	Vector3f estimateDirect(Interaction* p1, Geometry* p1Geometry, Scene* scene)
	{
		Vector3f L = { 0, 0, 0 };
		// select a light [selectLightPDF = 1 / lightsNum]
		Geometry* light = scene->aggregation->uniformRandomLight();
		// no use
		float light_area_pdf;
		Vector3f light_p = light->sample_point(light_area_pdf);

		// light shadow test
		// won't use wi->direction * 0.05f£¬because this function is only called for not using specular
		Ray* lightShadowRay = new Ray(p1->p + p1->normal * 0.05f, light_p);
		auto* lightShadowInteraction = new Interaction(lightShadowRay);
		unsigned lightShadowHitGeomId = scene->intersect(lightShadowInteraction);

		if (lightShadowHitGeomId == light->getRTCInnerGeometryId() && lightShadowHitGeomId != p1Geometry->getRTCInnerGeometryId()) {
			float light_pdf;
			Vector3f L_light = light->le(lightShadowInteraction, light_pdf);
			// not on the light side
			if (L_light.x() == 0 && L_light.y() == 0 && L_light.z() == 0) {
				return { 0, 0, 0 };
			}

			float light_bxdf_pdf;
			Vector3f F_light = p1Geometry->bxdf->f(p1, lightShadowRay, light_bxdf_pdf);
			//light_bxdf_pdf = abs(p1->normal.dot(p2light.normalized())) / (2 * PI);

			L = {
				L_light.x() * F_light.x(),
				L_light.y() * F_light.y(),
				L_light.z() * F_light.z()
			};

			Vector3f p2light = lightShadowInteraction->p - p1->p;
			L *= abs(p1->normal.dot(p2light.normalized()));

			L /= light_pdf;

			// selectLightPDF = 1 / lightsNum
			L *= scene->aggregation->lights.size();
		}
		delete lightShadowRay;
		delete lightShadowInteraction;
		return L;
	}
}