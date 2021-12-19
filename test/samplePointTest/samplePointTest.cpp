//
// Created by v25bh on 2021/12/4.
//

// test the geometries sample [simulate with no obj parser]
#include <iostream>
#include "../../src/scene.h"
#include "../../src/geometry/rectangle.h"

using namespace std;
using namespace pathTracer;

int main(int argc, char** argv) {
    /*
     * information for obj & mtl
     */

    Vector3f ka, kd, ks;
    BxDF *bxdf = nullptr;
    Quad *quad = new Quad({2, 2, 0}, {2, 1, 0}, {1, 1, 0}, {1, 2, 0}, (BxDF *) bxdf, {0, 0, 0});
    cout<<"sample 10 point in quad"<<endl;
    for(int i = 0; i < 10; i++) {
        Vector3f point = quad->sample_point();
        cout<<"("<<point.x()<<", "<<point.y()<<", "<<point.z()<<")"<<endl;
    }
    Rectangle *rectangle = new Rectangle({0, 0, 0}, {1, 1, 1}, bxdf, {0, 0, 0});
    cout<<"sample 10 point in rectangle"<<endl;
    for(int i = 0; i < 10; i++) {
        Vector3f point = rectangle->sample_point();
        cout<<"("<<point.x()<<", "<<point.y()<<", "<<point.z()<<")"<<endl;
    }
}