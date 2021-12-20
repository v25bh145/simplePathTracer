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
			Vector3f sum_L = { 0, 0, 0 };
			for (int i = 0; i < sampleOnePixel; ++i) {
				sum_L += estimateDirect(p, hitGeometry, scene);
			}
			delete p;
			return sum_L / sampleOnePixel + hitGeometry->emitLight;
		}
		else return { 0, 0, 0 };
	}
	void DirectIntegrator::deepCopy(Integrator*& integrator)
	{
		integrator = new DirectIntegrator(this->sampleOnePixel);
	}
	string DirectIntegrator::toString()
	{
		string info = "";
		ostringstream buffer(info);
		buffer << "directIntegrator: sampleOnePixel=" << sampleOnePixel << endl;
		return buffer.str();
	}
}