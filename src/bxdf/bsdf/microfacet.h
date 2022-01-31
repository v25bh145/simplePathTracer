//
// Created by v25bh on 2022/01/14.
//

#ifndef SIMPLEPATHTRACER_MICROFACET_H
#define SIMPLEPATHTRACER_MICROFACET_H

#include "../../default.h"
#include "../bsdf.h"

namespace pathTracer {
    enum MicrofacetDistributionType { BECKMANNISOTROPIC, GGXISOTROPIC };
    // 物体坐标系
    class MicrofacetDistribution {
    public:
        virtual float D(Vector3f wh_obj) = 0;
        virtual float Lambda(Vector3f wo_obj) = 0;
        virtual void deepCopy(MicrofacetDistribution* microfacetDistribution) = 0;
    };
    class BeckmannIsotropic : public MicrofacetDistribution {
    public:
        float alpha;
        BeckmannIsotropic(float alpha) :alpha(alpha) {};
        float D(Vector3f wh_obj) override;
        float Lambda(Vector3f wo_obj) override;
        void deepCopy(MicrofacetDistribution* microfacetDistribution) override;
    };
    class GGXIsotropic : public MicrofacetDistribution {
    public:
        float alpha;
        GGXIsotropic(float alpha) :alpha(alpha) {};
        float D(Vector3f wh_obj) override;
        float Lambda(Vector3f wo_obj) override;
        void deepCopy(MicrofacetDistribution* microfacetDistribution) override;
    };
    class Microfacet : public BSDF {
    public:
        float alpha;
        unsigned distributionType;
        Vector3f ks;
        MicrofacetDistribution* distribution;
        Microfacet(Vector3f ks, unsigned distributionType, float alpha) :ks(ks), distributionType(distributionType), alpha(alpha) {
            type = BxDFType::REFLECTION;

            switch (distributionType) {
            case BECKMANNISOTROPIC:
                distribution = new BeckmannIsotropic(this->alpha);
                break;
            case GGXISOTROPIC:
                distribution = new GGXIsotropic(this->alpha);
                break;
            default:
                distribution = nullptr;
                cout << "false distribution type of microfacet." << endl;
                assert(false);
            }
        };
        Vector3f sample_f(Interaction* interaction, Ray* wi, float& pdf, int& sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;

        void deepCopy(BxDF*& bxdf) override;
    };
}


#endif //SIMPLEPATHTRACER_MICROFACET_H
