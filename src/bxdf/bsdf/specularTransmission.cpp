//
// Created by v25bh on 2021/11/23.
//

#include "specularTransmission.h"

namespace pathTracer {
    Vector3f SpecularTransmission::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        pdf = 1;
        sampleType = type;

        Vector3f n = -interaction->normal;
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
        float k = wo_obj.y() / wo_obj.x();

        float wo_costheta = wo_obj.z();
        float wo_theta = acos(wo_costheta);
        float wo_sintheta = sin(wo_theta);
        float wi_sintheta;
        // -n: negative(negative(normal))=positive
        if (interaction->geometry->getOutsideNormal().dot(-n) > 0) { 
            //cout << "�������" << endl;
            wi_sintheta = wo_sintheta * (outsideEta / insideEta);
        } else { 
            //cout << "�ڲ����" << endl;
            wi_sintheta = wo_sintheta / (outsideEta / insideEta);
        }
        if (wi_sintheta >= 1.f) return {0, 0, 0};
        float wi_theta = asin(wi_sintheta);
        float wi_costheta = cos(wi_theta);
        //cout << "wo_theta" << wo_theta << endl;
        //cout << "wo_sintheta" << wo_sintheta << endl;
        //cout << "wi_sintheta" << wi_sintheta << endl;

        //float wo_cosalpha = wo_obj.x() / wo_sintheta;
        //float wo_sinalpha = wo_obj.y() / wo_sintheta;
        //float wi_cosalpha = wo_cosalpha;
        //float wi_sinalpha = wo_sinalpha;

        Vector3f wi_obj = { wo_obj.x(), wo_obj.y(), wi_costheta };
        wi_obj.normalize();
        //cout << "wi_obj=" << vector3fToString(wi_obj) << endl;
        Vector3f wi_world = trans_obj_world * wi_obj;
        //cout << "wi_world=" << vector3fToString(wi_world) << endl;
        wi_world.normalize();

        *wi = Ray(interaction->p, wi_world, 0);
        return ks;
    }
    Vector3f SpecularTransmission::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        pdf = 1;
        return ks;
    }
    void SpecularTransmission::deepCopy(BxDF*& bxdf)
    {
        bxdf = new SpecularTransmission(this->ks, this->outsideEta, this->insideEta);
    }
}