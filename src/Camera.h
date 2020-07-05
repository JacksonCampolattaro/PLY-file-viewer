//
// Created by jackcamp on 7/5/20.
//

#ifndef PLY_FILE_VIEWER_CAMERA_H
#define PLY_FILE_VIEWER_CAMERA_H

#include <Magnum/Shaders/Phong.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;

struct Camera {

  Shaders::Phong shader;
  Matrix4 projection;
  float aspectRatio;

  void draw(GL::Mesh mesh, Matrix4 transformation, Color3 color) {

    projection =
            Matrix4::perspectiveProjection(
                    35.0_degf, aspectRatio, 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-10.0f));

    shader.setTransformationMatrix(transformation)
           .setNormalMatrix(transformation.normalMatrix())
           .setProjectionMatrix(projection)
           .draw(mesh);
  }
};

#endif //PLY_FILE_VIEWER_CAMERA_H
