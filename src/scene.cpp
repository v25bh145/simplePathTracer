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
            
            // UV
            if (rayHit.hit.geomID > 0) {
                interaction->geometry = aggregation->findGeometryById(rayHit.hit.geomID);
                Vector2f UV = interaction->geometry->getUV(interaction->p);
                interaction->u = UV.x(); interaction->v = UV.y();
                pair<Vector3f, Vector3f> dpduv = interaction->geometry->getdpduv(interaction->p);
                interaction->dpdu = dpduv.first;
                interaction->dpdv = dpduv.second;
                if (interaction->ray->differential.hasDifferential) {
                    float d = interaction->normal.dot(interaction->p);

                    Vector3f rxOrigin = interaction->ray->differential.rxOrigin;
                    Vector3f rxDirection = interaction->ray->differential.rxDirection;
                    if (interaction->normal.dot(rxDirection) < 1e-8) goto textureDifferentialFlag;
                    float tx = -(interaction->normal.dot(rxOrigin) - d) / interaction->normal.dot(rxDirection);
                    Vector3f px = rxOrigin + tx * rxDirection;

                    Vector3f ryOrigin = interaction->ray->differential.ryOrigin;
                    Vector3f ryDirection = interaction->ray->differential.ryDirection;
                    if (interaction->normal.dot(ryDirection) < 1e-8) goto textureDifferentialFlag;
                    float ty = -(interaction->normal.dot(ryOrigin) - d) / interaction->normal.dot(ryDirection);
                    Vector3f py = ryOrigin + ty * ryDirection;

                    interaction->dpdx = px - interaction->p;
                    interaction->dpdy = py - interaction->p;

                    // TODO: 二元一次方程组
                }
                else {
    textureDifferentialFlag:
                    interaction->dpdx = interaction->dpdy = interaction->dstdx = interaction->dstdy = { 0, 0, 0 };
                }
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