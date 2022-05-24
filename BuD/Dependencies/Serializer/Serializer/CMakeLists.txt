set(LIBRARY_NAME MGSerializer)

set(MODEL_FILES
        Models/Curves/Bezier.h
        Models/Curves/BezierC0.cpp
        Models/Curves/BezierC0.h
        Models/Curves/InterpolatedC2.cpp
        Models/Curves/InterpolatedC2.h
        Models/Curves/BezierC2.cpp
        Models/Curves/BezierC2.h
        Models/Surfaces/BezierPatch.h
        Models/Surfaces/BezierPatchC0.h
        Models/Surfaces/BezierPatchC2.h
        Models/Surfaces/BezierPatchC0.cpp
        Models/Surfaces/BezierPatchC2.cpp
        Models/Surfaces/BezierSurfaceC0.h
        Models/Surfaces/BezierSurfaceC2.h
        Models/Surfaces/BezierSurfaceC0.cpp
        Models/Surfaces/BezierSurfaceC2.cpp
        Models/PointRef.cpp
        Models/PointRef.h
        Models/SceneObject.h
        Models/SceneObject.cpp
        Models/Point.cpp
        Models/Torus.h
        Models/Point.h
        Models/Torus.cpp
)

set(SCENE_FILES
        Scene/SerializerException.h
        Scene/SceneSerializer.h
        Scene/Scene.h
        Scene/SceneSerializer.cpp
        Scene/Scene.cpp
)

set(UTILS_FILES
        Utils/Uint2.cpp
        Utils/Float3.h
        Utils/Uint2.h
        Utils/Float3.cpp
)

set(RESOURCES_FILES
        Resources/Schema.h
)

set(LIBRARY_FILES
    ${MODEL_FILES}
    ${SCENE_FILES}
    ${UTILS_FILES}
    ${RESOURCES_FILES}
    pch.cpp
    pch.h
    Serializable.h
    Serializer.h
)
add_library(${LIBRARY_NAME} STATIC ${LIBRARY_FILES} )

# To jedyne co przyszło mi do głowy aby kopiowac wszystkie headery do build dira
# Jak ktos będzie miec lepszy pomysł jak poprawic zależności pomiędzy Sandboxem a Serializerem to prosze bardzo
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${PROJECT_BINARY_DIR}/include
        FILES_MATCHING
        PATTERN *.h
        )
