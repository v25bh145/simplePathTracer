//
// Created by v25bh on 2021/12/4.
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
    //Vector3f light_ka = { 20, 20, 20 };
    Vector3f light_ka = { 7500, 7500, 7500 };

    auto* red_diffuse = new LambertianReflection({ 0.63f, 0.065f, 0.05f });
    auto* specularReflection = new SpecularReflection({ 1.f, 1.f, 1.f });
    auto* green_diffuse = new LambertianReflection({ 0.14f, 0.45f, 0.091f });
    auto* blue_diffuse = new LambertianReflection({ 0.14f, 0.091f, 0.45f });
    auto* white_diffuse = new LambertianReflection({ 0.725f, 0.71f, 0.68f });
    auto* light_diffuse = new LambertianReflection({ 0, 0, 0 });
    auto* specularTransmission = new SpecularTransmission({ 1.f, 1.f, 1.f }, 1.f, 4.f / 3.f);
    auto* fresnelSpecular = new FresnelSpecular({ 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, 1.f, 4.f / 3.f);

    auto* muddyMedium = new Medium({ 0.025f, 0.07f, 0.07f }, { 0.05f, 0.025f, 0.025f });

    //geometry creation
    auto* floor = new Quad({ 550, 0, 0 }, { 0, 0, 0 }, { 0, 550, 0 }, { 550, 550, 0 }, white_diffuse, { 0, 0, 0 });
    auto* light = new Quad({ 340, 230, 549.9 }, { 340, 330, 549.9 }, { 210, 330, 549.9 }, { 210, 230, 549.9 }, light_diffuse, light_ka, { 0, 0, -1 }, true);
    auto* ceiling = new Quad({ 550, 0, 550 }, { 550, 550, 550 }, { 0, 550, 550 }, { 0, 0, 550 }, white_diffuse, { 0, 0, 0 });
    auto* back_wall = new Quad({ 550, 550, 0 }, { 0, 550, 0 }, { 0, 550, 550 }, { 550, 550, 550 }, white_diffuse, { 0, 0, 0 });
    //    auto *front_wall = new Quad({550, 0, 0}, {0, 0, 0}, {0, 0, 550}, {550, 0, 550}, white_diffuse, {0, 0, 0});
    auto* green_wall = new Quad({ 0, 550, 0 }, { 0, 0, 0 }, { 0, 0, 550 }, { 0, 550, 550 }, blue_diffuse, { 0, 0, 0 });
    auto* red_wall = new Quad({ 550, 0, 0 }, { 550, 550, 0 }, { 550, 550, 550 }, { 550, 0, 550 }, red_diffuse, {0, 0, 0}, { -1, 0, 0 }, true);
    /*
    * specular transmission
    */
    auto* short_block_1 = new Quad({ 130, 65, 165 }, { 82, 225, 165 }, { 240, 272, 165 }, { 290, 114, 165 }, white_diffuse, { 0, 0, 0 }, { 0, 0, 1 }, false, nullptr, nullptr);
    Vector3f short_block2_vec1 = { 0, 0, 1 };
    Vector3f short_block2_vec2 = { 50, -158, 0 };
    Vector3f short_block2_vec3 = short_block2_vec1.cross(short_block2_vec2).normalized();
    auto* short_block_2 = new Quad({ 290, 114, 0 }, { 290, 114, 165 }, { 240, 272, 165 }, { 240, 272, 0 }, white_diffuse, { 0, 0, 0 }, sameSide(short_block2_vec3, { 0, 1, 0 }), false, nullptr, nullptr);
    Vector3f short_block3_vec1 = { 0, 0, 1 };
    Vector3f short_block3_vec2 = { 160, 49, 0 };
    Vector3f short_block3_vec3 = short_block3_vec1.cross(short_block3_vec2).normalized();
    auto* short_block_3 = new Quad({ 130, 65, 0 }, { 130, 65, 165 }, { 290, 114, 165 }, { 290, 114, 0 }, white_diffuse, { 0, 0, 0 }, sameSide(short_block3_vec3, { 0, -1, 0 }), false, nullptr, nullptr);
    Vector3f short_block4_vec1 = { 0, 0, 1 };
    Vector3f short_block4_vec2 = { 48, -160, 0 };
    Vector3f short_block4_vec3 = short_block4_vec1.cross(short_block4_vec2).normalized();
    auto* short_block_4 = new Quad({ 82, 225, 0 }, { 82, 225, 165 }, { 130, 65, 165 }, { 130, 65, 0 }, white_diffuse, { 0, 0, 0 }, sameSide(short_block4_vec3, { -1, 0, 0 }), false, nullptr, nullptr);
    Vector3f short_block5_vec1 = { 0, 0, 1 };
    Vector3f short_block5_vec2 = { 158, 47, 0 };
    Vector3f short_block5_vec3 = short_block5_vec1.cross(short_block5_vec2).normalized();
    auto* short_block_5 = new Quad({ 240, 272, 0 }, { 240, 272, 165 }, { 82, 225, 165 }, { 82, 225, 0 }, white_diffuse, { 0, 0, 0 }, sameSide(short_block5_vec3, { 0, 1, 0 }), false, nullptr, nullptr);

    //auto* short_block_1 = new Quad({ 130, 65, 165 }, { 82, 225, 165 }, { 240, 272, 165 }, { 290, 114, 165 }, white_diffuse, { 0, 0, 0 });
    //auto* short_block_2 = new Quad({ 290, 114, 0 }, { 290, 114, 165 }, { 240, 272, 165 }, { 240, 272, 0 }, white_diffuse, { 0, 0, 0 });
    //auto* short_block_3 = new Quad({ 130, 65, 0 }, { 130, 65, 165 }, { 290, 114, 165 }, { 290, 114, 0 }, white_diffuse, { 0, 0, 0 });
    //auto* short_block_4 = new Quad({ 82, 225, 0 }, { 82, 225, 165 }, { 130, 65, 165 }, { 130, 65, 0 }, white_diffuse, { 0, 0, 0 });
    //auto* short_block_5 = new Quad({ 240, 272, 0 }, { 240, 272, 165 }, { 82, 225, 165 }, { 82, 225, 0 }, white_diffuse, { 0, 0, 0 });

    //auto* tall_block_1 = new Quad({ 423, 247, 330 }, { 265, 296, 330 }, { 314, 456, 330 }, { 472, 406, 330 }, fresnelSpecular, { 0, 0, 0 }, {0, 0, 1});
    //Vector3f tall_block2_vec1 = { 0, 0, 1 };
    //Vector3f tall_block2_vec2 = { 49, 159, 0 };
    //Vector3f tall_block2_vec3 = sameSide(tall_block2_vec1.cross(tall_block2_vec2).normalized(), { 1, 0, 0 });
    //auto* tall_block_2 = new Quad({ 423, 247, 0 }, { 423, 247, 330 }, { 472, 406, 330 }, { 472, 406, 0 }, fresnelSpecular, { 0, 0, 0 }, tall_block2_vec3);
    //Vector3f tall_block3_vec1 = { 0, 0, 1 };
    //Vector3f tall_block3_vec2 = { -158, 50, 0 };
    //Vector3f tall_block3_vec3 = sameSide(tall_block3_vec1.cross(tall_block3_vec2).normalized(), { 0, 1, 0 });
    //auto* tall_block_3 = new Quad({ 472, 406, 0 }, { 472, 406, 330 }, { 314, 456, 330 }, { 314, 456, 0 }, fresnelSpecular, { 0, 0, 0 }, tall_block3_vec3);
    //Vector3f tall_block4_vec1 = { 0, 0, 1 };
    //Vector3f tall_block4_vec2 = { 49, 160, 0 };
    //Vector3f tall_block4_vec3 = sameSide(tall_block4_vec1.cross(tall_block4_vec2).normalized(), { -1, 0, 0 });
    //auto* tall_block_4 = new Quad({ 314, 456, 0 }, { 314, 456, 330 }, { 265, 296, 330 }, { 265, 296, 0 }, fresnelSpecular, { 0, 0, 0 }, tall_block4_vec3);
    Vector3f tall_block5_vec1 = { 0, 0, 1 };
    Vector3f tall_block5_vec2 = { -158, 49, 0 };
    Vector3f tall_block5_vec3 = sameSide(tall_block5_vec1.cross(tall_block5_vec2).normalized(), { -1, 0, 0 });
    auto* tall_block_5 = new Quad({ 265, 296, 0 }, { 265, 296, 330 }, { 423, 247, 330 }, { 423, 247, 0 }, specularReflection, { 0, 0, 0 }, tall_block5_vec3);

    auto* tall_block_1 = new Quad({ 423, 247, 330 }, { 265, 296, 330 }, { 314, 456, 330 }, { 472, 406, 330 }, white_diffuse, { 0, 0, 0 });
    auto* tall_block_2 = new Quad({ 423, 247, 0 }, { 423, 247, 330 }, { 472, 406, 330 }, { 472, 406, 0 }, white_diffuse, { 0, 0, 0 });
    auto* tall_block_3 = new Quad({ 472, 406, 0 }, { 472, 406, 330 }, { 314, 456, 330 }, { 314, 456, 0 }, white_diffuse, { 0, 0, 0 });
    auto* tall_block_4 = new Quad({ 314, 456, 0 }, { 314, 456, 330 }, { 265, 296, 330 }, { 265, 296, 0 }, white_diffuse, { 0, 0, 0 });
    //auto* tall_block_5 = new Quad({ 265, 296, 0 }, { 265, 296, 330 }, { 423, 247, 330 }, { 423, 247, 0 }, white_diffuse, { 0, 0, 0 });

    auto* aggregation = new Aggregation();
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
    aggregation->attachAllGeometriesToScene(scene);
    scene->commit();

    cout << "geometries count: " << scene->aggregation->geometries.size() << endl;

    PathIntegrator* integrator = new PathIntegrator(10, 10);

    Vector3f cameraOrigin = { 278, -800, 273 };
    Vector3f cameraLookingAt = { 0, 1, 0 };
    Vector3f cameraUpAngle = { 0, 0, 1 };
    Vector2i resolution = { 512, 512 };
    auto* camera = new Camera(cameraOrigin, cameraLookingAt, cameraUpAngle, 800, PI / 3, 0, 2000, scene, resolution, integrator);
    cout << camera->toString() << endl;
    cout << "begin generating" << endl;
    camera->generate();
    cout << "end generating" << endl;

    auto* pngParser = new PngParser(camera->pixels, { resolution.x(), resolution.y() });
    pngParser->writeToFile("result.png");

    return 0;
}