//
// Created by v25bh on 2021/11/21.
//

#ifndef SIMPLEPATHTRACER_APPLICATION_H
#define SIMPLEPATHTRACER_APPLICATION_H

#include "default.h"
#include "scene.h"
#include "bxdf/bsdf/lambertianReflection.h"
#include "bxdf/bsdf/specularTransmission.h"
#include "bxdf/bsdf/specularReflection.h"
#include "interaction.h"

#include "integrator/pathIntegrator.h"

//#include "integrator/directIntegrator.h"

#include "geometry/quad.h"
#include "camera.h"
#include "pngParser.h"

namespace pathTracer {
    class Application {
    public:
        Application() = default;
        int run(int argc, char **argv);
    };
}

#endif //SIMPLEPATHTRACER_APPLICATION_H
