//
// Created by v25bh on 2021/11/24.
//
#include <embree3/rtcore.h>
#include <Eigen/Dense>
#include <iostream>

using namespace std;

int main() {
    RTCDevice device = rtcNewDevice(NULL);
    RTCScene scene = rtcNewScene(device);

//    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
//    float *vertices = (float*) rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3*sizeof(float), 3);
//    vertices[0] = 1; vertices[1] = 0; vertices[2] = 0;
//    vertices[3] = 0; vertices[4] = 2; vertices[5] = 0;
//    vertices[6] = 0; vertices[7] = 0; vertices[8] = 3;
//    unsigned *indices = (unsigned*) rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3*sizeof(unsigned), 1);
//    indices[0] = 0; indices[1] = 1; indices[2] = 2;

    RTCGeometry geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_QUAD);
    float *vertices = (float*) rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3*sizeof(float), 4);
    vertices[0] = 0.f; vertices[1] = 0.f; vertices[2] = 0.f;
    vertices[3] = 0.f; vertices[4] = 1.f; vertices[5] = 0.f;
    vertices[6] = 1.f; vertices[7] = 1.f; vertices[8] = 0.f;
    vertices[9] = 1.f; vertices[10] = 0.f; vertices[11] = 0.f;
//    vertices[18] = 1; vertices[19] = 0; vertices[20] = 1;
//    vertices[21] = 1; vertices[22] = 1; vertices[23] = 1;
    unsigned *indices = (unsigned*) rtcSetNewGeometryBuffer(geometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT4, 4*sizeof(unsigned), 1);
    indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 3;

    rtcCommitGeometry(geometry);
    rtcAttachGeometryByID(scene, geometry, 70);
    rtcCommitScene(scene);

    RTCRayHit rayHit;
    rayHit.ray.org_x = 0.5;
    rayHit.ray.org_y = 0.5;
    rayHit.ray.org_z = -1;
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

    rtcIntersect1(scene, &context, &rayHit);

    if(rayHit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
        cout<<"Hit the geometry which id is: "<<rayHit.hit.geomID<<endl;
        cout<<"Hit information: "<<endl;
        cout<<"time="<<rayHit.ray.time<<endl;
        cout<<"(tFar="<<rayHit.ray.tfar<<", tNear="<<rayHit.ray.tnear<<")"<<endl;
        cout<<"n("<<rayHit.hit.Ng_x<<", "<<rayHit.hit.Ng_y<<", "<<rayHit.hit.Ng_z<<")"<<endl;
        cout<<"(u="<<rayHit.hit.u<<", v="<<rayHit.hit.v<<")"<<endl;
    } else
        cout<<"Didn't find any hit"<<endl;

    rtcReleaseGeometry(geometry);
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
    return 0;
}