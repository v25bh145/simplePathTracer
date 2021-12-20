//
// Created by v25bh on 2021/11/27.
//

#include "quad.h"

namespace pathTracer {
    void pathTracer::Quad::loadRealGeometryFlush() {
        rtcCommitGeometry(*RTCInnerGeometry);
    }

    Geometry *Quad::hasSubGeometriesId(unsigned int id) {
        if(RTCInnerGeometryId == id) return this;
        else return nullptr;
    }

    Vector3f Quad::sample_point(float& pdf) {
        float e1 = (p1 - p2).norm(),
            e2 = (p2 - p3).norm(),
            e3 = (p1 - p3).norm(),
            e4 = (p3 - p4).norm(),
            e5 = (p1 - p4).norm();
        float pt1 = (e1 + e2 + e3) / 2.f, pt2 = (e3 + e4 + e5) / 2.f;
        float s1 = sqrt(pt1 * (pt1 - e1) * (pt1 - e2) * (pt1 - e3)),
            s2 = sqrt(pt2 * (pt2 - e3) * (pt2 - e4) * (pt2 - e5));
        float s = s1 + s2;

        pdf = 1 / area();

        RandomGenerator randomGenerator;
        float q = randomGenerator.uniform0To1(), 
              u = randomGenerator.uniform0To1(), 
              v = randomGenerator.uniform0To1();
        if (u + v > 1.f) {
            u = 1 - u;
            v = 1 - v;
        }
        
        Vector3f point;
        float s1_perc = s1 / s;
        if (q > s1_perc) {
            // sample s2
            point = u * p1 + v * p4 + (1.f - u - v) * p3;
        }
        else {
            // sample s1
            point = u * p1 + v * p2 + (1.f - u - v) * p3;
        }
        // trilerp interpolation
        return point;
    }

    Vector3f Quad::le(Interaction* p, float& pdf)
    {
        Vector3f p2light = p->p - p->ray->origin;
        if (singleSide && p2light.dot(outsideNormal) > 0) {
            pdf = 0;
            return { 0, 0, 0 };
        }
        float dis = p2light.norm();
        float abscostheta = abs(p->normal.dot(p2light.normalized()));
        pdf = dis * dis / (abscostheta * area());
        return emitLight;
    }

    float Quad::area() {
        float e1 = (p1 - p2).norm(), 
              e2 = (p2 - p3).norm(), 
              e3 = (p1 - p3).norm(), 
              e4 = (p3 - p4).norm(), 
              e5 = (p1 - p4).norm();
        float pt1 = (e1 + e2 + e3) / 2.f, pt2 = (e3 + e4 + e5) / 2.f;
        return sqrt(pt1 * (pt1 - e1) * (pt1 - e2) * (pt1 - e3))
             + sqrt(pt2 * (pt2 - e3) * (pt2 - e4) * (pt2 - e5));
    }

    void Quad::attachAllGeometriesToScene(Scene scene) {
        cout<<"attach geometry to scene"<<endl;
        rtcAttachGeometryByID(*scene.RTCInnerScene, *RTCInnerGeometry, RTCInnerGeometryId);
    }

    unsigned Quad::getRTCInnerGeometryId()
    {
        return this->RTCInnerGeometryId;
    }

    RTCGeometry* Quad::getRTCInnerGeometry()
    {
        return this->RTCInnerGeometry;
    }

    Vector3f Quad::getOutsideNormal()
    {
        if (vector3fEqualTo0(outsideNormal)) {
            cout << "fatal: getOutsideNormal() by a simple geometry without setting outsideNormal" << endl;
            assert(false);
        }
        return outsideNormal;
    }

    void Quad::deepCopy(Geometry*& geometry)
    {
        geometry = new Quad();
        Quad* quad = (Quad*)geometry;
        quad->p1 = this->p1;
        quad->p2 = this->p2;
        quad->p3 = this->p3;
        quad->p4 = this->p4;
        quad->emitLight = this->emitLight;
        quad->outsideNormal = this->outsideNormal;
        quad->singleSide = this->singleSide;
        quad->bxdf = nullptr;
        this->bxdf->deepCopy(quad->bxdf);
        quad->RTCInnerGeometry = this->RTCInnerGeometry;
        quad->RTCInnerGeometryId = this->RTCInnerGeometryId;
        quad->insideMedia = quad->outsideMedia = nullptr;
        if(insideMedia != nullptr)
            this->insideMedia->deepCopy(insideMedia);
        if (outsideMedia != nullptr)
            this->outsideMedia->deepCopy(outsideMedia);
    }

    Media* Quad::getOutsideMedia()
    {
        return this->outsideMedia;
    }

    Media* Quad::getInsideMedia()
    {
        return this->insideMedia;
    }

    //void Quad::deepCopy(Quad* quad)
    //{
    //    quad->vertices = this->vertices;
    //    quad->indices = this->indices;
    //    quad->p1 = this->p1; quad->p2 = this->p2; quad->p3 = this->p3; quad->p4 = this->p4;
    //    quad->outsideNormal = this->outsideNormal;
    //    quad->singleSide = this->singleSide;

    //    Geometry* thisGeometry = (Geometry*)this;
    //    thisGeometry->deepCopy(quad);
    //}

}