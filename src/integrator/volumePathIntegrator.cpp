//
// Created by v25bh on 2021/12/1.
//

#include "volumePathIntegrator.h"

namespace pathTracer {

    Vector3f VolumePathIntegrator::sample_li(Scene* scene, Ray* origin_ray) {

        auto* hitInteraction = new Interaction(origin_ray);
        unsigned hitGeomID = scene->intersect(hitInteraction);
        Geometry* hitGeometry = hitInteraction->geometry;
        if (hitGeomID != 9) return { 0, 0, 0 };
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

        Vector3f sum_L = { 0.f, 0.f, 0.f };
        for (int i = 0; i < sampleOnePixel; ++i) {
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
                cout << "   bounce=" << bounce << ", 当前交点=" << vector3fToString(hitInteraction->p) << ", id=" << hitGeomID << ", 入射方向=" << vector3fToString(hitInteraction->ray->direction) << endl;

                // have a medium, so can't use this hitInteraction directly
                // sample the time which ray goes
                // otherSideMedium: hitInteraction->ray->direction is on the outside of hemisphere

                if (hitGeomID > 0 && otherSideMedium(hitInteraction) != nullptr) {
                    cout << "目前正在介质中，射到边界上" << endl;
                    Medium* medium = hitGeometry->getInsideMedium();
                    // this point to hit point
                    float distance = hitInteraction->time;
                    float p_surf = medium->p_surf(distance);
                    RandomGenerator randomGenerator;
                    float u = randomGenerator.uniform0To1();

                    if (u < p_surf) {
                        // hit the surface, use the hitInteraction
                        cout << "决定从该边界出去" << endl;
                        Vector3f tr, beta_surf;
                        tr = medium->tr(distance);
                        beta_surf = tr / p_surf;
                        beta = {
                            beta.x() * beta_surf.x(),
                            beta.y() * beta_surf.y(),
                            beta.z() * beta_surf.z()
                        };
                        if (sameSideMedium(hitInteraction) != nullptr) {
                            // still in a new medium, adopt the hitInteraction, distance and p_surf
                            // estimateVolumeDirect
                            cout << "边界外还是介质(estimateVolumeDirect)" << endl;
                            inMedium = true;
                        }
                        else {
                            // outside the medium
                            // execute the normal pathTracing
                            cout << "边界外是空气(estimateDirect)" << endl;
                            inMedium = false;
                        }
                    }
                    else {
                        // still in medium, abolish the hitInteraction, distance and p_surf
                        // estimateVolumeDirect

                        cout << "决定不从该边界出去，进行线积分(estimateVolumeDirect)" << endl;
                        float p_t;
                        Vector3f sigma_s = medium->sigma_s, tr;
                        float moveDistance = medium->sample_t(p_t, tr);
                        Vector3f beta_mid = {
                            sigma_s.x() * tr.x() / moveDistance,
                            sigma_s.y() * tr.y() / moveDistance,
                            sigma_s.z() * tr.z() / moveDistance
                        };
                        beta = {
                            beta.x() * beta_mid.x(),
                            beta.y() * beta_mid.y(),
                            beta.z() * beta_mid.z()
                        };

                        delete hitInteraction;
                        hitInteraction = new Interaction(ray);
                        hitInteraction->p = ray->origin + ray->direction * moveDistance;
                        inMedium = true;
                    }
                }

                if (inMedium) {
                    // estimateVolumeDirect
                    cout << "estimateVolumeDirect" << endl;
                    // use constructed hitInteraction[not have normal, geometry*]
                    Medium* medium = hitGeometry->getInsideMedium();
                    Vector3f Ld = estimateVolumeDirect(hitInteraction, medium, scene);
                    Vector3f mul = {
                        Ld.x() * beta.x(),
                        Ld.y() * beta.y(),
                        Ld.z() * beta.z()
                    };
                    cout << "   介质直接光照=" << vector3fToString(Ld) << endl;
                    cout << "   介质直接光照*beta=" << vector3fToString(mul) << endl;
                    L += mul;

                    medium->sample_phase(hitInteraction, wi);

                    cout << "偏转方向" << vector3fToString(wi->direction) << endl;

                    isSpecular = false;
                }
                else {
                    // normal path tracer
                    cout << "estimateDirect" << endl;
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
                        Vector3f Ld = estimateDirect(hitInteraction, hitGeometry, scene);
                        Vector3f mul = {
                            Ld.x() * beta.x(),
                            Ld.y() * beta.y(),
                            Ld.z() * beta.z()
                        };
                        //cout << "   直接光照=" << vector3fToString(Ld) << endl;
                        //cout << "   直接光照*beta=" << vector3fToString(mul) << endl;
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
                    //cout << "     f=" << vector3fToString(mul) << endl;
                    mul *= abs(hitInteraction->normal.dot(wi->direction));
                    //cout << "     cos=" << abs(hitInteraction->normal.dot(wi->direction)) << endl;
                    mul /= wi_pdf;
                    //cout << "     pdf=" << wi_pdf << endl;
                    beta = {
                            beta.x() * mul.x(),
                            beta.y() * mul.y(),
                            beta.z() * mul.z()
                    };

                    cout << "   采样方向=" << vector3fToString(wi->direction) << "beta=" << vector3fToString(beta) << endl;
                    if ((sampleType & BxDFType::SPECULAR) != 0 && (isSpecular == true || bounce == 0)) {
                        isSpecular = true;
                        //cout << "   镜面竟是我自己" << endl;
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
                delete ray;
                ray = new Ray(hitInteraction->p + wi->direction * 0.1, wi->direction, 0);
                delete wi;
                delete hitInteraction;
            }
            //cout << "L=" << vector3fToString(L) << endl;
            delete ray;
            sum_L = sum_L + L;
        }
        //cout << "sum_L" << vector3fToString(sum_L) << endl;
        return sum_L / sampleOnePixel;
    }
    void VolumePathIntegrator::deepCopy(Integrator*& integrator)
    {
        integrator = new VolumePathIntegrator(this->maxDepth, this->sampleOnePixel);
    }
    string VolumePathIntegrator::toString()
    {
        string info = "";
        ostringstream buffer(info);
        buffer << "volumePathIntegrator: (maxDepth, sampleOnePixel)=(" << maxDepth << ", " << sampleOnePixel << ")" << endl;
        return buffer.str();
    }
    Vector3f estimateVolumeDirect(Interaction* p1, Medium* p1Medium, Scene* scene)
    {
        Vector3f L = { 0, 0, 0 };
        // select a light [selectLightPDF = 1 / lightsNum]
        Geometry* light = scene->aggregation->uniformRandomLight();
        // no use
        float light_area_pdf;
        Vector3f light_p = light->sample_point(light_area_pdf);
        Vector3f p1_p = p1->p + p1->normal * 0.1;
        Ray* wi = new Ray(p1_p, light_p);
        Interaction* lightInteraction = new Interaction(wi);

        Vector3f Tr = volumeVisibility(p1_p, p1Medium, light_p, light, scene);
        if (vector3fEqualTo0(Tr)) return { 0, 0, 0 };

        float light_phase_pdf;
        Vector3f Phase_light = p1Medium->phase(p1, wi, light_phase_pdf);
        float light_pdf;
        Vector3f L_light = light->le(lightInteraction, light_pdf);
        if (vector3fEqualTo0(L_light)) return { 0, 0, 0 };

        L = {
            L_light.x() * Phase_light.x() * Tr.x(),
            L_light.y() * Phase_light.y() * Tr.y(),
            L_light.z() * Phase_light.z() * Tr.z()
        };

        Vector3f p2light = lightInteraction->p - p1_p;
        L *= abs(p1->ray->direction.dot(p2light.normalized()));

        L /= light_pdf;

        // selectLightPDF = 1 / lightsNum
        L *= scene->aggregation->lights.size();
        delete wi;
        return L;
    }
    Vector3f volumeVisibility(Vector3f origin, Medium* p1Medium, Vector3f destination, Geometry* light, Scene* scene) {
        float distance = (destination - origin).norm();
        Vector3f Tr = { 1.f, 1.f, 1.f };

        float sumDistance = 0;
        Vector3f now_p = origin;
        Medium* nowMedium = p1Medium;
        unsigned nowHitGeomId = -1;
        while (sumDistance < distance && nowHitGeomId != light->getRTCInnerGeometryId()) {
            Ray* nowRay = new Ray(now_p, destination);
            Interaction* nowInteraction = new Interaction(nowRay);
            nowHitGeomId = scene->intersect(nowInteraction);
            if (nowHitGeomId < 0) {
                cout << "fatal: unexpected intersect called by volumeVisibility()" << endl;
                assert(false);
            }
            float time = nowInteraction->time;
            if (nowMedium != nullptr) {
                Vector3f nowTr = nowMedium->tr(time);
                Tr = { Tr.x() * nowTr.x(), Tr.y() * nowTr.y(), Tr.z() * nowTr.z() };
            }
            sumDistance += time + 0.1f;
            nowMedium = sameSideMedium(nowInteraction);
            if (nowInteraction->geometry->getRTCInnerGeometryId() != light->getRTCInnerGeometryId() && nowInteraction->geometry->getInsideMedium() == nullptr && nowInteraction->geometry->getOutsideMedium() == nullptr) {
                return { 0, 0, 0 };
            }
            now_p = nowInteraction->p + 0.1f * nowRay->direction;
            nowHitGeomId = nowInteraction->geometry->getRTCInnerGeometryId();
            delete nowRay;
            delete nowInteraction;
        }
        return Tr;
    }
}