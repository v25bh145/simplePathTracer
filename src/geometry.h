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
#include "medium.h"
#include "texture.h"

namespace pathTracer {

    class Interaction;
    class Ray;
    class BxDF;
    class Scene;
    class Medium;

    class Geometry {
    //protect:

    public:
//        default: store the world coordination
//        Matrix4f obj2World;

        BxDF* bxdf;

        //tips: emitLight = ka
        Vector3f emitLight;

        static unsigned RTCInnerObjNumber;

        //Geometry(BxDF* bxdf, Vector3f emitLight = {0, 0, 0}):bxdf(bxdf), emitLight(emitLight) {}
        Geometry(BxDF* bxdf, Vector3f emitLight = { 0, 0, 0 })
            :bxdf(bxdf), emitLight(emitLight) {}
        Geometry() {
            bxdf = nullptr;
        };
        // light(for sampling)
        virtual Vector3f sample_point(float& pdf) = 0;
        virtual Vector3f le(Interaction* p1, Interaction* pLight, float& wi_pdf) = 0;
        virtual float area() = 0;
        // base class
        virtual Geometry* hasSubGeometriesId(unsigned id) = 0;
        // RTC core
        virtual void attachAllGeometriesToScene(Scene scene) = 0;
        virtual void loadRealGeometryFlush() = 0;
        virtual unsigned getRTCInnerGeometryId() = 0;
        virtual RTCGeometry* getRTCInnerGeometry() = 0;
        // light & medium
        virtual Vector3f getOutsideNormal() = 0;
        // medium(geometry's child class which can't implement this will return nullptr)
        virtual Medium* getOutsideMedium() = 0;
        virtual Medium* getInsideMedium() = 0;
        // texture
        virtual void attachTexture(Texture2D* texture, vector<Vector2f> uvArray) = 0;
        virtual Vector2f getUV(Vector3f p) = 0;
        virtual Texture2D* getTexture() = 0;
        // object
        virtual void deepCopy(Geometry*& geometry) = 0;
        virtual string toString() = 0;
    };

    Vector3f getMax(Vector3f p1, Vector3f p2);

    Vector3f getMin(Vector3f p1, Vector3f p2);
}


#endif //SIMPLEPATHTRACER_GEOMOTRY_H
