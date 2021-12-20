//
// Created by v25bh on 2021/12/2.
//

#include "sampling.h"

namespace pathTracer {

    float RandomGenerator::uniform0To1() {
        dis = uniform_int_distribution<int>(0, DEFAULT_ACCURACY);
        return (float)dis(gen) / DEFAULT_ACCURACY;
    }
    // [n, m]
    int RandomGenerator::uniformNToM(int n, int m) {
        if(n > m)   swap(n, m);
        dis = uniform_int_distribution<int>(n, m);
        return dis(gen);
    }

}