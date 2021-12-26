//
// Created by v25bh on 2021/12/16.
//

#ifndef SIMPLEPATHTRACER_DIRECTINTEGRATOR_H
#define SIMPLEPATHTRACER_DIRECTINTEGRATOR_H

#include "../integrator.h"

namespace pathTracer {
    class DirectIntegrator : public Integrator {
    public:
        DirectIntegrator() {}
        Vector3f sample_li(Scene* scene, Ray* origin_ray) override;

        void deepCopy(Integrator*& integrator) override;

        string toString() override;
    };
}

#endif //SIMPLEPATHTRACER_PATHINTEGRATOR_H
