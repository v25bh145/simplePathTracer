//
// Created by v25bh on 2021/11/23.
//

#include "camera.h"
#define THREAD_N 16

namespace pathTracer {
    DWORD WINAPI generateByThread(LPVOID lpParameter) {
        ImageFragment* imageFragment = (ImageFragment*) lpParameter;
        Camera* camera = imageFragment->camera;
        int xLow = imageFragment->widthRange.first,
            xHigh = imageFragment->widthRange.second,
            yLow = imageFragment->heightRange.first,
            yHigh = imageFragment->heightRange.second;
        Vector2f pixelSize = imageFragment->camera->pixelSize;
        for (int x = xLow; x < xHigh; ++x) {
            for (int y = yLow; y < yHigh; ++y) {
                float x_pixel = x + pixelSize.x() / 2;
                float y_pixel = y + pixelSize.y() / 2;
                // LHS
                vector<Ray*> ray_N = camera->sample_wi_LHS(x_pixel, y_pixel);
                Vector3f color_N = { 0.f, 0.f, 0.f };
                for (auto ray : ray_N) {
                    Vector3f color = camera->integrator->sample_li(camera->scene, ray);
                    color_N += color;
                    //if (color.x() > 0.f) { 
                    //    cout << "!x=" << x << ", !y=" << y << ", !color=" << vector3fToString(color) << endl; 
                    //    cout << ray->toString() << endl;
                    //}
                    delete ray;
                }
                imageFragment->pixels[x - xLow][y - yLow] = color_N / ray_N.size();
                
                // normal
                //Vector3f color_N = { 0, 0, 0 };
                //for (int i = 0; i < camera->sampleOnePixel; ++i) {
                //    Ray* ray = camera->sample_wi(x_pixel, y_pixel);
                //    Vector3f color = camera->integrator->sample_li(camera->scene, ray);
                //    color_N += color;
                //    delete ray;
                //}
                //imageFragment->pixels[x - xLow][y - yLow] = color_N / camera->sampleOnePixel;
            }
        }
        return 0;
    }
    // use integrator compute the pixels
    // x: width
    // y: height
    void Camera::generate() {
        /*
        * multi-thread
        */
        ImageFragment** imageFragment = new ImageFragment*[THREAD_N];
        for (int i = 0; i < THREAD_N; ++i) {
            imageFragment[i] = new ImageFragment(
                { resolution.x() * i / THREAD_N , resolution.x() * (i + 1) / THREAD_N },
                { 0, resolution.y() },
                this
            );
        }
        HANDLE* handle = new HANDLE[THREAD_N];
        for (int i = 0; i < THREAD_N; ++i)
            // [ first(), second() )
            handle[i] = CreateThread(NULL, 0, &generateByThread, imageFragment[i], 0, NULL);
        for (int i = 0; i < THREAD_N; ++i)
            WaitForSingleObject(handle[i], INFINITE);
        for (int i = 0; i < THREAD_N; ++i) {
            int xLow = imageFragment[i]->widthRange.first,
                xHigh = imageFragment[i]->widthRange.second,
                yLow = imageFragment[i]->heightRange.first,
                yHigh = imageFragment[i]->heightRange.second;
            for (int x = xLow; x < xHigh; ++x) {
                for (int y = yLow; y < yHigh; ++y) {
                    Vector3f color = imageFragment[i]->pixels[x - xLow][y - yLow] * 255.f;
                    pixels[resolution.x() - x - 1][resolution.y() - y - 1] = color;
                }
            }
        }
        /*
        * one-thread
        */
        //int progressBar = 0;
        //for(int x = 0; x < resolution.x(); ++x) {
        //    if (x == progressBar * resolution.x() / 10) {
        //        cout << progressBar * 10 << "%" << endl;
        //        progressBar++;
        //    }
        //    for(int y = 0; y < resolution.y(); ++y) {
        //        Ray* ray = sample_wi(x, y);
        //        Vector3f color = integrator->sample_li(scene, ray);
        //        delete ray;
        //        // reverse
        //        pixels[resolution.x() - x - 1][resolution.y() - y - 1] = color;
        //    }
        //}

        /*
        * zoom the color
        */
        //for (int x = 0; x < resolution.x(); ++x) {
        //    for (int y = 0; y < resolution.y(); ++y) {
        //        pixels[x][y] = {
        //            pixels[x][y].x() > 255.f ? 255.f : pixels[x][y].x(),
        //            pixels[x][y].y() > 255.f ? 255.f : pixels[x][y].y(),
        //            pixels[x][y].z() > 255.f ? 255.f : pixels[x][y].z()
        //        };
        //    }
        //}
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
        buffer << "pixelSize = (" << pixelSize.x() << ", " << pixelSize.y() << ")" << endl;
        return buffer.str();
    }
    // only deep copy the object, won't copy the RTCInner things
    void Camera::deepCopy(Camera*& camera)
    {
        camera = new Camera(this->origin, this->lookingAt, this->upAngle, this->f, this->fov, this->zNear, this->zFar, nullptr, this->resolution, nullptr, this->sampleOnePixel);
        this->scene->deepCopy(camera->scene);
        this->integrator->deepCopy(camera->integrator);
    }

    Ray *Camera::sample_wi(float x, float y) {
        Vector3f point = filmA + x * width * XAxle / (float)resolution.x() - y * height * upAngle / (float)resolution.y();

        Vector3f dir = origin - point;
        float dir_norm = dir.norm();
        dir.normalize();

        float tNear = zNear * dir_norm / f + dir_norm;
        float tFar = zFar * dir_norm / f + dir_norm;

        Ray *ray = new Ray(point, dir, 0, tNear, tFar);
        //cout << "generate camera ray:" << ray->toString() << endl;

        return ray;
    }
    vector<Ray*> Camera::sample_wi_LHS(float x_center, float y_center)
    {
        //cout << "(xCenter, yCenter)=(" << x_center << ", " << x_center << ")" << endl;
        //cout << "pixelsize=" << pixelSize << ", sampleOnePixel=" << sampleOnePixel << endl;
        RandomGenerator randomGenerator;
        vector<Ray*> ray_N;
        Vector2f upperLeft = { x_center - pixelSize.x() / 2, y_center - pixelSize.y() / 2 };
        Vector2f stride = pixelSize / (int)sampleOnePixel;

        vector<Vector2f> u2D = randomGenerator.uniform0To1By2D(sampleOnePixel);
        vector<int> shuffleArray = randomGenerator.shuffleN(sampleOnePixel);

        for (int i = 0; i < sampleOnePixel; ++i) {
            float x = upperLeft.x() + ((float)i + u2D[i].x()) * stride.x();
            float y = upperLeft.y() + ((float)shuffleArray[i] + u2D[i].y()) * stride.y();
            //cout << "   (x, y)=(" << x << ", " << y << ")" << endl;
            ray_N.push_back(sample_wi(x, y));
        }
        return ray_N;
    }
}