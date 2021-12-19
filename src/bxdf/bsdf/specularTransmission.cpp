//
// Created by v25bh on 2021/11/23.
//

#include "specularTransmission.h"

namespace pathTracer {
    Vector3f SpecularTransmission::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        //Vector3f n = sameSide(interaction->normal, );
        Vector3f n = interaction->normal;
        Vector3f s, t;
        orthogonal(n, s, t);

        Vector3f wo_world = interaction->ray->direction;
        //cout << "wo_world=" << vector3fToString(wo_world) << endl;

        Matrix3f trans_obj_world, trans_world_obj;
        trans_obj_world <<
            s.x(), t.x(), n.x(),
            s.y(), t.y(), n.y(),
            s.z(), t.z(), n.z();
        trans_world_obj = trans_obj_world.inverse();
        // the same hemisphere plane as wi's
        Vector3f wo_obj = trans_world_obj * wo_world;
        //cout << "wo_obj=" << vector3fToString(wo_obj) << endl;

        float wo_costheta = wo_obj.z();
        float wo_theta = acos(wo_costheta);
        float wo_sintheta = sin(wo_theta);
        float wi_sintheta = wo_sintheta * eta;
        float wi_theta = asin(wi_sintheta);
        float wi_costheta = cos(wi_theta);
        //cout << "wo_theta" << wo_theta << endl;
        //cout << "wo_sintheta" << wo_sintheta << endl;
        //cout << "wi_sintheta" << wi_sintheta << endl;

        float wo_cosalpha = wo_obj.x() / wo_sintheta;
        float wo_sinalpha = wo_obj.y() / wo_sintheta;
        float wi_cosalpha = -wo_cosalpha;
        float wi_sinalpha = -wo_sinalpha;

        Vector3f wi_obj = { wi_cosalpha * wi_sintheta, wi_sinalpha * wi_sintheta, wi_costheta };
        //cout << "wi_obj=" << vector3fToString(wi_obj) << endl;
        Vector3f wi_world = trans_obj_world * wi_obj;
        //cout << "wi_world=" << vector3fToString(wi_world) << endl;
        wi_world.normalize();

        *wi = Ray(interaction->p, wi_world, 0);

        pdf = abs(interaction->normal.dot(wi->direction));
        sampleType = type;
        return ks;
    }
    Vector3f SpecularTransmission::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        pdf = abs(interaction->normal.dot(wi->direction));;
        return ks;
    }
}