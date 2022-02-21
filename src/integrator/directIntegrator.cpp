//
// Created by v25bh on 2021/12/16.
//

#include "directIntegrator.h"

namespace pathTracer {
	Vector3f DirectIntegrator::sample_li(Scene* scene, Ray* origin_ray)
	{
		//auto* hitInteraction = new Interaction(origin_ray);
		//unsigned hitGeomID = scene->intersect(hitInteraction);
		//Geometry* hitGeometry = hitInteraction->geometry;
		//delete hitInteraction;
		//if (hitGeomID != 8) return { 0, 0, 0 };
		Interaction* p = new Interaction(origin_ray);
		unsigned hitId = scene->intersect(p);
		if (hitId != 1 && hitId != 2) return { 0, 0, 0 };
		if (hitId > 0) {
			Geometry *hitGeometry = p->geometry;
			Vector3f L = estimateDirect(p, hitGeometry, scene);
			L += L + hitGeometry->emitLight;
			if (hitGeometry->getTexture() != nullptr) {
				Vector2f UV = hitGeometry->getUV(p->p);
				// input: dstdx, dstdy
				Vector4f color = hitGeometry->getTexture()->mapping(UV, );
				L = { L.x() * color.x(), L.y() * color.y(), L.z() * color.z() };
			}
			delete p;
			return L;
		}
		else { 
			delete p;
			return { 0, 0, 0 }; 
		}
	}
	void DirectIntegrator::deepCopy(Integrator*& integrator)
	{
		integrator = new DirectIntegrator();
		integrator->pixelSize = this->pixelSize;
	}
	string DirectIntegrator::toString()
	{
		string info = "";
		ostringstream buffer(info);
		buffer << "directIntegrator" << endl;
		return buffer.str();
	}
}