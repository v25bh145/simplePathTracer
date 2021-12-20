//
// Created by v25bh on 2021/11/23.
//

#include "aggregation.h"

namespace pathTracer {

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
    void Aggregation::deepCopy(Aggregation*& aggregation)
    {
        aggregation = new Aggregation();
        for (auto geometry : this->geometries) {
            Geometry* newGeometry = nullptr;
            geometry->deepCopy(newGeometry);
            aggregation->push_back(newGeometry);
        }
    }
}
