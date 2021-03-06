//
// Created by jackcamp on 7/5/20.
//

#ifndef PLY_FILE_VIEWER_CAMERA_H
#define PLY_FILE_VIEWER_CAMERA_H

#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;

class Camera {

public:

  Shaders::Phong shader;
  Matrix4 projection;

  void draw(GL::Mesh &mesh) {

    shader.setProjectionMatrix(projection)
            .draw(mesh);
  }
};

#endif //PLY_FILE_VIEWER_CAMERA_H
