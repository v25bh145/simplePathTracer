//
// Created by v25bh on 2021/12/16.
//

#ifndef SIMPLEPATHTRACER_PATHINTEGRATOR_H
#define SIMPLEPATHTRACER_PATHINTEGRATOR_H

#include "../integrator.h"

namespace pathTracer {
    class DirectIntegrator : public Integrator {
    public:
        int sampleOnePixel;
        DirectIntegrator(int sampleOnePixel = 1) { 
            this->sampleOnePixel = sampleOnePixel < 0 ? 1 : sampleOnePixel;
        }
        Vector3f sample_li(Scene* scene, Ray* origin_ray);
    };
}

#endif //SIMPLEPATHTRACER_PATHINTEGRATOR_H
