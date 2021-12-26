//
// Created by v25bh on 2021/11/23.
//

#ifndef SIMPLEPATHTRACER_CAMERA_H
#define SIMPLEPATHTRACER_CAMERA_H

#include "default.h"
#include "ray.h"
#include "scene.h"
#include "integrator.h"

namespace pathTracer {
    class Ray;
    class Scene;
    class Integrator;

    class Camera {
    public:
//        Matrix4f *obj2World;
        Vector3f origin;
        // normalized
        Vector3f lookingAt;
        // normalized
        // angle rotated when shooting [Angle System]
        Vector3f upAngle;
        int sampleOnePixel;
        float fov;
        float zNear;
        float zFar;
        float f;
        float height;
        float width;
        float aspect;
        Vector2f pixelSize;
        Vector3f XAxle;
        Vector3f filmOrigin;
        Vector3f filmA;
        Vector2i resolution;
        Vector3f **pixels;
        Integrator *integrator;
        Scene *scene;

        Camera(Vector3f origin, Vector3f lookingAt, Vector3f upAngle, float f, float fov, float zNear, float zFar, Scene *scene, Vector2i resolution, Integrator *integrator, int sampleOnePixel)
        :origin(origin), lookingAt(lookingAt.normalized()), upAngle(upAngle.normalized()), f(f), fov(fov),zNear(zNear),zFar(zFar), scene(scene),resolution(resolution),integrator(integrator), sampleOnePixel(sampleOnePixel > 0 ? sampleOnePixel : 1){
            pixels = new Vector3f*[this->resolution.x()];
            for(int i = 0; i < this->resolution.x(); ++i)
                pixels[i] = new Vector3f[this->resolution.y()];

            aspect = (float)resolution.x() / (float)resolution.y(); // width / height
            height = 2.f * f * (float)tan(fov / 2.f);
            width = height * aspect;

            XAxle = lookingAt.cross(upAngle);
            XAxle.normalize();
            filmOrigin = origin - f * lookingAt;
            filmA = filmOrigin + height * upAngle / 2 - width * XAxle / 2;

            Vector3f filmB, filmC, filmD;
            filmB = filmOrigin + height * upAngle / 2 + width * XAxle / 2;
            filmC = filmOrigin - height * upAngle / 2 - width * XAxle / 2;
            filmD = filmOrigin - height * upAngle / 2 + width * XAxle / 2;

            pixelSize.x() = width / resolution.y();
            pixelSize.y() = height / resolution.x();
        };

        Ray *sample_wi(float x, float y);
        vector<Ray*> sample_wi_LHS(float x_center, float y_center);

        void generate();

        string toString();

        void deepCopy(Camera*& camera);
    };
    class ImageFragment {
    public:
        Camera* camera;
        pair<unsigned, unsigned> widthRange;
        pair<unsigned, unsigned> heightRange;
        Vector3f** pixels;
        ImageFragment(){}
        ImageFragment(pair<unsigned, unsigned> widthRange, pair<unsigned, unsigned> heightRange, Camera* camera): widthRange(widthRange), heightRange(heightRange) {
            this->camera = nullptr;
            camera->deepCopy(this->camera);
            int widthFragment = this->widthRange.second - this->widthRange.first;
            int heightFragment = this->heightRange.second - this->heightRange.first;
            pixels = new Vector3f*[widthFragment];
            for (int i = 0; i < widthFragment; ++i) {
                pixels[i] = new Vector3f[heightFragment];
            }
        }
    };
}

#endif //SIMPLEPATHTRACER_CAMERA_H
