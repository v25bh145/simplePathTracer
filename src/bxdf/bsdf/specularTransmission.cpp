//
// Created by v25bh on 2021/11/23.
//

#include "specularTransmission.h"

namespace pathTracer {
    Vector3f SpecularTransmission::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        Vector3f n = -interaction->normal;
        Vector3f s, t;
        orthogonal(n, s, t);

        Vector3f wo_world = interaction->ray->direction;

        Matrix3f trans_obj_world, trans_world_obj;
        trans_obj_world <<
            s.x(), t.x(), n.x(),
            s.y(), t.y(), n.y(),
            s.z(), t.z(), n.z();
        trans_world_obj = trans_obj_world.inverse();
        // the same hemisphere plane as wi's
        Vector3f wo_obj = trans_world_obj * wo_world;
        float k = wo_obj.y() / wo_obj.x();

        float wo_costheta = wo_obj.z();
        float wo_theta = acos(wo_costheta);
        float wo_sintheta = sin(wo_theta);
        float wi_sintheta;
        // -n: negative(negative(normal))=positive
        if (interaction->geometry->getOutsideNormal().dot(-n) > 0) { 
            wi_sintheta = wo_sintheta * outsideEta / insideEta;
        } else { 
            wi_sintheta = wo_sintheta * insideEta / outsideEta;
        }
        if (wi_sintheta >= 1.f){
            //pdf = 1;
            pdf = abs(interaction->normal.dot(wi->direction));
            sampleType = type;
            return {0, 0, 0}; 
        }
        float wi_theta = asin(wi_sintheta);
        float wi_costheta = cos(wi_theta);


        //float wo_cosalpha = wo_obj.x() / wo_sintheta;
        //float wo_sinalpha = wo_obj.y() / wo_sintheta;
        //float wi_cosalpha = wo_cosalpha;
        //float wi_sinalpha = wo_sinalpha;

        Vector3f wi_obj = { wo_obj.x(), wo_obj.y(), wi_costheta };
        wi_obj.normalize();
        Vector3f wi_world = trans_obj_world * wi_obj;
        wi_world.normalize();

        *wi = Ray(interaction->p, wi_world, 0);
        //pdf = 1;
        pdf = abs(interaction->normal.dot(wi->direction));
        sampleType = type;
        return ks;
    }
    Vector3f SpecularTransmission::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        //pdf = 1;
        pdf = abs(interaction->normal.dot(wi->direction));
        return ks;
    }
    void SpecularTransmission::deepCopy(BxDF*& bxdf)
    {
        bxdf = new SpecularTransmission(this->ks, this->outsideEta, this->insideEta);
    }
}