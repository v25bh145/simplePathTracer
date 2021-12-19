//
// Created by v25bh on 2021/12/1.
//

#ifndef SIMPLEPATHTRACER_PATHINTEGRATOR_H
#define SIMPLEPATHTRACER_PATHINTEGRATOR_H

#include "../integrator.h"

namespace pathTracer {
    class PathIntegrator : public Integrator {
    public:
        int maxDepth;
        int sampleOnePixel;
        PathIntegrator() { maxDepth = 0; sampleOnePixel = 1; }
        PathIntegrator(int maxDepth, int sampleOnePixel):maxDepth(maxDepth), sampleOnePixel(sampleOnePixel > 1 ? sampleOnePixel : 1){}
        Vector3f sample_li(Scene *scene, Ray *origin_ray);
    };
}

#endif //SIMPLEPATHTRACER_PATHINTEGRATOR_H
