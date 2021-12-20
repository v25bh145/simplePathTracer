//
// Created by v25bh on 2021/12/1.
//

#include "pathIntegrator.h"

namespace pathTracer {

    Vector3f PathIntegrator::sample_li(Scene *scene, Ray *origin_ray) {

        //auto* hitInteraction = new Interaction(origin_ray);
        //unsigned hitGeomID = scene->intersect(hitInteraction);
        //Geometry* hitGeometry = hitInteraction->geometry;
        //if (hitGeomID != 10) return { 0, 0, 0 };
        //if (hitGeomID < 8 || hitGeomID > 11) return { 0, 0, 0 };

        Vector3f sum_L = { 0.f, 0.f, 0.f };
        for (int i = 0; i < sampleOnePixel; ++i) {
            int bounce = 0;
            bool isSpecular = false;
            Vector3f L = { 0.f, 0.f, 0.f };
            Vector3f beta = { 1.f, 1.f, 1.f };
            Ray *ray = new Ray(origin_ray);
            while (true) {
                auto* hitInteraction = new Interaction(ray);
                unsigned hitGeomID = scene->intersect(hitInteraction);
                Geometry* hitGeometry = hitInteraction->geometry;
                //cout << "   bounce=" << bounce << ", 当前交点=" << vector3fToString(hitInteraction->p) << ", id=" << hitGeomID << ", 入射方向=" << vector3fToString(hitInteraction->ray->direction) << endl;

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
                    delete hitInteraction;
                    break;
                }

                // if the geometry don't include any SPECULAR,
                // randomly choose a point from a randomly chosen light source
                // if not shaded
                // calculate the radiance from this point to the interaction point, multiply the weight beta
                if (!hitGeometry->bxdf->hasType(BxDFType::SPECULAR)) {
                    // IS
                    Vector3f ld = estimateDirect(hitInteraction, hitGeometry, scene);
                    Vector3f mul = {
                        ld.x() * beta.x(),
                        ld.y() * beta.y(),
                        ld.z() * beta.z()
                    };
                    //cout << "   直接光照=" << vector3fToString(ld) << endl;
                    //cout << "   直接光照*beta=" << vector3fToString(mul) << endl;
                    L += mul;
                }

                // randomly choose a BxDF,
                // if this BSDF has a type SPECULAR, then set the isSpecular,
                // multiply that BxDF into beta
                Ray* wi = new Ray();
                float wi_pdf;
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
                //cout << "   采样方向=" << vector3fToString(wi->direction) << "beta=" << vector3fToString(beta) << endl;
                if ((sampleType & BxDFType::SPECULAR) != 0 && (isSpecular == true || bounce == 0)) {
                    isSpecular = true;
                    //cout << "   镜面竟是我自己" << endl;
                }
                // **only when there was a specular bxdf in front of this while, isSpecular is true**
                else { 
                    isSpecular = false; 
                }

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
    void PathIntegrator::deepCopy(Integrator*& integrator)
    {
        integrator = new PathIntegrator(this->maxDepth, this->sampleOnePixel);
    }
    string PathIntegrator::toString()
    {
        string info = "";
        ostringstream buffer(info);
        buffer << "pathIntegrator: (maxDepth, sampleOnePixel)=(" << maxDepth << ", " << sampleOnePixel << ")" << endl;
        return buffer.str();
    }
}