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
        PathIntegrator() { maxDepth = 0; }
        PathIntegrator(int maxDepth):maxDepth(maxDepth){}
        Vector3f sample_li(Scene *scene, Ray *origin_ray) override;
        void deepCopy(Integrator*& integrator) override;
        string toString() override;
    };
}

#endif //SIMPLEPATHTRACER_PATHINTEGRATOR_H
