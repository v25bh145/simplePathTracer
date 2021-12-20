//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_BRDF_H
#define SIMPLEPATHTRACER_BRDF_H

#include "../default.h"
#include "../bxdf.h"

namespace pathTracer {
    class BSDFs : public BxDF {
    public:
        vector<BxDF*> BxDFs;

        BSDFs(vector<BxDF*> BxDFs): BxDFs(BxDFs) {
            for(auto *bxdf : BxDFs) {
                type = type | bxdf->type;
            }
        }

        /// \param interaction input: wo, n
        /// \param wi output
        /// \param pdf output
        /// \param sampleType output
        /// \return Vector3f color(percentage)
        /// sample wi with transforming wo from n given to n=(0, 0, 1) and transform this wi from n=(0, 0, 1) to n given
        Vector3f sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) override;
        Vector3f f(Interaction* interaction, Ray* wi, float& pdf) override;
        void deepCopy(BxDF*& bxdf) override;
    };
}


#endif //SIMPLEPATHTRACER_BRDF_H
