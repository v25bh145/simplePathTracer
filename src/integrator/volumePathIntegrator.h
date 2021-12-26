//
// Created by v25bh on 2021/12/20.
//

#ifndef SIMPLEPATHTRACER_VOLUMEPATHINTEGRATOR
#define SIMPLEPATHTRACER_VOLUMEPATHINTEGRATOR

#include "../integrator.h"

namespace pathTracer {
    class VolumePathIntegrator : public Integrator {
    public:
        int maxDepth;
        VolumePathIntegrator() { maxDepth = 0;}
        VolumePathIntegrator(int maxDepth) :maxDepth(maxDepth) {}
        Vector3f sample_li(Scene* scene, Ray* origin_ray) override;
        void deepCopy(Integrator*& integrator) override;
        string toString() override;
    };
    Vector3f estimateVolumeDirect(Interaction* p1, Geometry* p1Geometry, Scene* scene, Medium* p1Medium);
    Vector3f volumeVisibility(Vector3f origin, Medium* p1Medium, Vector3f destination, Interaction* lightInteraction, Scene* scene);
}

#endif