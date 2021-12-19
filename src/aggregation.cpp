//
// Created by v25bh on 2021/11/23.
//

#include "aggregation.h"

namespace pathTracer {
    Aggregation::Aggregation() {
    }

    void Aggregation::push_back(Geometry *geometry) {
        geometries.push_back(geometry);
        if(geometry->emitLight.x() > 0 || geometry->emitLight.y() > 0 || geometry->emitLight.z() > 0)
            lights.push_back(geometry);
    }

    Geometry *Aggregation::findGeometryById(unsigned int id) {
        if (id <= 0 || id == RTC_INVALID_GEOMETRY_ID) return nullptr;
        for(auto geometry : geometries) {
            Geometry *findResult = geometry->hasSubGeometriesId(id);
            if(findResult != nullptr) return findResult;
        }
        return nullptr;
    }

    void Aggregation::attachAllGeometriesToScene(Scene *scene) {
        for(auto geometry : geometries) {
            geometry->attachAllGeometriesToScene(*scene);
        }
    }
}
