add_executable(
        outputPngTest
        outputPngTest/outputPngTest.cpp
        ${SRCFILES}
)

target_link_libraries(
        outputPngTest
        ${EXT_LIBRARIES}
)

add_executable(
        rayTracerCoreTest
        rayTracerCoreTest/rayTracerCoreTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        rayTracerCoreTest
        ${EXT_LIBRARIES}
)

add_executable(
        sceneCreateTest
        sceneCreateTest/sceneCreateTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        sceneCreateTest
        ${EXT_LIBRARIES}
)

add_executable(
        geometryTest
        geometryTest/geometryTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        geometryTest
        ${EXT_LIBRARIES}
)

add_executable(
        rayCreateTest
        rayCreateTest/rayCreateTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        rayCreateTest
        ${EXT_LIBRARIES}
)

add_executable(
        sampleCoreTest
        sampleCoreTest/sampleCoreTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        sampleCoreTest
        ${EXT_LIBRARIES}
)

add_executable(
        randomTest
        randomTest/randomTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        randomTest
        ${EXT_LIBRARIES}
)

add_executable(
        shadowTest
        shadowTest/shadowTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        shadowTest
        ${EXT_LIBRARIES}
)

add_executable(
        samplePointTest
        samplePointTest/samplePointTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        samplePointTest
        ${EXT_LIBRARIES}
)

add_executable(
        cornellBoxTest
        cornellBoxTest/cornellBoxTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        cornellBoxTest
        ${EXT_LIBRARIES}
)

add_executable(
        lambertianTest
        lambertianTest/lambertianTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        lambertianTest
        ${EXT_LIBRARIES}
)

add_executable(
        quadTest
        quadTest/quadTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        quadTest
        ${EXT_LIBRARIES}
)

add_executable(
        windowsThreadApiTest
        windowsThreadApiTest/windowsThreadApiTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        windowsThreadApiTest
        ${EXT_LIBRARIES}
)

add_executable(
        deepCopyTest
        deepCopyTest/deepCopyTest.cpp
        ${SRCFILES}
)
target_link_libraries(
        deepCopyTest
        ${EXT_LIBRARIES}
)