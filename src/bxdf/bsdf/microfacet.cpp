//
// Created by v25bh on 2022/01/14.
//

#include "microfacet.h"
#include "fresnelSpecular.h"

namespace pathTracer {
	Vector3f Microfacet::sample_f(Interaction* interaction, Ray* wi, float& pdf, int& sampleType)
	{
        Vector3f n = interaction->normal;
        Vector3f s, t;
        orthogonal(n, s, t);

        //Ray *wo = interaction->ray;

        RandomGenerator randomGenerator;
        float u = randomGenerator.uniform0To1(), v = randomGenerator.uniform0To1();
        //Vector3f wi_obj = {sqrt(u) * (float)cos(2 * PI * v), sqrt(u) * (float)sin(2 * PI * v), sqrt(1 - u)};
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

        float cosalpha_o = wo_obj.x() / sintheta_o;
        float sinalpha_o = wo_obj.y() / sintheta_o;


        Vector3f wh_dir = (wo_obj + wi_obj).normalized();
        float Fr = FrIron(wh_dir, wo_obj);


		// ÒþÊ½ÕÚÕÖº¯Êý
		float masking = 1.f / (1.f + distribution->Lambda(wo_obj));
        float shadowing = 1.f / (1.f + distribution->Lambda(wi_obj));
        float G2 = masking * shadowing;

		float D = distribution->D(wh_dir);
		float brdf = Fr * G2 * D / (4.f * costheta_o * costheta_i);

        return ks * brdf;
	}
	Vector3f Microfacet::f(Interaction* interaction, Ray* wi, float& pdf)
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
        Vector3f wo_world = -interaction->ray->direction;
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

        Vector3f wh_dir = (wo_obj + wi_obj).normalized();
        float Fr = FrIron(wh_dir, wo_obj);


        // ÒþÊ½ÕÚÕÖº¯Êý
        float masking = 1.f / (1.f + distribution->Lambda(wo_obj));
        float shadowing = 1.f / (1.f + distribution->Lambda(wi_obj));
        float G2 = masking * shadowing;

        float D = distribution->D(wh_dir);
        float brdf = Fr * G2 * D / (4.f * costheta_o * costheta_i);

        pdf = 1.f / (sintheta_i * PI * PI);
        //cout << "brdf" << brdf << endl;
        return ks * brdf;
	}
	void Microfacet::deepCopy(BxDF*& bxdf)
	{
        bxdf = new Microfacet(this->ks, this->distributionType, this->alpha);
	}
	float BeckmannIsotropic::D(Vector3f wh_obj)
	{
        if (wh_obj.z() < 0.f) return 0.f;
        float tan2Theta = (1.f - wh_obj.z() * wh_obj.z()) / (wh_obj.z() * wh_obj.z());
        return exp(-tan2Theta / (alpha * alpha)) / (PI * alpha * alpha * wh_obj.z() * wh_obj.z() * wh_obj.z() * wh_obj.z());
	}
	float BeckmannIsotropic::Lambda(Vector3f wo_obj)
	{
        float tanTheta_o = sqrt(1.f - wo_obj.z() * wo_obj.z()) / wo_obj.z();
        float a = 1.f / (tanTheta_o * alpha);
        if (a < 1.6f) {
            return (1.f - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
        }
        return 0.f;
	}
	void BeckmannIsotropic::deepCopy(MicrofacetDistribution* microfacetDistribution)
	{
        microfacetDistribution = new BeckmannIsotropic(alpha);
	}
	float GGXIsotropic::D(Vector3f wh_obj)
	{
        if (wh_obj.z() < 0.f) return 0.f;
        float cos2Theta = wh_obj.z() * wh_obj.z();
        float tan2Theta = (1.f - cos2Theta) / cos2Theta;
        float tmp = 1.f + tan2Theta / (alpha * alpha);
        return 1.f / (PI * alpha * alpha * cos2Theta * cos2Theta * tmp * tmp);
	}
	float GGXIsotropic::Lambda(Vector3f wo_obj)
	{
        float tanTheta_o = sqrt(1.f - wo_obj.z() * wo_obj.z()) / wo_obj.z();
        float a = 1.f / (tanTheta_o * alpha);
        return (-1.f + sqrt(1.f + 1.f / (a * a))) / 2.f;
	}
	void GGXIsotropic::deepCopy(MicrofacetDistribution* microfacetDistribution)
	{
        microfacetDistribution = new GGXIsotropic(alpha);
    }
}