//
// Created by v25bh on 2021/11/23.
//

#include "lambertianReflection.h"

namespace pathTracer {
    Vector3f LambertianReflection::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        Vector3f n = interaction->normal;
        Vector3f s, t;
        orthogonal(n, s, t);

        //Ray *wo = interaction->ray;

        //assert(n.norm() == 1);
        //assert(wo->direction.norm() == 1);

        RandomGenerator randomGenerator;
        float u = randomGenerator.uniform0To1(), v = randomGenerator.uniform0To1();
        //Vector3f wi_obj = {sqrt(u) * (float)cos(2 * PI * v), sqrt(u) * (float)sin(2 * PI * v), sqrt(1 - u)};
        float theta = PI * u / 2, alpha = 2 * PI * v;
        float sintheta = sin(theta), costheta = cos(theta),
            sinalpha = sin(alpha), cosalpha = cos(alpha);
        Vector3f wi_obj = { sintheta * cosalpha, sintheta * sinalpha, costheta };

        // rotate: (0, 0, 1) => n
        Matrix3f trans_obj_world;
        trans_obj_world <<
            s.x(), t.x(), n.x(),
            s.y(), t.y(), n.y(),
            s.z(), t.z(), n.z();

        Vector3f wi_world = trans_obj_world * wi_obj;

        *wi = Ray(interaction->p, {wi_world.x(), wi_world.y(), wi_world.z()}, 0);
        pdf = 1.f / (sintheta * PI * PI);
        sampleType = type;

        //cout << "----------------------------sample----------------------" << endl;

        //Vector3f p = interaction->p;
        //cout << "p: " << p.x() << ", " << p.y() << ", " << p.z() << endl;

        //cout<<"n: "<< n.x() << ", " << n.y() << ", " << n.z() << endl;
        //cout << "s: " << s.x() << ", " << s.y() << ", " << s.z() << endl;
        //cout << "t: " << t.x() << ", " << t.y() << ", " << t.z() << endl;
        //cout << "wo information" << endl;
        //cout << wo->toString() << endl;
        //cout << "wi information" << endl;
        //cout << wi->toString() << endl;
        //cout << "wi_obj information" << endl;
        //cout << "wi_obj: " << wi_obj.x() << ", " << wi_obj.y() << ", " << wi_obj.z() << endl;
        //cout << "wi_world information" << endl;
        //cout << "wi_world: " << wi_world.x() << ", " << wi_world.y() << ", " << wi_world.z() << endl;
        //cout << "----------------------------sample----------------------" << endl;
        return kd / PI;
    }
    Vector3f LambertianReflection::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        Vector3f n = interaction->normal;
        float costheta = n.dot(wi->direction);
        float sintheta = sqrt(1 - costheta * costheta);
        pdf = 1.f / ( sintheta * PI * PI );
        return kd / PI;
    }
}