//
// Created by v25bh on 2021/11/21.
//
// libs in extensions/ and namespace

#ifndef SIMPLEPATHTRACER_DEFAULT_H
#define SIMPLEPATHTRACER_DEFAULT_H

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <windows.h>
using namespace std;

// core of ray tracing
//#include "../extensions/Embree3/include/embree3/rtcore.h"
#include <embree3/rtcore.h>
// algebra
//#include "../extensions/Eigen3/include/eigen3/Eigen/Dense"
#include <Eigen/Dense>
using namespace Eigen;
// pixels 2 PNG
//#include "../extensions/libpng/include/png.h"
#include <png.h>

namespace pathTracer {
    const RTCDevice DEVICE = rtcNewDevice(NULL);
    const float PI = (float)acos(-1.0);
    const int THREAD_N = 16;
}

#endif //SIMPLEPATHTRACER_DEFAULT_H
