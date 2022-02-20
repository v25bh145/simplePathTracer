cmake_minimum_required(VERSION 3.19)

project(simplePathTracer)

set(CMAKE_CXX_STANDARD 14)

find_package(EMBREE 3 REQUIRED)
find_package(PNG REQUIRED)
find_package(EIGEN3 REQUIRED)

include_directories(
        ${PNG_INCLUDE_DIRS}
        ${EMBREE_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIRS}
)

set (
    EXTFILES
    ${PROJECT_SOURCE_DIR}/ext/stb_image.h
    ${PROJECT_SOURCE_DIR}/ext/stb_image.cpp
)

set(
        SRCFILES
        ${EXTFILES}
        ${PROJECT_SOURCE_DIR}/src/default.h
        ${PROJECT_SOURCE_DIR}/src/pngParser.cpp
        ${PROJECT_SOURCE_DIR}/src/pngParser.h
        ${PROJECT_SOURCE_DIR}/src/geometry.cpp
        ${PROJECT_SOURCE_DIR}/src/geometry.h
        ${PROJECT_SOURCE_DIR}/src/bxdf.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf.h
        ${PROJECT_SOURCE_DIR}/src/ray.cpp
        ${PROJECT_SOURCE_DIR}/src/ray.h
        ${PROJECT_SOURCE_DIR}/src/scene.cpp
        ${PROJECT_SOURCE_DIR}/src/scene.h
        ${PROJECT_SOURCE_DIR}/src/aggregation.cpp
        ${PROJECT_SOURCE_DIR}/src/aggregation.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdfs.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdfs.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/lambertianReflection.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/lambertianReflection.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/specularReflection.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/specularReflection.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/specularTransmission.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/specularTransmission.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/fresnelSpecular.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/fresnelSpecular.h
        ${PROJECT_SOURCE_DIR}/src/camera.cpp
        ${PROJECT_SOURCE_DIR}/src/camera.h
        ${PROJECT_SOURCE_DIR}/src/interaction.cpp
        ${PROJECT_SOURCE_DIR}/src/interaction.h
        ${PROJECT_SOURCE_DIR}/src/geometry/triangle.cpp
        ${PROJECT_SOURCE_DIR}/src/geometry/triangle.h
        ${PROJECT_SOURCE_DIR}/src/integrator.cpp
        ${PROJECT_SOURCE_DIR}/src/integrator.h
        ${PROJECT_SOURCE_DIR}/src/integrator/pathIntegrator.cpp
        ${PROJECT_SOURCE_DIR}/src/integrator/pathIntegrator.h
        ${PROJECT_SOURCE_DIR}/src/sampling.cpp
        ${PROJECT_SOURCE_DIR}/src/sampling.h
        ${PROJECT_SOURCE_DIR}/src/mathHelper.cpp
        ${PROJECT_SOURCE_DIR}/src/mathHelper.h
        ${PROJECT_SOURCE_DIR}/src/integrator/directIntegrator.h
        ${PROJECT_SOURCE_DIR}/src/integrator/directIntegrator.cpp
        ${PROJECT_SOURCE_DIR}/src/pathTracer.h
        ${PROJECT_SOURCE_DIR}/src/medium.cpp
        ${PROJECT_SOURCE_DIR}/src/medium.h
        ${PROJECT_SOURCE_DIR}/src/integrator/volumePathIntegrator.h
        ${PROJECT_SOURCE_DIR}/src/integrator/volumePathIntegrator.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/orenNayer.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/orenNayer.cpp
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/microfacet.h
        ${PROJECT_SOURCE_DIR}/src/bxdf/bsdf/microfacet.cpp
        ${PROJECT_SOURCE_DIR}/src/texture.cpp
        ${PROJECT_SOURCE_DIR}/src/texture.h
)

set (
        EXT_LIBRARIES
        ${PNG_LIBRARY}
        ${EMBREE_LIBRARIES}
)

add_subdirectory(test)