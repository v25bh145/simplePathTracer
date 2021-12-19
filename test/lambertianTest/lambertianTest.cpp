//
// Created by v25bh on 2021/12/7.
//
// test the lambertian's sample

#include <iostream>
#include "../../src/scene.h"
#include "../../src/ray.h"
#include "../../src/mathHelper.h"
#include "../../src/bxdf/bsdf/lambertianReflection.h"
#include "../../src/bxdf/bsdf/specularTransmission.h"

using namespace std;
using namespace pathTracer;

void distributionTest() {
    int groupsNumber[100];
    int p;
    for(int & i : groupsNumber) i = 0;
    RandomGenerator randomGenerator;
    for(int i = 0; i < 1000000; ++i) {
        float u = randomGenerator.uniform0To1(), v = randomGenerator.uniform0To1();
        //float theta = acos(1 - 2 * u) / 2, phi = 2.f * PI * v;
        float theta = PI * u / 2, alpha = 2 * PI * v;
        if((int)(theta * 20.f / PI) == 10) {groupsNumber[9]++;continue;}
        if((int)(theta * 20.f / PI) < 0 || (int)(theta * 20.f / PI) > 9) cout<<"WARNING:"<<theta<<endl;
        else groupsNumber[(int)(theta * 20.f / PI)]++;
    }
    float sum = 0;
    for(int i = 0; i < 10; ++i) {
        cout<<"i: "<<i<<endl;
        cout<<((float)groupsNumber[i] / 1000000.f)<<endl;
        sum += ((float)groupsNumber[i] / 1000000.f);
    }
    cout<<"sum="<<sum<<endl;
}
void transformTest(Vector3f n, Vector3f p, Vector4f w_obj, Vector4f &w_world) {
    Vector3f s, t;
    orthogonal(n, s, t);
    Matrix4f trans_obj_world;
    trans_obj_world <<
            s.x(), s.y(), s.z(), p.x(),
            t.x(), t.y(), t.z(), p.y(),
            n.x(), n.y(), n.z(), p.z(),
            0, 0, 0, 1;
    w_world = trans_obj_world * w_obj;
}
void pdfTest(int N) {
    Vector3f sum = { 0.f, 0.f, 0.f };
    for (int i = 0; i < N; i++) {
        float pdf;
        LambertianReflection *bxdf = new LambertianReflection({ 1, 1, 1 });
        Ray* ray = new Ray({ 0, 0, 0 }, { 0, 0, 1 }, 0);
        Interaction* interaction = new Interaction(ray);
        interaction->p = { 0, 0, 0 };
        interaction->normal = { 0, 0, 1 };
        int sampleType;
        Ray* wi = nullptr;
        Vector3f value = bxdf->sample_f(interaction, wi, pdf, sampleType);
        sum = sum + value / pdf;
    }
    sum = sum / N;
    cout << vector3fToString(sum) << endl;
}
void example_sample_f(Ray *wi) {
    *wi = Ray({2, 2, 2}, {1, 1, 1});
}
int main() {
    // cos weighted test

    //distributionTest();

//    Vector3f n = {1, 1, 1}, p = {0, 0, 0};
//    n.normalize();
//    Vector4f w_obj = {1, 1, 1, 0}, w_world;
//    transformTest(n, p, w_obj, w_world);
//    cout<<w_world<<endl;

    //Ray *wi = new Ray();
    //example_sample_f(wi);
    //cout<<wi->origin<<endl;

    //pdfTest(1000);
    SpecularTransmission *specularTransmission = new SpecularTransmission({ 1.f, 1.f, 1.f }, 0.75f);
    Interaction *interaction = new Interaction();
    interaction->normal = { -1, 0, 0 };
    Vector3f wo_dir = { 1, 1, 1 };
    wo_dir.normalize();
    interaction->ray = new Ray({ 0, 0, 0 }, wo_dir, 0);
    interaction->p = { 1, 1, 1 };
    Ray* wi = new Ray();
    float pdf;
    int sampleType;
    Vector3f color = specularTransmission->sample_f(interaction, wi, pdf, sampleType);
    float abscoswon = abs(wo_dir.dot(interaction->normal));
    float abswon = acos(abscoswon);
    float abssinwon = sin(abswon);
    cout << "cos wo-n" << abscoswon << endl;
    cout << "wo-n" << abswon << endl;
    cout << "sin wo-n" << abssinwon << endl;

    float abscoswin = abs(wi->direction.dot(interaction->normal));
    float abswin = acos(abscoswin);
    float abssinwin = sin(abswin);
    cout << "sin wi-n" << abssinwin << endl;
    cout << "in/on" << abssinwin / abssinwon;
    return 0;
}