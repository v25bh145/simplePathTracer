//
// Created by v25bh on 2021/11/23.
//

#include "camera.h"

namespace pathTracer {
    //DWORD WINAPI generateByThread(LPVOID lpParameter) {
    //    ImageFragment* imageFragment = (ImageFragment*) lpParameter;
    //    Camera* camera = imageFragment->camera;
    //    int xLow = imageFragment->widthRange.first,
    //        xHigh = imageFragment->widthRange.second,
    //        yLow = imageFragment->heightRange.first,
    //        yHigh = imageFragment->heightRange.second;
    //    for (int x = xLow; x < xHigh; ++x) {
    //        for (int y = yLow; y < yHigh; ++y) {
    //            Ray* ray = camera->sample_wi(x, y);
    //            imageFragment->pixels[x][y] = camera->integrator->sample_li(camera->scene, ray);
    //            delete ray;
    //        }
    //    }
    //    return 0;
    //}
    // use integrator compute the pixels
    // x: width
    // y: height
    void Camera::generate() {
        //ImageFragment* imageFragment = new ImageFragment[20];
        //for (int i = 0; i < THREAD_N; ++i) {
        //    imageFragment[i] = ImageFragment(
        //        { resolution.x() * i / THREAD_N , resolution.x() * (i + 1) / THREAD_N },
        //        { 0, resolution.y() },
        //        this
        //    );
        //}

        //HANDLE* handle = new HANDLE[THREAD_N];
        //for (int i = 0; i < THREAD_N; ++i)
        //    // [ first(), second() )
        //    handle[i] = CreateThread(NULL, 0, &generateByThread, imageFragment + i, 0, NULL);
        //for (int i = 0; i < THREAD_N; ++i)
        //    WaitForSingleObject(handle[i], INFINITE);

        //Vector3f max_color = { 0.f, 0.f, 0.f };
        //for (int i = 0; i < THREAD_N; ++i) {
        //    int xLow = imageFragment[i].widthRange.first,
        //        xHigh = imageFragment[i].widthRange.second,
        //        yLow = imageFragment[i].heightRange.first,
        //        yHigh = imageFragment[i].heightRange.second;
        //    for (int x = xLow; x < xHigh; ++x) {
        //        for (int y = yLow; y < yHigh; ++y) {
        //            Vector3f color = imageFragment[i].pixels[x][y];
        //            if (color.x() < 20.f) {
        //                max_color(0) = max_color.x() > color.x() ? max_color.x() : color.x();
        //            }
        //            if (color.y() < 20.f) {
        //                max_color(1) = max_color.y() > color.y() ? max_color.y() : color.y();
        //            }
        //            if (color.z() < 20.f) {
        //                max_color(2) = max_color.z() > color.z() ? max_color.z() : color.z();
        //            }
        //            pixels[resolution.x() - x - 1][resolution.y() - y - 1] = color;
        //        }
        //    }
        //}

        Vector3f max_color = { 0.f, 0.f, 0.f };
        for(int x = 0; x < resolution.x(); ++x) {
            for(int y = 0; y < resolution.y(); ++y) {
                Ray* ray = sample_wi(x, y);
                Vector3f color = integrator->sample_li(scene, ray);
                delete ray;
                if (color.x() < 20.f) {
                    max_color(0) = max_color.x() > color.x() ? max_color.x() : color.x();
                }
                if (color.y() < 20.f) {
                    max_color(1) = max_color.y() > color.y() ? max_color.y() : color.y();
                }
                if (color.z() < 20.f) {
                    max_color(2) = max_color.z() > color.z() ? max_color.z() : color.z();
                }
                // reverse
                pixels[resolution.x() - x - 1][resolution.y() - y - 1] = color;
            }
        }
        for (int x = 0; x < resolution.x(); ++x) {
            for (int y = 0; y < resolution.y(); ++y) {
                pixels[x][y] = {
                    pixels[x][y].x() * 255.f / max_color.x(),
                    pixels[x][y].y() * 255.f / max_color.y(),
                    pixels[x][y].z() * 255.f / max_color.z()
                };
                pixels[x][y] = {
                    pixels[x][y].x() > 255.f ? 255.f : pixels[x][y].x(),
                    pixels[x][y].y() > 255.f ? 255.f : pixels[x][y].y(),
                    pixels[x][y].z() > 255.f ? 255.f : pixels[x][y].z()
                };
            }
        }
    }

    string Camera::toString() {
        Vector3f filmB, filmC, filmD;
        filmB = filmOrigin + height * upAngle / 2 + width * XAxle / 2;
        filmC = filmOrigin - height * upAngle / 2 - width * XAxle / 2;
        filmD = filmOrigin - height * upAngle / 2 + width * XAxle / 2;
        string info = "";
        ostringstream buffer(info);
        buffer << "CameraInfo:" << endl;
        buffer << "f = " << f << endl;
        buffer << "aspect = " << aspect << ", height = " << height << ", width = " << width << endl;
        buffer << "origin = (" << origin.x() << ", " << origin.y() << ", " << origin.z() << ")" << endl;
        buffer << "lookingAt = (" << lookingAt.x() << ", " << lookingAt.y() << ", " << lookingAt.z() << ")" << endl;
        buffer << "upAngle = (" << upAngle.x() << ", " << upAngle.y() << ", " << upAngle.z() << ")" << endl;
        buffer << "XAxle = (" << XAxle.x() << ", " << XAxle.y() << ", " << XAxle.z() << ")" << endl;
        buffer << "filmOrigin = (" << filmOrigin.x() << ", " << filmOrigin.y() << ", " << filmOrigin.z() << ")" << endl;
        buffer << "filmA = (" << filmA.x() << ", " << filmA.y() << ", " << filmA.z() << ")" << endl;
        buffer << "filmB = (" << filmB.x() << ", " << filmB.y() << ", " << filmB.z() << ")" << endl;
        buffer << "filmC = (" << filmC.x() << ", " << filmC.y() << ", " << filmC.z() << ")" << endl;
        buffer << "filmD = (" << filmD.x() << ", " << filmD.y() << ", " << filmD.z() << ")" << endl;
        return buffer.str();
    }

    Ray *Camera::sample_wi(unsigned int x, unsigned int y) {
        Vector3f point = filmA + x * width * XAxle / resolution.x() - y * height * upAngle / resolution.y();

        Vector3f dir = origin - point;
        float dir_norm = dir.norm();
        dir.normalize();

        float tNear = zNear * dir_norm / f + dir_norm;
        float tFar = zFar * dir_norm / f + dir_norm;

        Ray *ray = new Ray(point, dir, 0, tNear, tFar);
        //cout << "generate camera ray:" << ray->toString() << endl;

        return ray;
    }
}