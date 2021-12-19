//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_GEOMOTRY_H
#define SIMPLEPATHTRACER_GEOMOTRY_H

#include "scene.h"
#include "default.h"
#include "interaction.h"
#include "ray.h"
#include "bxdf.h"
#include "sampling.h"
#define FATHER_GEOMETRY_ID -5

namespace pathTracer {

    class Interaction;
    class Ray;
    class BxDF;
    class Scene;

    class Geometry {
    //protect:

    public:
//        default: store the world coordination
//        Matrix4f obj2World;

        BxDF* bxdf;

        //tips: emitLight = ka
        Vector3f emitLight;

        // null if collection
        RTCGeometry* RTCInnerGeometry;
        // -1 if collection
        unsigned RTCInnerGeometryId;

        static unsigned RTCInnerObjNumber;

        //Geometry(BxDF* bxdf, Vector3f emitLight = {0, 0, 0}):bxdf(bxdf), emitLight(emitLight) {}
        Geometry(BxDF* bxdf, Vector3f emitLight = { 0, 0, 0 }, RTCGeometry* RTCInnerGeometry = nullptr, unsigned RTCInnerGeometryId = FATHER_GEOMETRY_ID)
            :bxdf(bxdf), emitLight(emitLight), RTCInnerGeometry(RTCInnerGeometry), RTCInnerGeometryId(RTCInnerGeometryId){}
        Geometry();

        virtual Vector3f sample_point(float& pdf) = 0;
        virtual Vector3f le(Interaction *p, float &pdf) = 0;

        virtual float area() = 0;

        virtual Geometry* hasSubGeometriesId(unsigned id) = 0;
        virtual void attachAllGeometriesToScene(Scene scene) = 0;

        virtual void loadRealGeometryFlush() = 0;

        virtual unsigned getRTCInnerGeometryId() = 0;
    };

    Vector3f getMax(Vector3f p1, Vector3f p2);

    Vector3f getMin(Vector3f p1, Vector3f p2);
}


#endif //SIMPLEPATHTRACER_GEOMOTRY_H
