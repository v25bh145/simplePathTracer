//
// Created by v25bh on 2021/12/4.
//

// test the shadow ray [simulate with no obj parser]
#include <iostream>
#include "../../src/scene.h"
#include "../../src/geometry/quad.h"
#include "../../src/bxdf/bsdfs.h"
#include "../../src/bxdf/bsdf/lambertianReflection.h"
#include "../../src/bxdf/bsdf/specularReflection.h"

using namespace std;
using namespace pathTracer;

int main(int argc, char** argv) {
    /*
     * information for obj & mtl
     */

    // material creation
    Vector3f light_ka = {20, 20, 20};
    auto *red_diffuse = new LambertianReflection({1, 0, 0});
    auto *green_diffuse = new LambertianReflection({0, 1, 0});
    auto *blue_diffuse = new LambertianReflection({0, 0, 1});
    auto *white_diffuse = new LambertianReflection({0, 0, 0});
    auto *light_diffuse = new LambertianReflection({0, 0, 0});

    //geometry creation
    auto *floor = new Quad({550, 0, 0}, {0, 0, 0}, {0, 550, 0}, {550, 550, 0}, white_diffuse, {0, 0, 0});
    auto *light = new Quad({340, 230, 549.9}, {340, 330, 549.9}, {210, 330, 549.9}, {210, 230, 549.9}, light_diffuse, light_ka);
    auto *ceiling = new Quad({550, 0, 550}, {550, 550, 550}, {0, 550, 550}, {0, 0, 550}, white_diffuse, {0, 0, 0});
    auto *back_wall = new Quad({550, 550, 0}, {0, 550, 0}, {0, 550, 550}, {550, 550, 550}, white_diffuse, {0, 0, 0});
//    auto *front_wall = new Quad({550, 0, 0}, {0, 0, 0}, {0, 0, 550}, {550, 0, 550}, white_diffuse, {0, 0, 0});
    auto *green_wall = new Quad({0, 550, 0}, {0, 0, 0}, {0, 0, 550}, {0, 550, 550}, green_diffuse, {0, 0, 0});
    auto *red_wall = new Quad({550, 0, 0}, {550, 550, 0}, {550, 550, 550}, {550, 0, 550}, red_diffuse, {0, 0, 0});
    auto *short_block_1 = new Quad({130, 65, 165}, {82, 225, 165}, {240, 272, 165}, {290, 114, 165}, white_diffuse, {0, 0, 0});
    auto *short_block_2 = new Quad({290, 114, 0}, {290, 114, 165}, {240, 272, 165}, {240, 272, 0}, white_diffuse, {0, 0, 0});
    auto *short_block_3 = new Quad({130, 65, 0}, {130, 65, 165}, {290, 114, 165}, {290, 114, 0}, white_diffuse, {0, 0, 0});
    auto *short_block_4 = new Quad({82, 225, 0}, {82, 225, 165}, {130, 65, 165}, {130, 65, 0}, white_diffuse, {0, 0, 0});
    auto *short_block_5 = new Quad({240, 272, 0}, {240, 272, 165}, {82, 225, 165}, {82, 225, 0}, white_diffuse, {0, 0, 0});
    auto *tall_block_1 = new Quad({423, 247, 330}, {265, 296, 330}, {314, 456, 330}, {472, 406, 330}, white_diffuse, {0, 0, 0});
    auto *tall_block_2 = new Quad({423, 247, 0}, {423, 247, 330}, {472, 406, 330}, {472, 406, 0}, white_diffuse, {0, 0, 0});
    auto *tall_block_3 = new Quad({472, 406, 0}, {472, 406, 330}, {314, 456, 330}, {314, 456, 0}, white_diffuse, {0, 0, 0});
    auto *tall_block_4 = new Quad({314, 456, 0}, {314, 456, 330}, {265, 296, 330}, {265, 296, 0}, white_diffuse, {0, 0, 0});
    auto *tall_block_5 = new Quad({265, 296, 0}, {265, 296, 330}, {423, 247, 330}, {423, 247, 0}, white_diffuse, {0, 0, 0});

    auto *aggregation = new Aggregation();
    auto* scene = new Scene(aggregation);

    aggregation->push_back(floor);
    aggregation->push_back(light);
    aggregation->push_back(ceiling);
    aggregation->push_back(back_wall);
//    aggregation->push_back(front_wall);
    aggregation->push_back(green_wall);
    aggregation->push_back(red_wall);
    aggregation->push_back(short_block_1);
    aggregation->push_back(short_block_2);
    aggregation->push_back(short_block_3);
    aggregation->push_back(short_block_4);
    aggregation->push_back(short_block_5);
    aggregation->push_back(tall_block_1);
    aggregation->push_back(tall_block_2);
    aggregation->push_back(tall_block_3);
    aggregation->push_back(tall_block_4);
    aggregation->push_back(tall_block_5);
    scene->commit();

    cout << scene->aggregation->geometries.size() << endl;


    Vector3f point1 = {200, -800, 273};
    float pdf;
    Vector3f point2 = back_wall->sample_point(pdf);
//    Vector3f point2 = {550, 550, 0};
    Ray *ray = new Ray(point1, point2);

    auto *interaction = new Interaction(ray);
    unsigned geomID = scene->intersect(interaction);

    if(geomID != RTC_INVALID_GEOMETRY_ID) {
        cout<<"Hit the geometry which id is: "<<geomID<<endl;
        cout<<"Hit information: "<<endl;
        cout<<"time="<<interaction->ray->t<<endl;
        cout<<"(tFar="<<interaction->ray->tMax<<", tNear="<<interaction->ray->tMin<<")"<<endl;
        cout<<"n("<<interaction->normal.x()<<", "<<interaction->normal.y()<<", "<<interaction->normal.z()<<")"<<endl;
        cout<<"(u="<<interaction->u<<", v="<<interaction->v<<")"<<endl;
        cout<<"Geometry information: "<<endl;
        Geometry *geometry = aggregation->findGeometryById(geomID);
        if(geometry == nullptr) cout<<"not find the geometry";
        else {

        }
    } else
        cout<<"Didn't find any hit"<<endl;
}