//
// Created by v25bh on 2021/11/28.
//

// test the geometries creation [simulate with no obj parser]
#include <iostream>
#include "../../src/scene.h"
#include "../../src/geometry/rectangle.h"

using namespace std;
using namespace pathTracer;

int main(int argc, char** argv){
    /*
     * information for obj & mtl
     */
    Vector3f ka, kd, ks;
    BxDF *bxdf = nullptr;
    Quad *quad1 = new Quad({0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, (BxDF*)bxdf, {0, 0, 0});
    Quad *quad2 = new Quad({0, 0, 1}, {0, 1, 1}, {1, 1, 1}, {1, 0, 1}, (BxDF*)bxdf, {0, 0, 0});
    Quad *quad3 = new Quad({0, 0, 2}, {0, 1, 2}, {1, 1, 2}, {1, 0, 2}, (BxDF*)bxdf, {0, 0, 0});

    cout<<"quad1 id"<<quad1->RTCInnerGeometryId<<endl;
    cout<<"quad2 id"<<quad2->RTCInnerGeometryId<<endl;
    cout<<"quad3 id"<<quad3->RTCInnerGeometryId<<endl;

    RTCScene innerScene = rtcNewScene(DEVICE);
    rtcAttachGeometryByID(innerScene, *quad1->RTCInnerGeometry, quad1->RTCInnerGeometryId);
    rtcAttachGeometryByID(innerScene, *quad2->RTCInnerGeometry, quad2->RTCInnerGeometryId);
    rtcAttachGeometryByID(innerScene, *quad3->RTCInnerGeometry, quad3->RTCInnerGeometryId);
    rtcCommitScene(innerScene);

    RTCRayHit rayHit;
    rayHit.ray.org_x = 1.5;
    rayHit.ray.org_y = 1.5;
    rayHit.ray.org_z = 0.5;
    rayHit.ray.dir_x = 0;
    rayHit.ray.dir_y = 0;
    rayHit.ray.dir_z = 1;
    rayHit.ray.time = 0;
    rayHit.ray.tnear = 0; rayHit.ray.tfar = numeric_limits<float>::infinity();
    rayHit.ray.mask = 0;
    rayHit.ray.flags = 0;
    rayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    rayHit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

    struct RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    rtcIntersect1(innerScene, &context, &rayHit);

    if(rayHit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
        cout<<"Hit the geometry which id is: "<<rayHit.hit.geomID<<endl;
        cout<<"Hit information: "<<endl;
        cout<<"time="<<rayHit.ray.time<<endl;
        cout<<"(tFar="<<rayHit.ray.tfar<<", tNear="<<rayHit.ray.tnear<<")"<<endl;
        cout<<"n("<<rayHit.hit.Ng_x<<", "<<rayHit.hit.Ng_y<<", "<<rayHit.hit.Ng_z<<")"<<endl;
        cout<<"(u="<<rayHit.hit.u<<", v="<<rayHit.hit.v<<")"<<endl;
    } else
        cout<<"Didn't find any hit"<<endl;
}