//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_AGGREGATION_H
#define SIMPLEPATHTRACER_AGGREGATION_H

#include "default.h"
#include "geometry.h"
#include "scene.h"
#include "sampling.h"

namespace pathTracer {

    class Scene;
    class Geometry;
    class RandomGenerator;

    class Aggregation {
    public:
        vector<Geometry*> geometries = vector<Geometry*>(0);
        // 几何体其中的灯光
        vector<Geometry*> lights;
        Aggregation();
        void push_back(Geometry* geometry);
        Geometry *findGeometryById(unsigned id);
        void attachAllGeometriesToScene(Scene *scene);

        Geometry *uniformRandomLight() {
            RandomGenerator randomGenerator;
            int num = randomGenerator.uniformNToM(0, lights.size() - 1);
            return lights[num];
        }
    };
}

#endif //SIMPLEPATHTRACER_AGGREGATION_H
