//
// Created by v25bh on 2021/11/23.
//

#include "scene.h"

namespace pathTracer {
    void Scene::commit() {
        aggregation->attachAllGeometriesToScene(this);
        rtcCommitScene(*RTCInnerScene);
    }

    unsigned Scene::intersect(Interaction *interaction) {
        RTCRayHit rayHit;
        rayHit.ray = interaction->ray->getRTCInnerRay();
        rayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
        rayHit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

        struct RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        rtcIntersect1(*RTCInnerScene, &context, &rayHit);

        if(rayHit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
            //interaction->ray->updateRay(rayHit.ray);

            interaction->normal = interaction->normalShading = {rayHit.hit.Ng_x, rayHit.hit.Ng_y, rayHit.hit.Ng_z};

            if (interaction->ray->direction.dot(interaction->normal) > 0) {
                interaction->normal = -interaction->normal;
                interaction->normalShading = -interaction->normalShading;
            }

            interaction->normal.normalize();
            interaction->normalShading.normalize();

            interaction->u = rayHit.hit.u; interaction->v = rayHit.hit.v;

            interaction->p = {
                    rayHit.ray.org_x + rayHit.ray.dir_x * rayHit.ray.tfar,
                    rayHit.ray.org_y + rayHit.ray.dir_y * rayHit.ray.tfar,
                    rayHit.ray.org_z + rayHit.ray.dir_z * rayHit.ray.tfar,
            };
//            cout<<"Hit the geometry which id is: "<<rayHit.hit.geomID<<endl;
//            cout<<"Hit information: "<<endl;
//            cout<<"time="<<rayHit.ray.time<<endl;
//            cout<<"(tFar="<<rayHit.ray.tfar<<", tNear="<<rayHit.ray.tnear<<")"<<endl;
//            cout<<"n("<<rayHit.hit.Ng_x<<", "<<rayHit.hit.Ng_y<<", "<<rayHit.hit.Ng_z<<")"<<endl;
//            cout<<"(u="<<rayHit.hit.u<<", v="<<rayHit.hit.v<<")"<<endl;

            return rayHit.hit.geomID;
        }
        return 0;
    }
}