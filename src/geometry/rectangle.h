//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_RECTANGLE_H
#define SIMPLEPATHTRACER_RECTANGLE_H

#include "../default.h"
#include "../geometry.h"
#include "quad.h"

namespace pathTracer {
    class Quad;

    class Rectangle : public Geometry {
    public:
        Rectangle(Vector3f p1, Vector3f p2, BxDF* bxdf, Vector3f emitLight): Geometry(bxdf, emitLight, nullptr) {
            pMin = getMin(p1, p2);
            pMax = getMax(p1, p2);
            quads.push_back(new Quad(
                    {pMin.x(), pMin.y(), pMin.z()},
                    {pMin.x(), pMax.y(), pMin.z()},
                    {pMax.x(), pMax.y(), pMin.z()},
                    {pMax.x(), pMin.y(), pMin.z()},
                    this->bxdf, this->emitLight
            ));
            quads.push_back(new Quad(
                    {pMin.x(), pMin.y(), pMin.z()},
                    {pMin.x(), pMax.y(), pMin.z()},
                    {pMin.x(), pMax.y(), pMax.z()},
                    {pMin.x(), pMin.y(), pMax.z()},
                    this->bxdf, this->emitLight
            ));
            quads.push_back(new Quad(
                    {pMin.x(), pMin.y(), pMin.z()},
                    {pMin.x(), pMin.y(), pMax.z()},
                    {pMax.x(), pMin.y(), pMax.z()},
                    {pMin.x(), pMin.y(), pMax.z()},
                    this->bxdf, this->emitLight
            ));
            quads.push_back(new Quad(
                    {pMin.x(), pMax.y(), pMin.z()},
                    {pMax.x(), pMax.y(), pMin.z()},
                    {pMax.x(), pMax.y(), pMax.z()},
                    {pMin.x(), pMax.y(), pMax.z()},
                    this->bxdf, this->emitLight
            ));
            quads.push_back(new Quad(
                    {pMax.x(), pMin.y(), pMin.z()},
                    {pMax.x(), pMax.y(), pMin.z()},
                    {pMax.x(), pMax.y(), pMax.z()},
                    {pMax.x(), pMin.y(), pMax.z()},
                    this->bxdf, this->emitLight
            ));
            quads.push_back(new Quad(
                    {pMin.x(), pMin.y(), pMax.z()},
                    {pMin.x(), pMax.y(), pMax.z()},
                    {pMax.x(), pMax.y(), pMax.z()},
                    {pMax.x(), pMin.y(), pMax.z()},
                    this->bxdf, this->emitLight
            ));
        }

        // A-B-C-D-E-F
        vector<Quad*> quads;
        Vector3f pMax, pMin;

        Geometry* hasSubGeometriesId(unsigned int id) override;
        void loadRealGeometryFlush() override;
        void attachAllGeometriesToScene(Scene scene) override;

        Vector3f sample_point(float &pdf) override;
        Vector3f le(Interaction* p, float& pdf) override;

        float area() override;

        unsigned getRTCInnerGeometryId() override;
    };
}


#endif //SIMPLEPATHTRACER_RECTANGLE_H
