//
// Created by v25bh on 2021/11/27.
//

#ifndef SIMPLEPATHTRACER_QUAD_H
#define SIMPLEPATHTRACER_QUAD_H

#include "../geometry.h"

namespace pathTracer {
    class Quad : public Geometry {
    public:
        // default will not create new RTCInnerGeometry
        Quad(){}
        Quad(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4, BxDF* bxdf, Vector3f emitLight, Vector3f outsideNormal = { 0, 0, 0 }, bool singleSide = false, Media* outsideMedia = nullptr, Media* insideMedia = nullptr) :p1(p1), p2(p2), p3(p3), p4(p4), outsideNormal(outsideNormal.normalized()), singleSide(singleSide), Geometry(bxdf, emitLight), outsideMedia(outsideMedia), insideMedia(insideMedia) {
            if (vector3fEqualTo0(outsideNormal)) {
                if (bxdf->hasType(BxDFType::TRANSMISSION)) {
                    cout << "must set the outsideNormal when having set TRANSMISSION bxdf" << endl;
                    assert(false);
                }
                if (insideMedia != nullptr || outsideMedia != nullptr) {
                    cout << "must set the outsideNormal when having set media" << endl;
                    assert(false);
                }
            }

            RTCInnerGeometryId = ++Geometry::RTCInnerObjNumber;
            RTCInnerGeometry = new RTCGeometry[1];
            *RTCInnerGeometry = rtcNewGeometry(DEVICE, RTC_GEOMETRY_TYPE_QUAD);
            float *vertices = (float*) rtcSetNewGeometryBuffer(*RTCInnerGeometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3*sizeof(float), 4);
            vertices[0] = p1.x(); vertices[1] = p1.y(); vertices[2] = p1.z();
            vertices[3] = p2.x(); vertices[4] = p2.y(); vertices[5] = p2.z();
            vertices[6] = p3.x(); vertices[7] = p3.y(); vertices[8] = p3.z();
            vertices[9] = p4.x(); vertices[10] = p4.y(); vertices[11] = p4.z();
            unsigned *indices = (unsigned*) rtcSetNewGeometryBuffer(*RTCInnerGeometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT4, 4*sizeof(unsigned), 1);
            indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 3;
            rtcCommitGeometry(*RTCInnerGeometry);
        }
        RTCGeometry* RTCInnerGeometry;
        unsigned RTCInnerGeometryId;
        float* vertices;
        unsigned* indices;
        Vector3f p1, p2, p3, p4;

        Vector3f outsideNormal;
        bool singleSide;

        Media* outsideMedia;
        Media* insideMedia;

        Geometry *hasSubGeometriesId(unsigned int id) override;

        Vector3f sample_point(float &pdf) override;
        Vector3f le(Interaction* p, float& pdf) override;

        float area() override;

        void loadRealGeometryFlush() override;
        void attachAllGeometriesToScene(Scene scene) override;

        unsigned getRTCInnerGeometryId() override;
        RTCGeometry* getRTCInnerGeometry() override;

        Vector3f getOutsideNormal() override;

        void deepCopy(Geometry*& geometry) override;

        Media* getOutsideMedia();
        Media* getInsideMedia();
    };
}

#endif //SIMPLEPATHTRACER_QUAD_H
