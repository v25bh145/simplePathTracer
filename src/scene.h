//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_SCENE_H
#define SIMPLEPATHTRACER_SCENE_H

#include "default.h"
#include "ray.h"
#include "interaction.h"
#include "aggregation.h"

namespace pathTracer {
    class Ray;
    class Interaction;
    class Aggregation;

    class Scene {
    public:
        Scene(Aggregation *aggregation):aggregation(aggregation)  {
            RTCInnerScene = new RTCScene[1];
            *RTCInnerScene = rtcNewScene(DEVICE);
        }
        RTCScene *RTCInnerScene;
        Aggregation* aggregation;
        unsigned intersect(Interaction *interaction); //return the Geometry ID & update the interaction's info
        void commit();
    };
}


#endif //SIMPLEPATHTRACER_SCENE_H
