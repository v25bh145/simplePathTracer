//
// Created by v25bh on 2021/11/23.
//

#include "bxdf.h"

namespace pathTracer {
    bool BxDF::hasType(unsigned type) {
        return this->type & type;
    }
}