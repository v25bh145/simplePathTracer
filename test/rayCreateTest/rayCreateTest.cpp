//
// Created by v25bh on 2021/11/30.
//
// test the ray creation [simulate with no obj parser]
#include <iostream>
#include "../../src/scene.h"
#include "../../src/ray.h"
#include "../../src/geometry/rectangle.h"

using namespace std;
using namespace pathTracer;

int main(int argc, char** argv){
    /*
     * information for obj & mtl
     */
    Vector3f ka, kd, ks;
    BxDF *bxdf = nullptr;
    auto *rectangle_red = new Rectangle({0, 0, 0}, {1, 1, 1}, bxdf, {0, 0, 0});
    auto *rectangle_blue = new Rectangle({0, 0, 2}, {1, 1, 3}, bxdf, {0, 0, 0});

    auto *aggregation = new Aggregation();
    auto* scene = new Scene(aggregation);
    aggregation->push_back(rectangle_red);
    aggregation->push_back(rectangle_blue);
    scene->commit();

    Ray *ray = new Ray({0.5, 0.5, 2.5}, {0, 0, -1}, 0, 0, numeric_limits<float>::infinity());

    auto *interaction = new Interaction(ray);

    unsigned geomID = scene->intersect(interaction);

    if(geomID != RTC_INVALID_GEOMETRY_ID) {
        cout<<"Hit the geometry which id is: "<<geomID<<endl;
        cout<<"Hit information: "<<endl;
        cout<<"time="<<interaction->ray->t<<endl;
        cout<<"(tFar="<<interaction->ray->tMax<<", tNear="<<interaction->ray->tMin<<")"<<endl;
        cout<<"n("<<interaction->normal.x()<<", "<<interaction->normal.y()<<", "<<interaction->normal.z()<<")"<<endl;
        cout<<"(u="<<interaction->u<<", v="<<interaction->v<<")"<<endl;
        cout<<"Geometry information: "<<endl;
        Geometry *geometry = aggregation->findGeometryById(geomID);
        if(geometry == nullptr) cout<<"not find the geometry";
        else {

        }
    } else
        cout<<"Didn't find any hit"<<endl;
}