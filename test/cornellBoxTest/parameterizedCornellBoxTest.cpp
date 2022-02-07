//
// Created by v25bh on 2022/02/01.
//

// test the shadow ray [simulate with no obj parser]
#include "../../src/pathTracer.h"

using namespace std;
using namespace pathTracer;

int main(int argc, char** argv) {
    /*
    * scene creation
    */

    // material creation
    Vector3f light_ka = { 1.f, 1.f, 1.f };

    auto* red_diffuse = new LambertianReflection({ 0.63f, 0.065f, 0.05f });
    auto* specularReflection = new SpecularReflection({ 1.f, 1.f, 1.f });
    auto* green_diffuse = new LambertianReflection({ 0.14f, 0.45f, 0.091f });
    auto* blue_diffuse = new LambertianReflection({ 0.14f, 0.091f, 0.45f });
    auto* white_diffuse = new LambertianReflection({ 0.725f, 0.71f, 0.68f });
    auto* light_diffuse = new LambertianReflection({ 0, 0, 0 });
    auto* specularTransmission = new SpecularTransmission({ 1.f, 1.f, 1.f }, 1.f, 4.f / 3.f);
    auto* fresnelSpecular = new FresnelSpecular({ 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, 1.f, 4.f / 3.f);
    auto* orenNayerRed = new OrenNayer({ 0.63f, 0.065f, 0.05f }, 0.5f);
    auto* iron_microfacet = new Microfacet({ 0.46f, 0.46f, 0.47f }, MicrofacetDistributionType::BECKMANNISOTROPIC, 1.f);
    auto* green_microfacet = new Microfacet({ 0.14f, 0.091f, 0.45f }, MicrofacetDistributionType::BECKMANNISOTROPIC, 1.f);

    auto* muddyMedium = new Medium({ 0.20f, 3.5f, 3.5f }, { 0.9f, 0.05f, 0.05f });

    //geometry creation
    auto* floor1 = new Triangle({ 1.f, 0, 0 }, { 0, 0, 0 }, { 0, 1.f, 0 }, white_diffuse, { 0, 0, 0 });
    auto* floor2 = new Triangle({ 1.f, 0, 0 }, { 0, 1.f, 0 }, { 1.f, 1.f, 0 }, white_diffuse, { 0, 0, 0 });

    auto* light1 = new Triangle({ 0.7f, 0.3f, 0.99f }, { 0.7f, 0.7f, 0.99f }, { 0.3f, 0.7f, 0.99f }, light_diffuse, light_ka, { 0, 0, -1 }, true);
    auto* light2 = new Triangle({ 0.7f, 0.3f, 0.99f }, { 0.3f, 0.3f, 0.99f }, { 0.3f, 0.7f, 0.99f }, light_diffuse, light_ka, { 0, 0, -1 }, true);
    auto* ceiling1 = new Triangle({ 1.f, 0, 1.f }, { 1.f, 1.f, 1.f }, { 0, 1.f, 1.f }, blue_diffuse, { 0, 0, 0 });
    auto* ceiling2 = new Triangle({ 1.f, 0, 1.f }, { 0, 1.f, 1.f }, { 0, 0, 1.f }, blue_diffuse, { 0, 0, 0 });
    auto* back_wall1 = new Triangle({ 1.f, 1.f, 0 }, { 0, 1.f, 0 }, { 0, 1.f, 1.f }, white_diffuse, { 0, 0, 0 });
    auto* back_wall2 = new Triangle({ 1.f, 1.f, 0 }, { 0, 1.f, 1.f }, { 1.f, 1.f, 1.f }, white_diffuse, { 0, 0, 0 });
    auto* green_wall1 = new Triangle({ 0, 1.f, 0 }, { 0, 0, 0 }, { 0, 0, 1.f }, green_diffuse, { 0, 0, 0 });
    auto* green_wall2 = new Triangle({ 0, 1.f, 0 }, { 0, 0, 1.f }, { 0, 1.f, 1.f }, green_diffuse, { 0, 0, 0 });
    auto* red_wall1 = new Triangle({ 1.f, 0, 0 }, { 1.f, 1.f, 0 }, { 1.f, 1.f, 1.f }, orenNayerRed, { 0, 0, 0 }, { -1, 0, 0 }, true);
    auto* red_wall2 = new Triangle({ 1.f, 0, 0 }, { 1.f, 1.f, 1.f }, { 1.f, 0, 1.f }, orenNayerRed, { 0, 0, 0 }, { -1, 0, 0 }, true);
    /*
    * specular transmission
    */
    auto* short_block_11 = new Triangle({ 0.236f, 0.118f, 0.3f }, { 0.149f, 0.409f, 0.3f }, { 0.436f, 0.495f, 0.3f }, fresnelSpecular, { 0, 0, 0 }, { 0, 0, 1 }, false, nullptr, muddyMedium);
    auto* short_block_12 = new Triangle({ 0.236f, 0.118f, 0.3f }, { 0.436f, 0.495f, 0.3f }, { 0.527f, 0.207f, 0.3f }, fresnelSpecular, { 0, 0, 0 }, { 0, 0, 1 }, false, nullptr, muddyMedium);
    Vector3f short_block2_vec1 = { 0, 0, 1 };
    Vector3f short_block2_vec2 = { 50, -158, 0 };
    Vector3f short_block2_vec3 = short_block2_vec1.cross(short_block2_vec2).normalized();
    auto* short_block_21 = new Triangle({ 0.527f, 0.207f, 0 }, { 0.527f, 0.207f, 0.3f }, { 0.436f, 0.495f, 0.3f }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block2_vec3, { 0, 1, 0 }), false, nullptr, muddyMedium);
    auto* short_block_22 = new Triangle({ 0.527f, 0.207f, 0 }, { 0.436f, 0.495f, 0.3f }, { 0.436f, 0.495f, 0 }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block2_vec3, { 0, 1, 0 }), false, nullptr, muddyMedium);
    Vector3f short_block3_vec1 = { 0, 0, 1 };
    Vector3f short_block3_vec2 = { 160, 49, 0 };
    Vector3f short_block3_vec3 = short_block3_vec1.cross(short_block3_vec2).normalized();
    auto* short_block_31 = new Triangle({ 0.236f, 0.118f, 0 }, { 0.236f, 0.118f, 0.3f }, { 0.527f, 0.207f, 0.3f }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block3_vec3, { 0, -1, 0 }), false, nullptr, muddyMedium);
    auto* short_block_32 = new Triangle({ 0.236f, 0.118f, 0 }, { 0.527f, 0.207f, 0.3f }, { 0.527f, 0.207f, 0 }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block3_vec3, { 0, -1, 0 }), false, nullptr, muddyMedium);
    Vector3f short_block4_vec1 = { 0, 0, 1 };
    Vector3f short_block4_vec2 = { 48, -160, 0 };
    Vector3f short_block4_vec3 = short_block4_vec1.cross(short_block4_vec2).normalized();
    auto* short_block_41 = new Triangle({ 0.149f, 0.409f, 0 }, { 0.149f, 0.409f, 0.3f }, { 0.236f, 0.118f, 0.3f }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block4_vec3, { -1, 0, 0 }), false, nullptr, muddyMedium);
    auto* short_block_42 = new Triangle({ 0.149f, 0.409f, 0 }, { 0.236f, 0.118f, 0.3f }, { 0.236f, 0.118f, 0 }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block4_vec3, { -1, 0, 0 }), false, nullptr, muddyMedium);
    Vector3f short_block5_vec1 = { 0, 0, 1 };
    Vector3f short_block5_vec2 = { 158, 47, 0 };
    Vector3f short_block5_vec3 = short_block5_vec1.cross(short_block5_vec2).normalized();
    auto* short_block_51 = new Triangle({ 0.436f, 0.495f, 0 }, { 0.436f, 0.495f, 0.3f }, { 0.149f, 0.409f, 0.3f }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block5_vec3, { 0, 1, 0 }), false, nullptr, muddyMedium);
    auto* short_block_52 = new Triangle({ 0.436f, 0.495f, 0 }, { 0.149f, 0.409f, 0.3f }, { 0.149f, 0.409f, 0 }, fresnelSpecular, { 0, 0, 0 }, sameSide(short_block5_vec3, { 0, 1, 0 }), false, nullptr, muddyMedium);

    auto* tall_block_11 = new Triangle({ 0.769f, 0.449f, 0.6f }, { 0.482f, 0.538f, 0.6f }, { 0.571f, 0.829f, 0.6f }, fresnelSpecular, { 0, 0, 0 }, { 0, 0, 1 });
    auto* tall_block_12 = new Triangle({ 0.769f, 0.449f, 0.6f }, { 0.571f, 0.829f, 0.6f }, { 0.858f, 0.738f, 0.6f }, fresnelSpecular, { 0, 0, 0 }, { 0, 0, 1 });
    Vector3f tall_block2_vec1 = { 0, 0, 1 };
    Vector3f tall_block2_vec2 = { 49, 159, 0 };
    Vector3f tall_block2_vec3 = sameSide(tall_block2_vec1.cross(tall_block2_vec2).normalized(), { 1, 0, 0 });
    auto* tall_block_21 = new Triangle({ 0.769f, 0.449f, 0 }, { 0.769f, 0.449f, 0.6f }, { 0.858f, 0.738f, 0.6f }, fresnelSpecular, { 0, 0, 0 }, tall_block2_vec3);
    auto* tall_block_22 = new Triangle({ 0.769f, 0.449f, 0 }, { 0.858f, 0.738f, 0.6f }, { 0.858f, 0.738f, 0 }, fresnelSpecular, { 0, 0, 0 }, tall_block2_vec3);
    Vector3f tall_block3_vec1 = { 0, 0, 1 };
    Vector3f tall_block3_vec2 = { -158, 50, 0 };
    Vector3f tall_block3_vec3 = sameSide(tall_block3_vec1.cross(tall_block3_vec2).normalized(), { 0, 1, 0 });
    auto* tall_block_31 = new Triangle({ 0.858f, 0.738f, 0 }, { 0.858f, 0.738f, 0.6f }, { 0.571f, 0.829f, 0.6f }, fresnelSpecular, { 0, 0, 0 }, tall_block3_vec3);
    auto* tall_block_32 = new Triangle({ 0.858f, 0.738f, 0 }, { 0.571f, 0.829f, 0.6f }, { 0.571f, 0.829f, 0 }, fresnelSpecular, { 0, 0, 0 }, tall_block3_vec3);
    Vector3f tall_block4_vec1 = { 0, 0, 1 };
    Vector3f tall_block4_vec2 = { 49, 160, 0 };
    Vector3f tall_block4_vec3 = sameSide(tall_block4_vec1.cross(tall_block4_vec2).normalized(), { -1, 0, 0 });
    auto* tall_block_41 = new Triangle({ 0.571f, 0.829f, 0 }, { 0.571f, 0.829f, 0.6f }, { 0.482f, 0.538f, 0.6f }, fresnelSpecular, { 0, 0, 0 }, tall_block4_vec3);
    auto* tall_block_42 = new Triangle({ 0.571f, 0.829f, 0 }, { 0.482f, 0.538f, 0.6f }, { 0.482f, 0.538f, 0 }, fresnelSpecular, { 0, 0, 0 }, tall_block4_vec3);
    Vector3f tall_block5_vec1 = { 0, 0, 1 };
    Vector3f tall_block5_vec2 = { -158, 49, 0 };
    Vector3f tall_block5_vec3 = sameSide(tall_block5_vec1.cross(tall_block5_vec2).normalized(), { -1, 0, 0 });
    auto* tall_block_51 = new Triangle({ 0.482f, 0.538f, 0 }, { 0.482f, 0.538f, 0.6f }, { 0.769f, 0.449f, 0.6f }, specularReflection, { 0, 0, 0 }, tall_block5_vec3);
    auto* tall_block_52 = new Triangle({ 0.482f, 0.538f, 0 }, { 0.769f, 0.449f, 0.6f }, { 0.769f, 0.449f, 0 }, specularReflection, { 0, 0, 0 }, tall_block5_vec3);

    auto* aggregation = new Aggregation();
    auto* scene = new Scene(aggregation);
    aggregation->push_back(floor1);
    aggregation->push_back(light1);
    aggregation->push_back(ceiling1);
    aggregation->push_back(back_wall1);
    aggregation->push_back(green_wall1);
    aggregation->push_back(red_wall1);
    aggregation->push_back(short_block_11);
    aggregation->push_back(short_block_21);
    aggregation->push_back(short_block_31);
    aggregation->push_back(short_block_41);
    aggregation->push_back(short_block_51);
    aggregation->push_back(tall_block_11);
    aggregation->push_back(tall_block_21);
    aggregation->push_back(tall_block_31);
    aggregation->push_back(tall_block_41);
    aggregation->push_back(tall_block_51);
    aggregation->push_back(floor2);
    aggregation->push_back(light2);
    aggregation->push_back(ceiling2);
    aggregation->push_back(back_wall2);
    aggregation->push_back(green_wall2);
    aggregation->push_back(red_wall2);
    aggregation->push_back(short_block_12);
    aggregation->push_back(short_block_22);
    aggregation->push_back(short_block_32);
    aggregation->push_back(short_block_42);
    aggregation->push_back(short_block_52);
    aggregation->push_back(tall_block_12);
    aggregation->push_back(tall_block_22);
    aggregation->push_back(tall_block_32);
    aggregation->push_back(tall_block_42);
    aggregation->push_back(tall_block_52);
    scene->commit();

    cout << "geometries count: " << scene->aggregation->geometries.size() << endl;
    // Ray information: origin (x=-0.533653, y=-16, z=6.04339), dir (x=0.357406, y=0.862869, z=-0.357377), tMin=9.2714, tMax=241.056
    //Ray* ray = new Ray({ 0.533653f, -16.f, 6.04339f }, { 0.357406f ,0.862869f, -0.357377f }, 0.f, 9.2714f, 241.056f);
    //auto* hitInteraction = new Interaction(ray);
    //unsigned hitGeomID = scene->intersect(hitInteraction);
    //Geometry* hitGeometry = hitInteraction->geometry;
    //cout <<"hitID=" << hitGeomID << endl;

    VolumePathIntegrator* integrator = new VolumePathIntegrator(5);
    //PathIntegrator* integrator = new PathIntegrator(5);
    //DirectIntegrator* integrator = new DirectIntegrator();

    Vector3f cameraOrigin = { 0.5f, -1.45f, 0.5f };
    Vector3f cameraLookingAt = { 0, 1, 0 };
    Vector3f cameraUpAngle = { 0, 0, 1 };
    Vector2i resolution = { 256, 256 };
    auto* camera = new Camera(cameraOrigin, cameraLookingAt, cameraUpAngle, 1.45f, PI / 4, 0, 200.f, scene, resolution, integrator, 20);
    cout << camera->toString() << endl;
    cout << "begin generating" << endl;
    camera->generate();
    cout << "end generating" << endl;

    auto* pngParser = new PngParser(camera->pixels, { resolution.x(), resolution.y() });
    pngParser->writeToFile("result.png");

    return 0;
}