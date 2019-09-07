QT -= core gui

CONFIG += c++11 console
CONFIG -= app_bundle


INCLUDEPATH += ./include
DEPENDPATH  += ./include

INCLUDEPATH += ./3rdparty/font8x8/include
INCLUDEPATH += ./3rdparty/stb/include


SOURCES += \
    src/main.cpp \
    src/test_intersect.cpp \
    src/test_matrix.cpp \
    src/test_camera.cpp \
    src/rt/Camera.cpp \
    src/rt/Renderer.cpp \
    src/Image.cpp \
    src/rt/Texture/CheckedTexture.cpp \
    src/rt/Texture/FlatTexture.cpp \
    src/rt/Texture/ITexture.cpp \
    src/rt/Scene/Plane.cpp \
    src/rt/Scene/Sphere.cpp \
    src/rt/Light/ILightSoruce.cpp \
    src/rt/Light/DirectionalLight.cpp \
    src/rt/Light/PointLight.cpp \
    src/rt/Scene/Cylinder.cpp \
    src/scene_1.cpp \
    src/scene_2.cpp \
    src/scene_3.cpp \
    src/rt/Refraction.cpp \
    src/scene_4.cpp \
    src/rt/Color.cpp \
    src/rt/Material/IMaterial.cpp \
    src/rt/Material/MirrorMaterial.cpp \
    src/rt/Material/TransparentMaterial.cpp \
    src/rt/Material/OpaqueMaterial.cpp \
    src/rt/Scene/IObject.cpp \
    src/scene_text.cpp

HEADERS += \
    include/geom/Vec3.h \
    include/geom/Util.h \
    include/geom/Ray.h \
    include/geom/Intersect.h \
    include/math/Logical.h \
    include/math/Quadratic.h \
    include/geom/Mat3.h \
    include/rt/Types.h \
    include/tests.h \
    include/rt/Camera.h \
    include/geom/Transform.h \
    include/rt/Renderer.h \
    include/Image.h \
    include/rt/Texture/CheckedTexture.h \
    include/rt/Texture/FlatTexture.h \
    include/rt/Texture/ITexture.h \
    include/rt/Scene/Plane.h \
    include/rt/Scene/Sphere.h \
    include/rt/SurfaceInfo.h \
    include/rt/Light/ILightSource.h \
    include/rt/Light/DirectionalLight.h \
    include/rt/Light/PointLight.h \
    include/rt/Scene/Cylinder.h \
    include/scenes.h \
    include/geom/Optics.h \
    include/rt/Refraction.h \
    include/rt/Color.h \
    include/rt/Material/IMaterial.h \
    include/rt/Material/MirrorMaterial.h \
    include/rt/Material/TransparentMaterial.h \
    include/rt/Material/OpaqueMaterial.h \
    include/rt/Scene/IObject.h \
    include/math/Pythagoras.h
