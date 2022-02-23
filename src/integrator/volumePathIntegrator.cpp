//
// Created by v25bh on 2021/12/1.
//

#include "volumePathIntegrator.h"

namespace pathTracer {

    Vector3f VolumePathIntegrator::sample_li(Scene* scene, Ray* origin_ray) {

        //auto* hitInteraction = new Interaction(origin_ray);
        //unsigned hitGeomID = scene->intersect(hitInteraction);
        //Geometry* hitGeometry = hitInteraction->geometry;
        //if (hitGeomID != 3 && hitGeomID != 4) return { 0, 0, 0 };
        //delete hitInteraction;
        //else {
        //    Ray* wi = new Ray();
        //    float wi_pdf;
        //    int sampleType;
        //    Vector3f mul = hitGeometry->bxdf->sample_f(hitInteraction, wi, wi_pdf, sampleType);
        //    Ray* ray2 = new Ray(hitInteraction->p + wi->direction * 0.1, wi->direction, 0);
        //    auto* hitInteraction2 = new Interaction(ray2);
        //    unsigned hitGeomID2 = scene->intersect(hitInteraction2);
        //    if (hitGeomID2 != 12) return { 0, 0, 0 };
        //}
        int bounce = 0;
        bool isSpecular = false;
        Vector3f L = { 0.f, 0.f, 0.f };
        Vector3f beta = { 1.f, 1.f, 1.f };
        Ray* ray = new Ray(origin_ray);
        while (true) {
            // if have not medium, it will hit
            auto* hitInteraction = new Interaction(ray);
            unsigned hitGeomID = scene->intersect(hitInteraction);
            Geometry* hitGeometry = hitInteraction->geometry;
            Ray* wi = new Ray();

            bool inMedium = false;
            //cout << "   bounce=" << bounce << ", 当前交点=" << vector3fToString(hitInteraction->p) << ", id=" << hitGeomID << ", 入射方向=" << vector3fToString(hitInteraction->ray->direction) << endl;

            // have a medium, so can't use this hitInteraction directly
            // sample the time which ray goes
            // otherSideMedium: hitInteraction->ray->direction is on the outside of hemisphere

            if (hitGeomID > 0 && otherSideMedium(hitInteraction) != nullptr) {
                //cout << "目前正在介质中，射到边界上" << endl;
                Medium* medium = hitGeometry->getInsideMedium();
                // this point to hit point
                float distance = hitInteraction->time;
                float t_pdf;
                Vector3f tr;
                float moveDistance = medium->sample_t(t_pdf, tr);
                //cout << "预测移动的距离=" << moveDistance << ", 实际距离=" << distance << endl;
                // sample t
                if (moveDistance > distance) {
                    // hit the surface, use the hitInteraction
                    //cout << "决定从该边界出去" << endl;
                    // reset t_pdf & tr
                    t_pdf = medium->p_surf(distance);
                    tr = medium->tr(distance);

                    Vector3f beta_surf;
                    beta_surf = tr / t_pdf;
                    //cout << "   tr=" << vector3fToString(tr) << ", t_pdf=" << t_pdf << endl;
                    beta = {
                        beta.x() * beta_surf.x(),
                        beta.y() * beta_surf.y(),
                        beta.z() * beta_surf.z()
                    };
                    //cout << "beta_surf=" << vector3fToString(beta_surf) << ", beta更新为" << vector3fToString(beta) << endl;
                    if (sameSideMedium(hitInteraction) != nullptr) {
                        // still in a new medium, adopt the hitInteraction, distance and p_surf
                        // estimateVolumeDirect
                        //cout << "边界外还是介质(estimateVolumeDirect)" << endl;
                        inMedium = true;
                    }
                    else {
                        // outside the medium
                        // execute the normal pathTracing
                        //cout << "边界外是空气(estimateDirect)" << endl;
                        inMedium = false;
                    }
                }
                else {
                    // still in medium, abolish the hitInteraction, distance and p_surf
                    // estimateVolumeDirect
                    //cout << "决定不从该边界出去，进行线积分(estimateVolumeDirect)" << endl;
                    Vector3f sigma_s = medium->sigma_s;
                    Vector3f beta_mid = {
                        sigma_s.x() * tr.x() / t_pdf,
                        sigma_s.y() * tr.y() / t_pdf,
                        sigma_s.z() * tr.z() / t_pdf
                    };
                    //cout << "   tr=" << vector3fToString(tr) << ", t_pdf=" << t_pdf << ", sigma_s=" << vector3fToString(sigma_s) << endl;
                    beta = {
                        beta.x() * beta_mid.x(),
                        beta.y() * beta_mid.y(),
                        beta.z() * beta_mid.z()
                    };
                    //cout << "beta_mid=" << vector3fToString(beta_mid) << ", beta更新为" << vector3fToString(beta) << endl;

                    delete hitInteraction;
                    hitInteraction = new Interaction(ray);
                    //cout << "moveDistance=" << moveDistance << endl;
                    hitInteraction->p = ray->origin + ray->direction * moveDistance;
                    inMedium = true;
                }
            }

            if (inMedium) {
                // estimateVolumeDirect
                //cout << "estimateVolumeDirect" << endl;
                // use constructed hitInteraction[not have normal, geometry*]
                Medium* medium = hitGeometry->getInsideMedium();
                Vector3f Ld = estimateVolumeDirect(hitInteraction, nullptr, scene, medium);
                Vector3f mul = {
                    Ld.x() * beta.x(),
                    Ld.y() * beta.y(),
                    Ld.z() * beta.z()
                };
                //cout << "   介质直接光照=" << vector3fToString(Ld) << endl;
                //cout << "   介质直接光照*beta=" << vector3fToString(mul) << endl;
                L += mul;

                medium->sample_phase(hitInteraction, wi);

                //cout << "偏转方向" << vector3fToString(wi->direction) << endl;

                isSpecular = false;
            }
            else {
                // normal path tracer
                //cout << "estimateVolumeDirect(null)" << endl;
                // if this is the first ray OR the last ray sampled has a type SPECULAR
                if (bounce == 0 || isSpecular) {
                    // if the ray hit a geometry, add this geometry's emitLight
                    if (hitGeomID != RTC_INVALID_GEOMETRY_ID && hitGeomID > 0) {
                        Vector3f sum = {
                                hitGeometry->emitLight.x() * beta.x(),
                                hitGeometry->emitLight.y() * beta.y(),
                                hitGeometry->emitLight.z() * beta.z()
                        };
                        //cout << "   自身光源*beta=" << vector3fToString(sum) << endl;
                        // texture color
                        if (hitGeometry->getTexture() != nullptr) {
                            Vector2f UV = hitGeometry->getUV(hitInteraction->p);
                            // input: du, dv
                            float texelSize = hitGeometry->getTexelSize(hitInteraction->p);
                            Vector4f color = hitGeometry->getTexture()->mapping(UV, hitInteraction->getDuDxy(), hitInteraction->getDvDxy(), texelSize);
                            sum = { sum.x() * color.x(), sum.y() * color.y(), sum.z() * color.z() };
                        }
                        L += sum;
                    }
                    // if the ray don't hit any geometry, break this endless while
                }

                // if the ray don't hit any geometry OR the depth exceeds maxDepth
                // break this endless while
                if (hitGeomID == RTC_INVALID_GEOMETRY_ID || hitGeomID <= 0 || bounce > maxDepth) {
                    delete wi;
                    delete hitInteraction;
                    break;
                }

                // if the geometry don't include any SPECULAR,
                // randomly choose a point from a randomly chosen light source
                // if not shaded
                // calculate the radiance from this point to the interaction point, multiply the weight beta
                if (!hitGeometry->bxdf->hasType(BxDFType::SPECULAR)) {
                    // IS
                    Vector3f Ld = estimateVolumeDirect(hitInteraction, hitGeometry, scene, nullptr);
                    Vector3f mul = {
                        Ld.x() * beta.x(),
                        Ld.y() * beta.y(),
                        Ld.z() * beta.z()
                    };
                    //cout << "   直接光照=" << vector3fToString(Ld) << endl;
                    //cout << "   直接光照*beta=" << vector3fToString(mul) << endl;
                    // texture color
                    if (hitGeometry->getTexture() != nullptr) {
                        Vector2f UV = hitGeometry->getUV(hitInteraction->p);
                        // input: du, dv
                        float texelSize = hitGeometry->getTexelSize(hitInteraction->p);
                        Vector4f color = hitGeometry->getTexture()->mapping(UV, hitInteraction->getDuDxy(), hitInteraction->getDvDxy(), texelSize);
                        mul = { mul.x() * color.x(), mul.y() * color.y(), mul.z() * color.z() };
                    }
                    L += mul;
                }
                // randomly choose a BxDF,
                // if this BSDF has a type SPECULAR, then set the isSpecular,
                // multiply that BxDF into beta
                float wi_pdf = 0.f;
                int sampleType;
                Vector3f mul = hitGeometry->bxdf->sample_f(hitInteraction, wi, wi_pdf, sampleType);
                if (vector3fEqualTo0(mul)) {
                    // end
                    delete wi;
                    delete hitInteraction;
                    break;
                }
                ////cout << "     f=" << vector3fToString(mul) << endl;
                mul *= abs(hitInteraction->normal.dot(wi->direction));
                ////cout << "     cos=" << abs(hitInteraction->normal.dot(wi->direction)) << endl;
                mul /= wi_pdf;
                ////cout << "     pdf=" << wi_pdf << endl;
                // texture color
                if (hitGeometry->getTexture() != nullptr) {
                    Vector2f UV = hitGeometry->getUV(hitInteraction->p);
                    // input: du, dv
                    float texelSize = hitGeometry->getTexelSize(hitInteraction->p);
                    Vector4f color = hitGeometry->getTexture()->mapping(UV, hitInteraction->getDuDxy(), hitInteraction->getDvDxy(), texelSize);
                    beta = { beta.x() * color.x(), beta.y() * color.y(), beta.z() * color.z() };
                }
                beta = {
                        beta.x() * mul.x(),
                        beta.y() * mul.y(),
                        beta.z() * mul.z()
                };

                ////cout << "   采样方向=" << vector3fToString(wi->direction) << "beta=" << vector3fToString(beta) << endl;
                if ((sampleType & BxDFType::SPECULAR) != 0 && (isSpecular == true || bounce == 0)) {
                    isSpecular = true;
                    ////cout << "   镜面竟是我自己" << endl;
                }
                // **only when there was a specular bxdf in front of this while, isSpecular is true**
                else {
                    isSpecular = false;
                }
            }


            beta = {
                beta.x() > 1 ? 1 : beta.x(),
                beta.y() > 1 ? 1 : beta.y(),
                beta.z() > 1 ? 1 : beta.z()
            };
            // if the depth > 3, then apply RR q = max(0.05, 1 - max{beta's specular})
            // if pass Russian Roulette, then beta *= 1 / (1 - q), continue the endless while
            if (bounce > 3) {
                float beta_max = max(max(beta.x(), beta.y()), beta.z());
                float q = max(0.05f, 1.f - beta_max);
                RandomGenerator randomGenerator;
                float random_float = randomGenerator.uniform0To1();
                if (random_float > q) {
                    // continue
                    beta *= 1 / (1 - q);
                }
                else {
                    // end
                    delete wi;
                    delete hitInteraction;
                    break;
                }
            }


            // update the bounce
            bounce++;
            //cout << ray->toString() << endl;
            
            // ray propogation
            bool differentialFlag = ray->differential.hasDifferential;
            delete ray;
            Vector3f newRayOrigin = hitInteraction->p + wi->direction * 0.0005f;
            ray = new Ray(newRayOrigin, wi->direction, 0);
            if (differentialFlag) {
                Vector3f newRayOriginX = hitInteraction->p + hitInteraction->dpdx + wi->direction * 0.1;
                Vector3f newRayOriginY = hitInteraction->p + hitInteraction->dpdy + wi->direction * 0.1;
                ray->setDifferential({ newRayOriginX, newRayOriginY }, { wi->direction, wi->direction });
            }

            delete wi;
            delete hitInteraction;
        }
        //cout << "       L=" << vector3fToString(L) << endl;
        delete ray;
        return L;
    }
    void VolumePathIntegrator::deepCopy(Integrator*& integrator)
    {
        integrator = new VolumePathIntegrator(this->maxDepth);
        integrator->pixelSize = this->pixelSize;
    }
    string VolumePathIntegrator::toString()
    {
        string info = "";
        ostringstream buffer(info);
        buffer << "volumePathIntegrator: maxDepth=" << maxDepth << endl;
        return buffer.str();
    }
    Vector3f estimateVolumeDirect(Interaction* p1, Geometry* p1Geometry, Scene* scene, Medium* p1Medium = nullptr)
    {
        Vector3f L = { 0, 0, 0 };
        // select a light [selectLightPDF = 1 / lightsNum]
        Geometry* light = scene->aggregation->uniformRandomLight();
        // no use
        float light_area_pdf;
        Vector3f light_p = light->sample_point(light_area_pdf);
        Vector3f dir = (light_p - p1->p).normalized();
        Vector3f p1_p = p1->p + dir * 0.0005f;
        Ray* wi = new Ray(p1_p, light_p);
        Interaction* lightInteraction = new Interaction(wi);
        
        lightInteraction->p = light_p;
        lightInteraction->geometry = light;
        // update lightInteraction->normal
        Vector3f Tr;
        if (p1Geometry && p1Geometry->getRTCInnerGeometryId() == light->getRTCInnerGeometryId())
            Tr = { 1.f, 1.f, 1.f };
        else
            Tr = volumeVisibility(p1_p, p1Medium, light_p, lightInteraction, scene);
        if (vector3fEqualTo0(Tr)) {
            //cout << "Tr=0.f, 退出" << endl;
            delete wi;
            delete lightInteraction;
            return { 0, 0, 0 };
        };

        Vector3f p2light = lightInteraction->p - p1_p;
        float light_pdf;
        Vector3f L_light = light->le(p1, lightInteraction, light_pdf);
        if (vector3fEqualTo0(L_light)) {
            //cout << "L_light=0.f, 退出" << endl;
            delete wi;
            delete lightInteraction;
            return { 0, 0, 0 };
        }

        if (p1Geometry == nullptr) {
            // point in the geometry
            float light_phase_pdf = p1Medium->phase(p1, wi);
            Vector3f Phase_light = { light_phase_pdf , light_phase_pdf , light_phase_pdf };
            L = {
                L_light.x() * Phase_light.x() * Tr.x(),
                L_light.y() * Phase_light.y() * Tr.y(),
                L_light.z() * Phase_light.z() * Tr.z()
            };
            L *= abs(p1->ray->direction.dot(p2light.normalized()));
        }
        else {
            // point in geometry's surface
            float light_bxdf_pdf;
            Vector3f F_light = p1Geometry->bxdf->f(p1, wi, light_bxdf_pdf);
            //light_bxdf_pdf = abs(p1->normal.dot(p2light.normalized())) / (2 * PI);
            L = {
                L_light.x() * F_light.x() * Tr.x(),
                L_light.y() * F_light.y() * Tr.y(),
                L_light.z() * F_light.z() * Tr.z()
            };
            L *= abs(p1->normal.dot(p2light.normalized()));
        }

        L /= light_pdf;

        //cout << "L_light=" << vector3fToString(L_light) << ", Phase_light=" << light_phase_pdf << ", Tr=" << vector3fToString(Tr) << endl;
        //cout << "costheta=" << abs(p1->ray->direction.dot(p2light.normalized())) << ", light_pdf=" << light_pdf << endl;
        // selectLightPDF = 1 / lightsNum
        L *= scene->aggregation->lights.size();
        // TODO: 数值修正-10倍
        L *= 10.f;
        delete wi;
        delete lightInteraction;
        return L;
    }
    Vector3f volumeVisibility(Vector3f origin, Medium* p1Medium, Vector3f destination, Interaction* lightInteraction, Scene* scene) {
        Geometry* light = lightInteraction->geometry;
        float distance = (destination - origin).norm();
        Vector3f Tr = { 1.f, 1.f, 1.f };

        float sumDistance = 0;
        Vector3f now_p = origin;
        Medium* nowMedium = p1Medium;
        unsigned nowHitGeomId = -1;
        //cout << "起始点" << vector3fToString(now_p) << ", 目标:" << vector3fToString(destination) << ", 距离=" << distance << endl;
        //cout << "开始迭代" << endl;
        while (sumDistance < distance && nowHitGeomId != light->getRTCInnerGeometryId()) {
            Ray* nowRay = new Ray(now_p, destination);
            Interaction* nowInteraction = new Interaction(nowRay);
            nowHitGeomId = scene->intersect(nowInteraction);
            if (nowHitGeomId <= 0) {
                // parallel to the Quad[or light defined by delta]
                //cout << "fatal: unexpected intersect called by volumeVisibility()" << endl;
                //assert(false);
                delete nowRay;
                delete nowInteraction;
                return { 0, 0, 0 };
            }
            float time = nowInteraction->time;
            //cout << "id=" << nowHitGeomId << endl;
            if (nowMedium != nullptr) {
                Vector3f nowTr = nowMedium->tr(time);
                //cout << "time=" << time << ", nowTr=" << vector3fToString(nowTr) << endl;
                Tr = { Tr.x() * nowTr.x(), Tr.y() * nowTr.y(), Tr.z() * nowTr.z() };
            }
            sumDistance += time + 0.0005f;
            //cout << "距离=" << time << "已走距离=" << sumDistance << endl;
            nowMedium = sameSideMedium(nowInteraction);
            //if (nowMedium != nullptr)
            //    cout << nowMedium->toString() << endl;
            //else
            //    cout << "外侧空介质" << endl;
            if (nowInteraction->geometry->getRTCInnerGeometryId() != light->getRTCInnerGeometryId() && nowInteraction->geometry->getInsideMedium() == nullptr && nowInteraction->geometry->getOutsideMedium() == nullptr) {
                delete nowRay;
                delete nowInteraction;
                return { 0, 0, 0 };
            }
            now_p = nowInteraction->p + 0.0005f * nowRay->direction;
            nowHitGeomId = nowInteraction->geometry->getRTCInnerGeometryId();
            if (nowHitGeomId == light->getRTCInnerGeometryId()) lightInteraction->normal = nowInteraction->normal;
            delete nowRay;
            delete nowInteraction;
        }
        //cout << "结束迭代, Tr=" << vector3fToString(Tr) << endl;
        return Tr;
    }
}