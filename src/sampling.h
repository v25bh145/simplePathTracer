//
// Created by v25bh on 2021/12/2.
//

#ifndef SIMPLEPATHTRACER_SAMPLING_H
#define SIMPLEPATHTRACER_SAMPLING_H

#include "default.h"

namespace pathTracer {
#define DEFAULT_ACCURACY 10000.f
    class RandomGenerator {
    public:
        random_device rd;
        default_random_engine gen;
        uniform_int_distribution<int> dis;

        RandomGenerator() {
            gen = default_random_engine(rd());
        }

        float uniform0To1();
        int uniformNToM(int n, int m);
    };
}

#endif //SIMPLEPATHTRACER_SAMPLING_H
