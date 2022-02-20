//
// Created by v25bh on 2021/11/23.
//

#include "ray.h"

namespace pathTracer {
    void Ray::updateRay(RTCRay RTCInnerRay) {
        origin = {RTCInnerRay.org_x, RTCInnerRay.org_y, RTCInnerRay.org_z};
        direction = {RTCInnerRay.dir_x, RTCInnerRay.dir_y, RTCInnerRay.dir_z};
        t = RTCInnerRay.time;
        tMin = RTCInnerRay.tnear;
        tMax = RTCInnerRay.tfar;
    }
	RTCRay Ray::getRTCInnerRay() {
        RTCRay RTCInnerRay;
        //cout << origin.x() << endl;
        RTCInnerRay.org_x = origin.x();
        RTCInnerRay.org_y = origin.y();
        RTCInnerRay.org_z = origin.z();
        RTCInnerRay.dir_x = direction.x();
        RTCInnerRay.dir_y = direction.y();
        RTCInnerRay.dir_z = direction.z();
        RTCInnerRay.time = t;
        RTCInnerRay.tnear = tMin; RTCInnerRay.tfar = tMax;
        RTCInnerRay.mask = 0;
        RTCInnerRay.flags = 0;
		return RTCInnerRay;
	}
    string Ray::toString() {
        string info = "";
        ostringstream buffer(info);
        buffer << "Ray information: ";
        buffer << "origin "<< vector3fToString(origin)<<", ";
        buffer << "dir "<< vector3fToString(direction)<<", ";
        buffer << "tMin="; 
        buffer << tMin; 
        buffer << ", tMax="; 
        buffer << tMax;
        return buffer.str();
    }
}
