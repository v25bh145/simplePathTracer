//
// Created by v25bh on 2021/11/23.
//

#include "bsdfs.h"

namespace pathTracer {

    Vector3f BSDFs::sample_f(Interaction *interaction, Ray *wi, float &pdf, int &sampleType) {
        return Eigen::Vector3f();
    }
    Vector3f BSDFs::f(Interaction* interaction, Ray* wi, float& pdf)
    {
        return Vector3f();
    }
    void BSDFs::deepCopy(BxDF*& bxdf)
    {
        vector<BxDF*> newBxDFs;
        for (auto bxdf : this->BxDFs) {
            BxDF* newBxdf = nullptr;
            bxdf->deepCopy(newBxdf);
            newBxDFs.push_back(newBxdf);
        }
        bxdf = new BSDFs(newBxDFs);
    }
}