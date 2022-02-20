//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_RAY_H
#define SIMPLEPATHTRACER_RAY_H

#include "default.h"
#include "mathHelper.h"

namespace pathTracer {
    class Ray {
    public:
        Ray() {
            origin = {0, 0, 0};
            direction = {0, 0, 0};
            t = tMin = 0;
            tMax = numeric_limits<float>::infinity();
        };
        Ray(Ray* ray) {
            origin = ray->origin;
            direction = ray->direction;
            t = ray->t;
            tMin = ray->tMin;
            tMax = ray->tMax;
        }
        Ray(Vector3f origin, Vector3f direction, float t, float tMin = 0, float tMax = numeric_limits<float>::infinity()):origin(origin),direction(direction.normalized()),t(t),tMin(tMin),tMax(tMax) {
            //cout << toString() << endl;
            if (tMin > tMax) {
                cout << "tMin > tMax while constructing Ray()" << endl;
                assert(false);
            }
        };
        Ray(Vector3f origin, Vector3f destination) {
            this->origin = origin;
            this->direction = destination - origin;
            this->t  = 0;
            //this->tMax = this->direction.norm();
            tMax = numeric_limits<float>::infinity();
            this->tMin = 0;
            this->direction.normalize();
        }

        Vector3f origin;
        Vector3f direction;
        float t;
        float tMin;
        float tMax;
        RTCRay getRTCInnerRay();
        // 将RTCRay存储的信息更新到Ray中
        void updateRay(RTCRay RTCInnerRay);

        string toString();
    };
}


#endif //SIMPLEPATHTRACER_RAY_H
