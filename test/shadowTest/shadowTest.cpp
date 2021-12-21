//
// Created by v25bh on 2021/12/4.
//

// test the shadow ray [simulate with no obj parser]
#include <iostream>
#include "../../src/pathTracer.h"

using namespace std;
using namespace pathTracer;

int main(int argc, char** argv) {
    /*
     * information for obj & mtl
     */

//    Vector3f a = {1, 3, 5};
//    a.normalize();
//    Vector3f b = {5, 6, 7};
//    b.normalize();
//    Vector3f c = 2 * b * (b.dot(a)) - a;
//    cout<<"norm c"<<c.norm()<<endl;
//    c.normalize();
//    Vector3f d = a + c;
//    d.normalize();
//    cout<<"d"<<endl<<d<<endl;
//    cout<<"b"<<endl<<b<<endl;

    Vector3f ka, kd, ks;
    //BxDF *bxdf = nullptr;
    auto* fresnelSpecular = new FresnelSpecular({ 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, 1.f, 4.f / 3.f);

    //Quad *quad1 = new Quad({0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, (BxDF *) bxdf, {0, 0, 0});
    //Quad *quad2 = new Quad({0, 0, 1}, {0, 1, 1}, {1, 1, 1}, {1, 0, 1}, (BxDF *) bxdf, {0, 0, 0});
    //Quad *quad3 = new Quad({0, 0, 2}, {0, 1, 2}, {1, 1, 2}, {1, 0, 2}, (BxDF *) bxdf, {0, 0, 0});
    //Quad *quad_light = new Quad({0.4, 0.4, 1.99}, {0.4, 0.6, 1.99}, {0.6, 0.6, 1.99}, {0.6, 0.4, 1.99}, (BxDF *) bxdf, {0, 0, 0});
    //auto* tall_block_1 = new Quad({ 423, 247, 330 }, { 265, 296, 330 }, { 314, 456, 330 }, { 472, 406, 330 }, bxdf, { 0, 0, 0 });

    //cout<<"quad1 id"<<quad1->RTCInnerGeometryId<<endl;
    //cout<<"quad2 id"<<quad2->RTCInnerGeometryId<<endl;
    //cout<<"quad3 id"<<quad3->RTCInnerGeometryId<<endl;
    //cout<<"quad_light id"<<quad_light->RTCInnerGeometryId<<endl;
    auto* muddyMedium = new Medium({ 0.1f, 0.1f, 0.1f }, { 0.1f, 0.1f, 0.1f });
    Quad* quad4 = new Quad({ 0, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 }, { 1, 0, 0 }, fresnelSpecular, { 0, 0, 0 }, {0, 0, 1}, false, nullptr, muddyMedium);
    Quad* quad5 = new Quad({ 0, 0, 1 }, { 0, 1, 1 }, { 1, 1, 1 }, { 1, 0, 1 }, fresnelSpecular, { 0, 0, 0 }, {0, 0, -1}, false, nullptr, muddyMedium);
    Quad* quad6 = new Quad({ 0, 0, 2 }, { 0, 1, 2 }, { 1, 1, 2 }, { 1, 0, 2 }, fresnelSpecular, { 0, 0, 0 }, { 0, 0, 1 }, false, nullptr, muddyMedium);
    Quad* quad_light = new Quad({ 0, 0, 3 }, { 0, 1, 3 }, { 1, 1, 3 }, { 1, 0, 3 }, nullptr, { 500.f, 500.f, 500.f });

    auto* aggregation = new Aggregation();
    Scene *scene = new Scene(aggregation);
    //aggregation->push_back(tall_block_1);
    //aggregation->push_back(quad2);
    //aggregation->push_back(quad3);
    aggregation->push_back(quad4);
    aggregation->push_back(quad5);
    aggregation->push_back(quad6);
    aggregation->push_back(quad_light);
    scene->commit();

    //Vector3f point1 = quad2->sample_point();
    //Vector3f point_light = quad_light->sample_point();

    //cout<<"point1"<<endl;
    //cout<<point1<<endl;
    //cout<<"point_light"<<endl;
    //cout<<point_light<<endl;

//    Ray *ray = new Ray(point1, point_light);
    Vector3f Tr = volumeVisibility({ 0.5f, 0.5f, -1.f }, muddyMedium, { 0.5f, 0.5f, 3.f }, quad_light, scene);
    cout << vector3fToString(Tr) << endl;
    //cout<<"Ray"<<endl;
    //cout<<"origin: "<<ray->origin.x()<<" "<<ray->origin.y()<<" "<<ray->origin.z()<<endl;
    //cout<<"direction: "<<ray->direction.x()<<" "<<ray->direction.y()<<" "<<ray->direction.z()<<endl;

    //auto *interaction = new Interaction(ray);
    //unsigned geomID = scene->intersect(interaction);

    //if(geomID != RTC_INVALID_GEOMETRY_ID) {
    //    cout<<"Hit the geometry which id is: "<<geomID<<endl;
    //    cout<<"Hit information: "<<endl;
    //    cout<<"time="<<interaction->ray->t<<endl;
    //    cout<<"(tFar="<<interaction->ray->tMax<<", tNear="<<interaction->ray->tMin<<")"<<endl;
    //    cout<<"n("<<interaction->normal.x()<<", "<<interaction->normal.y()<<", "<<interaction->normal.z()<<")"<<endl;
    //    cout<<"(u="<<interaction->u<<", v="<<interaction->v<<")"<<endl;
    //    cout<<"Geometry information: "<<endl;
    //    Geometry *geometry = aggregation->findGeometryById(geomID);
    //    if(geometry == nullptr) cout<<"not find the geometry";
    //    else {

    //    }
    //} else
    //    cout<<"Didn't find any hit"<<endl;
}