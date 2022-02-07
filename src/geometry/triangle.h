//
// Created by v25bh on 2022/02/01.
//

#ifndef SIMPLEPATHTRACER_TRIANGLE_H
#define SIMPLEPATHTRACER_TRIANGLE_H

#include "../geometry.h"

namespace pathTracer {
    class Triangle : public Geometry {
    public:
        // default will not create new RTCInnerGeometry
        Triangle() {
        }
        Triangle(Vector3f p1, Vector3f p2, Vector3f p3, BxDF* bxdf, Vector3f emitLight, Vector3f outsideNormal = { 0, 0, 0 }, bool singleSide = false, Medium* outsideMedium = nullptr, Medium* insideMedium = nullptr)
            :p1(p1), p2(p2), p3(p3)
            , outsideNormal(outsideNormal.normalized()), singleSide(singleSide)
            , Geometry(bxdf, emitLight), outsideMedium(outsideMedium), insideMedium(insideMedium) {
            if (vector3fEqualTo0(outsideNormal)) {
                // ccw
                
                //if (bxdf != nullptr && bxdf->hasType(BxDFType::TRANSMISSION)) {
                //    cout << "must set the outsideNormal when having set TRANSMISSION bxdf" << endl;
                //    assert(false);
                //}
                //if (insideMedium != nullptr || outsideMedium != nullptr) {
                //    cout << "must set the outsideNormal when having set medium" << endl;
                //    assert(false);
                //}
            }
            if (bxdf != nullptr && !bxdf->hasType(BxDFType::TRANSMISSION) && (insideMedium != nullptr || outsideMedium != nullptr)) {
                cout << "must set the TRANSMISSION bxdf when having set medium" << endl;
                assert(false);
            }
            RTCInnerGeometryId = ++Geometry::RTCInnerObjNumber;
            RTCInnerGeometry = new RTCGeometry[1];
            *RTCInnerGeometry = rtcNewGeometry(DEVICE, RTC_GEOMETRY_TYPE_TRIANGLE);
            float* vertices = (float*)rtcSetNewGeometryBuffer(*RTCInnerGeometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), 3);
            vertices[0] = p1.x(); vertices[1] = p1.y(); vertices[2] = p1.z();
            vertices[3] = p2.x(); vertices[4] = p2.y(); vertices[5] = p2.z();
            vertices[6] = p3.x(); vertices[7] = p3.y(); vertices[8] = p3.z();
            unsigned* indices = (unsigned*)rtcSetNewGeometryBuffer(*RTCInnerGeometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), 1);
            indices[0] = 0; indices[1] = 1; indices[2] = 2;
            rtcCommitGeometry(*RTCInnerGeometry);
            cout << "id=" << this->RTCInnerGeometryId << endl;
            if (this->insideMedium)
                cout << "insideMedium" << this->insideMedium->toString() << endl;
            if (this->outsideMedium)
                cout << "outsideMedium" << this->outsideMedium->toString() << endl;
        }
        RTCGeometry* RTCInnerGeometry;
        unsigned RTCInnerGeometryId;
        float* vertices;
        unsigned* indices;
        Vector3f p1, p2, p3;

        Vector3f outsideNormal;
        bool singleSide;

        Medium* outsideMedium;
        Medium* insideMedium;

        Geometry* hasSubGeometriesId(unsigned int id) override;

        Vector3f sample_point(float& pdf) override;
        Vector3f le(Interaction* p1, Interaction* pLight, float& wi_pdf) override;

        float area() override;

        void loadRealGeometryFlush() override;
        void attachAllGeometriesToScene(Scene scene) override;

        unsigned getRTCInnerGeometryId() override;
        RTCGeometry* getRTCInnerGeometry() override;

        Vector3f getOutsideNormal() override;

        void deepCopy(Geometry*& geometry) override;

        Medium* getOutsideMedium() override;
        Medium* getInsideMedium() override;

        // TODO
        //void attachTexture(Texture* texture, vector<float,float> uvArray);

        string toString() override;
    };
}

#endif //SIMPLEPATHTRACER_QUAD_H
