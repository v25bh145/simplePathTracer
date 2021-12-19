//
// Created by v25bh on 2021/12/1.
//
// test the camera & integrator & PNGParser creation [simulate with no obj parser]
#include <iostream>
#include "../../src/scene.h"
#include "../../src/geometry/rectangle.h"
#include "../../src/integrator/pathIntegrator.h"
#include "../../src/pngParser.h"

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

    Vector2i resolution = {1960, 1280};
//    auto *camera = new Camera({0, 0, 0}, 0, PI / 3, 0, 1000, scene, resolution, new PathIntegrator());
//    camera->generate();

//    auto pngParser = new PngParser(camera->pixels, {resolution.x(), resolution.y()});
//    pngParser->writeToFile("sampleCoreTest.png");
}