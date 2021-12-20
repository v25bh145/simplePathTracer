//
// Created by v25bh on 2021/12/1.
//

#ifndef SIMPLEPATHTRACER_INTEGRATOR_H
#define SIMPLEPATHTRACER_INTEGRATOR_H

#include "default.h"
#include "ray.h"
#include "scene.h"
#include "mathHelper.h"

namespace pathTracer {
    class Scene;
    class Ray;
    class Integrator {
    public:
        Integrator() = default;
        virtual Vector3f sample_li(Scene *scene, Ray *origin_ray) = 0;
        virtual void deepCopy(Integrator*& integrator) = 0;
        virtual string toString() = 0;
    };
    // wi = nipi/(nipi+njpj)
    float equilibriumHeuristic(int ni, float pi, int nj, float pj);
    float powerHeuristic(int ni, float pi, int nj, float pj, int beta);
    Vector3f estimateDirect(Interaction* p1, Geometry* p1Geometry, Scene* scene);
}

#endif //SIMPLEPATHTRACER_INTEGRATOR_H
