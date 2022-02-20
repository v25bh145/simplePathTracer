//
// Created by v25bh on 2021/11/23.
//

#include "scene.h"

namespace pathTracer {
    void Scene::commit() {
        aggregation->attachAllGeometriesToScene(this);
        rtcCommitScene(*RTCInnerScene);
    }

    void Scene::deepCopy(Scene*& scene)
    {
        scene = new Scene();
        scene->aggregation = nullptr;
        scene->RTCInnerScene = this->RTCInnerScene;
        this->aggregation->deepCopy(scene->aggregation);
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

            interaction->RTCu = rayHit.hit.u; interaction->RTCv = rayHit.hit.v;

            interaction->time = rayHit.ray.tfar;
            interaction->p = {
                    rayHit.ray.org_x + rayHit.ray.dir_x * rayHit.ray.tfar,
                    rayHit.ray.org_y + rayHit.ray.dir_y * rayHit.ray.tfar,
                    rayHit.ray.org_z + rayHit.ray.dir_z * rayHit.ray.tfar,
            };
            // differential
            if (interaction->rayDifferential != nullptr) {
                RTCRayHit rayHitDifferential;
                rayHitDifferential.ray = interaction->rayDifferential->getRTCInnerRay();
                rayHitDifferential.hit.geomID = RTC_INVALID_GEOMETRY_ID;
                rayHitDifferential.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;
                struct RTCIntersectContext contextDifferential;
                rtcInitIntersectContext(&contextDifferential);
                rtcIntersect1(*RTCInnerScene, &contextDifferential, &rayHitDifferential);
                // rayDifferential's ID == ray's ID
                if (rayHitDifferential.hit.geomID == rayHit.hit.geomID) {
                    Vector3f pDifferential = {
                            rayHitDifferential.ray.org_x + rayHitDifferential.ray.dir_x * rayHitDifferential.ray.tfar,
                            rayHitDifferential.ray.org_y + rayHitDifferential.ray.dir_y * rayHitDifferential.ray.tfar,
                            rayHitDifferential.ray.org_z + rayHitDifferential.ray.dir_z * rayHitDifferential.ray.tfar,
                    };
                    pDifferential = pDifferential - interaction->p;
                    interaction->pixelSizeProjected = pDifferential.norm();
                    cout << "projected" << interaction->pixelSizeProjected << endl;
                }
            }
            // UV
            if (rayHit.hit.geomID > 0) {
                interaction->geometry = aggregation->findGeometryById(rayHit.hit.geomID);
                Vector2f UV = interaction->geometry->getUV(interaction->p);
                interaction->u = UV.x(); interaction->v = UV.y();
            }
            else
                interaction->geometry = nullptr;
            //cout<<"Hit the geometry which id is: "<<rayHit.hit.geomID<<endl;
            //cout<<"Hit information: "<<endl;
            //cout<<"time="<<rayHit.ray.time<<endl;
            //cout<<"(tFar="<<rayHit.ray.tfar<<", tNear="<<rayHit.ray.tnear<<")"<<endl;
            //cout<<"n("<<rayHit.hit.Ng_x<<", "<<rayHit.hit.Ng_y<<", "<<rayHit.hit.Ng_z<<")"<<endl;
            //cout<<"(u="<<rayHit.hit.u<<", v="<<rayHit.hit.v<<")"<<endl;

            return rayHit.hit.geomID;
        }
        return 0;
    }
}