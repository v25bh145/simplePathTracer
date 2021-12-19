//
// Created by v25bh on 2021/11/23.
//

#include "rectangle.h"

namespace pathTracer {
    Vector3f Rectangle::sample_point(float &pdf) {
        RandomGenerator randomGenerator;

        int n = randomGenerator.uniformNToM(0, 5);

        Vector3f point = quads[n]->sample_point(pdf);

        pdf /= 6;

        return point;
    }

    Vector3f Rectangle::le(Interaction* p, float& pdf)
    {
        pdf = 1 / area();
        return emitLight;
    }

    float Rectangle::area() {
        float subQuadsAreaSum = 0;
        for(auto quad : quads)
            subQuadsAreaSum += quad->area();
        return subQuadsAreaSum;
    }

    unsigned Rectangle::getRTCInnerGeometryId()
    {
        RandomGenerator randomGenerator;

        int n = randomGenerator.uniformNToM(0, 5);

        return quads[n]->getRTCInnerGeometryId();
    }

    //强制保存几何体的缓存
    void Rectangle::loadRealGeometryFlush() {
        for(auto quad : quads)
            quad->loadRealGeometryFlush();
    }

    // 查询子节点是否有给定的ID
    Geometry* Rectangle::hasSubGeometriesId(unsigned int id) {
        for(auto quad : quads)
            if(quad->hasSubGeometriesId(id) != nullptr) return (Geometry*)quad;
        return nullptr;
    }

    void Rectangle::attachAllGeometriesToScene(Scene scene) {
        for(auto quad : quads)
            quad->attachAllGeometriesToScene(scene);
    }
}