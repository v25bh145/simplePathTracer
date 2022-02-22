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
                if (interaction->ray->differential.hasDifferential) {
                    // get dpdx & dpdy
                    // inference -> readme/dpdx&dpdy.md
                    // dpdx
                    Vector3f rxOrigin = interaction->ray->differential.rxOrigin;
                    Vector3f rxDirection = interaction->ray->differential.rxDirection;
                    Vector3f XE = rxOrigin + rxDirection;
                    Matrix3f Ax;
                    Ax << interaction->normal.x(), interaction->normal.y(), interaction->normal.z(),
                        XE.y() - rxOrigin.y(), -XE.x() + rxOrigin.x(), 0,
                        XE.z() - rxOrigin.z(), 0, -XE.x() + rxOrigin.x();
                    Vector3f Bx = {
                        interaction->normal.x() * interaction->p.x() + interaction->normal.y() * interaction->p.y() + interaction->normal.z() * interaction->p.z(),
                        XE.x() * (XE.y() - rxOrigin.y()) - XE.y() * (XE.x() - rxOrigin.x()),
                        XE.x() * (XE.z() - rxOrigin.z()) - XE.z() * (XE.x() - rxOrigin.x()),
                    };
                    Vector3f px;
                    if (!ternarySystemOfLinearEquations(Ax, Bx, px)) {
                        interaction->dpdx = { 0.f, 0.f, 0.f };
                    }
                    else {
                        interaction->dpdx = px - interaction->p;
                    }
                    // dpdy
                    Vector3f ryOrigin = interaction->ray->differential.ryOrigin;
                    Vector3f ryDirection = interaction->ray->differential.ryDirection;
                    Vector3f YE = ryOrigin + ryDirection;
                    Matrix3f Ay;
                    Ay << interaction->normal.x(), interaction->normal.y(), interaction->normal.z(),
                        YE.y() - ryOrigin.y(), -YE.x() + ryOrigin.x(), 0,
                        YE.z() - ryOrigin.z(), 0, -YE.x() + ryOrigin.x();
                    Vector3f By = {
                        interaction->normal.x() * interaction->p.x() + interaction->normal.y() * interaction->p.y() + interaction->normal.z() * interaction->p.z(),
                        YE.x() * (YE.y() - ryOrigin.y()) - YE.y() * (YE.x() - ryOrigin.x()),
                        YE.x() * (YE.z() - ryOrigin.z()) - YE.z() * (YE.x() - ryOrigin.x()),
                    };
                    Vector3f py;
                    if (!ternarySystemOfLinearEquations(Ay, By, py)) {
                        interaction->dpdy = { 0.f, 0.f, 0.f };
                    }
                    else {
                        interaction->dpdy = py - interaction->p;
                    }
                    if (interaction->geometry->getTexture() != nullptr) {
                        //cout << "rxOrigin=" << vector3fToString(rxOrigin) << ", rxDirection=" << vector3fToString(rxDirection) << ", origin=" << vector3fToString(interaction->ray->origin) << endl;
                        //cout << "p=" << vector3fToString(interaction->p) << ", normal=" << vector3fToString(interaction->normal) << endl;
                        //cout << "dpdx=" << vector3fToString(interaction->dpdx) << endl;
                        //cout << "dpdy=" << vector3fToString(interaction->dpdy) << endl;
                        
                        // get dudx dudy dvdx dvdy
                        // 二元一次方程组: p' = p + du * (dpdu) + dv * (dpdv)
                        // dpdx.x = dudx * dpdu.x + dvdx * dpdv.x
                        // dpdx.y = dudx * dpdu.y + dvdx * dpdv.y
                        // dpdx.z = dudx * dpdu.z + dvdx * dpdv.z
                        // dpdy.x = dudy * dpdu.x + dvdy * dpdv.x
                        // dpdy.y = dudy * dpdu.y + dvdy * dpdv.y
                        // dpdy.z = dudy * dpdu.z + dvdy * dpdv.z
                        // select(min n) two dimensions to calculate[three equations & two variable]
                        Vector2f UV = interaction->geometry->getUV(interaction->p);
                        interaction->u = UV.x(); interaction->v = UV.y();
                        pair<Vector3f, Vector3f> dpduv = interaction->geometry->getdpduv(interaction->p);
                        interaction->dpdu = dpduv.first;
                        interaction->dpdv = dpduv.second;

                        Vector2f dim;
                        if (interaction->normal.x() > interaction->normal.y() && interaction->normal.x() > interaction->normal.z()) {
                            dim.x() = 1;
                            dim.y() = 2;
                        }
                        else if (interaction->normal.y() > interaction->normal.z()) {
                            dim.x() = 0;
                            dim.y() = 2;
                        }
                        else {
                            dim.x() = 0;
                            dim.y() = 1;
                        }
                        Vector2f Bx, By;
                        Bx.x() = interaction->dpdx[dim.x()];
                        Bx.y() = interaction->dpdx[dim.y()];
                        By.x() = interaction->dpdy[dim.x()];
                        By.y() = interaction->dpdy[dim.y()];
                        Matrix2f A;
                        A << interaction->dpdu[dim.x()], interaction->dpdv[dim.x()],
                            interaction->dpdu[dim.y()], interaction->dpdv[dim.y()];
                        Vector2f dudxANDdvdx, dudyANDdvdy;
                        if (!binarySystemOfLinearEquations(A, Bx, dudxANDdvdx)) goto textureDifferentialFailFlag;
                        if (!binarySystemOfLinearEquations(A, By, dudyANDdvdy)) goto textureDifferentialFailFlag;
                        interaction->dudx = dudxANDdvdx.x();
                        interaction->dudy = dudyANDdvdy.x();
                        interaction->dvdx = dudxANDdvdx.y();
                        interaction->dvdy = dudyANDdvdy.y();
                    }
                    else {
                    textureDifferentialFailFlag:
                        interaction->dpdx = interaction->dpdy = { 0, 0, 0 };
                        interaction->dudx = interaction->dudy = interaction->dvdx = interaction->dvdy = 0;
                    }
                }
                else {
                    interaction->dpdx = interaction->dpdy = { 0, 0, 0 };
                    interaction->dpdx = interaction->dpdy = { 0, 0, 0 };
                    interaction->dudx = interaction->dudy = interaction->dvdx = interaction->dvdy = 0;
                }
                // old version
    //            if (interaction->ray->differential.hasDifferential && interaction->geometry->getTexture() != nullptr) {
    //                Vector2f UV = interaction->geometry->getUV(interaction->p);
    //                interaction->u = UV.x(); interaction->v = UV.y();
    //                pair<Vector3f, Vector3f> dpduv = interaction->geometry->getdpduv(interaction->p);
    //                interaction->dpdu = dpduv.first;
    //                interaction->dpdv = dpduv.second;

    //                float d = interaction->normal.dot(interaction->p);
    //                cout << "time=" << interaction->time << endl;
    //                Vector3f rxOrigin = interaction->ray->differential.rxOrigin;
    //                Vector3f rxDirection = interaction->ray->differential.rxDirection;
    //                if (abs(interaction->normal.dot(rxDirection)) < EPSILON) goto textureDifferentialFailFlag;
    //                float tx = -(interaction->normal.dot(rxOrigin) - d) / interaction->normal.dot(rxDirection);
    //                Vector3f px = rxOrigin + tx * rxDirection;
    //                Vector3f ryOrigin = interaction->ray->differential.ryOrigin;
    //                Vector3f ryDirection = interaction->ray->differential.ryDirection;
    //                if (abs(interaction->normal.dot(ryDirection)) < EPSILON) goto textureDifferentialFailFlag;
    //                float ty = -(interaction->normal.dot(ryOrigin) - d) / interaction->normal.dot(ryDirection);
    //                Vector3f py = ryOrigin + ty * ryDirection;
    //                cout << "ro=" << vector3fToString(interaction->ray->origin) << ", roy=" << vector3fToString(ryOrigin) << endl;

    //                cout << "ty=" << ty << endl;
    //                interaction->dpdx = px - interaction->p;
    //                interaction->dpdy = py - interaction->p;
    //                cout << "dpdx=" << vector3fToString(interaction->dpdx) << endl;
    //                cout << "dpdy=" << vector3fToString(interaction->dpdy) << endl;

    //                // 二元一次方程组: p' = p + du * (dpdu) + dv * (dpdv)
    //                // dpdx.x = dudx * dpdu.x + dvdx * dpdv.x
    //                // dpdx.y = dudx * dpdu.y + dvdx * dpdv.y
    //                // dpdx.z = dudx * dpdu.z + dvdx * dpdv.z
    //                // dpdy.x = dudy * dpdu.x + dvdy * dpdv.x
    //                // dpdy.y = dudy * dpdu.y + dvdy * dpdv.y
    //                // dpdy.z = dudy * dpdu.z + dvdy * dpdv.z
    //                // select(min n) two dimensions to calculate[three equations & two variable]
    //                Vector2f dim;
    //                if (interaction->normal.x() > interaction->normal.y() && interaction->normal.x() > interaction->normal.z()) {
    //                    dim.x() = 1;
    //                    dim.y() = 2;
    //                }
    //                else if (interaction->normal.y() > interaction->normal.z()) {
    //                    dim.x() = 0;
    //                    dim.y() = 2;
    //                }
    //                else {
    //                    dim.x() = 0;
    //                    dim.y() = 1;
    //                }
    //                Vector2f Bx, By;
    //                Bx.x() = interaction->dpdx[dim.x()];
    //                Bx.y() = interaction->dpdx[dim.y()];
    //                By.x() = interaction->dpdy[dim.x()];
    //                By.y() = interaction->dpdy[dim.y()];
    //                Matrix2f A;
    //                A << interaction->dpdu[dim.x()], interaction->dpdv[dim.x()],
    //                     interaction->dpdu[dim.y()], interaction->dpdv[dim.y()];
    //                Vector2f dudxANDdvdx, dudyANDdvdy;
    //                if (!binarySystemOfLinearEquations(A, Bx, dudxANDdvdx)) goto textureDifferentialFailFlag;
    //                if (!binarySystemOfLinearEquations(A, By, dudyANDdvdy)) goto textureDifferentialFailFlag;
    //                interaction->dudx = dudxANDdvdx.x();
    //                interaction->dudy = dudyANDdvdy.x();
    //                interaction->dvdx = dudxANDdvdx.y();
    //                interaction->dvdy = dudyANDdvdy.y();
    //                cout << "dudx=" << interaction->dudx << endl;
    //            }
    //            else {
    //textureDifferentialFailFlag:
    //                interaction->dpdx = interaction->dpdy = { 0, 0, 0 };
    //                interaction->dudx = interaction->dudy = interaction->dvdx = interaction->dvdy = 0;
    //            }
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