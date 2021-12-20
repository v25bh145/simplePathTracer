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
        int sampleOnePixel;
        VolumePathIntegrator() { maxDepth = 0; sampleOnePixel = 1; }
        VolumePathIntegrator(int maxDepth, int sampleOnePixel) :maxDepth(maxDepth), sampleOnePixel(sampleOnePixel > 1 ? sampleOnePixel : 1) {}
        Vector3f sample_li(Scene* scene, Ray* origin_ray) override;
        void deepCopy(Integrator*& integrator) override;
        string toString() override;
    };
    Vector3f estimateVolumeDirect(Interaction* interaction, Scene* scene);
}

#endif