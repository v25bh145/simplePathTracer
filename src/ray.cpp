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
    void Ray::setDifferential(pair<Vector3f, Vector3f> rxy, pair<Vector3f, Vector3f> rdxy)
    {
        this->differential.hasDifferential = true;
        this->differential.rxOrigin = rxy.first;
        this->differential.ryOrigin = rxy.second;
        this->differential.rxDirection = rdxy.first;
        this->differential.ryDirection = rdxy.second;
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
        buffer << ", differential(";
        if (this->differential.hasDifferential)
            buffer << "True, Xorigin=" << vector3fToString(this->differential.rxOrigin) << ", Yorigin=" << vector3fToString(this->differential.ryOrigin) << ")";
        else buffer << "False)";
        buffer << endl;
        return buffer.str();
    }
}
