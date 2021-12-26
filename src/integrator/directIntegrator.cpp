//
// Created by v25bh on 2021/12/16.
//

#include "directIntegrator.h"

namespace pathTracer {
	Vector3f DirectIntegrator::sample_li(Scene* scene, Ray* origin_ray)
	{
		Interaction* p = new Interaction(origin_ray);
		unsigned hitId = scene->intersect(p);
		if (hitId > 0) {
			Geometry *hitGeometry = p->geometry;
			Vector3f L = estimateDirect(p, hitGeometry, scene);
			delete p;
			return L + hitGeometry->emitLight;
		}
		else { 
			delete p;
			return { 0, 0, 0 }; 
		}
	}
	void DirectIntegrator::deepCopy(Integrator*& integrator)
	{
		integrator = new DirectIntegrator();
	}
	string DirectIntegrator::toString()
	{
		string info = "";
		ostringstream buffer(info);
		buffer << "directIntegrator" << endl;
		return buffer.str();
	}
}