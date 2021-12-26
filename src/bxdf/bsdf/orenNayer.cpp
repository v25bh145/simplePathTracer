//
// Created by v25bh on 2021/12/26.
//

#include "orenNayer.h"

namespace pathTracer {
    Vector3f OrenNayer::sample_f(Interaction* interaction, Ray* wi, float& pdf, int& sampleType) {
        Vector3f n = interaction->normal;
        Vector3f s, t;
        orthogonal(n, s, t);

        //Ray *wo = interaction->ray;

        RandomGenerator randomGenerator;
        float u = randomGenerator.uniform0To1(), v = randomGenerator.uniform0To1();
        //Vector3f wi_obj = {sqrt(u) * (float)cos(2 * PI * v), sqrt(u) * (float)sin(2 * PI * v), sqrt(1 - u)};
        // TODO: oren-nayer sampling(random uniform)
        float theta_i = PI * u / 2, alpha_i = 2 * PI * v;
        float sintheta_i = sin(theta_i), costheta_i = cos(theta_i),
            sinalpha_i = sin(alpha_i), cosalpha_i = cos(alpha_i);
        Vector3f wi_obj = { sintheta_i * cosalpha_i, sintheta_i * sinalpha_i, costheta_i };

        // rotate: (0, 0, 1) => n
        Matrix3f trans_obj_world, trans_world_obj;
        trans_obj_world <<
            s.x(), t.x(), n.x(),
            s.y(), t.y(), n.y(),
            s.z(), t.z(), n.z();
        trans_world_obj = trans_obj_world.inverse();

        Vector3f wi_world = trans_obj_world * wi_obj;

        *wi = Ray(interaction->p, { wi_world.x(), wi_world.y(), wi_world.z() }, 0);
        pdf = 1.f / (sintheta_i * PI * PI);
        sampleType = type;

        Vector3f wo_world = -interaction->ray->direction;
        Vector3f wo_obj = trans_world_obj * wo_world;
        assert(wo_obj.z() >= 0.f);
        float costheta_o = wo_obj.z();
        float theta_o = acos(costheta_o);
        float sintheta_o = sin(theta_o);
        float alpha, beta;
        if (theta_i > theta_o) {
            alpha = theta_i;
            beta = theta_o;
        }
        else {
            alpha = theta_o;
            beta = theta_i;
        }

        float cosalpha_o = wo_obj.x() / sintheta_o;
        float sinalpha_o = wo_obj.y() / sintheta_o;

        float cosPhi_iMinusPli_o = cosalpha_i * cosalpha_o + sinalpha_i * sinalpha_o;
        cosPhi_iMinusPli_o = cosPhi_iMinusPli_o > 0.f ? cosPhi_iMinusPli_o : 0.f;
        return R * (A + B * cosPhi_iMinusPli_o * sin(alpha) * sin(beta)) / PI;
    }
    Vector3f OrenNayer::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        // rotate: (0, 0, 1) => n
        Vector3f n = interaction->normal;
        Vector3f s, t;
        orthogonal(n, s, t);
        Matrix3f trans_obj_world, trans_world_obj;
        trans_obj_world <<
            s.x(), t.x(), n.x(),
            s.y(), t.y(), n.y(),
            s.z(), t.z(), n.z();
        trans_world_obj = trans_obj_world.inverse();

        Vector3f wi_world = wi->direction;
        Vector3f wi_obj = trans_world_obj * wi_world;
        Vector3f wo_world = interaction->ray->direction;
        Vector3f wo_obj = trans_world_obj * wo_world;

        float costheta_o = wo_obj.z();
        float theta_o = acos(costheta_o);
        float sintheta_o = sin(theta_o);
        float cosalpha_o = wo_obj.x() / sintheta_o;
        float sinalpha_o = wo_obj.y() / sintheta_o;

        float costheta_i = wi_obj.z();
        float theta_i = acos(costheta_i);
        float sintheta_i = sin(theta_i);
        float cosalpha_i = wi_obj.x() / sintheta_i;
        float sinalpha_i = wi_obj.y() / sintheta_i;

        float alpha, beta;
        if (theta_i > theta_o) {
            alpha = theta_i;
            beta = theta_o;
        }
        else {
            alpha = theta_o;
            beta = theta_i;
        }

        float cosPhi_iMinusPli_o = cosalpha_i * cosalpha_o + sinalpha_i * sinalpha_o;
        cosPhi_iMinusPli_o = cosPhi_iMinusPli_o > 0.f ? cosPhi_iMinusPli_o : 0.f;

        pdf = 1.f / (sintheta_i * PI * PI);
        return R * (A + B * cosPhi_iMinusPli_o * sin(alpha) * sin(beta)) / PI;
    }
    void OrenNayer::deepCopy(BxDF*& bxdf)
    {
        bxdf = new OrenNayer(this->R, this->sigma);
    }
}