//
// Created by jackcamp on 7/3/20.
//

#ifndef PLY_FILE_VIEWER_VIEWER_H
#define PLY_FILE_VIEWER_VIEWER_H

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Mesh.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>

#include <Magnum/MeshTools/Compile.h>

#include <Magnum/Primitives/Icosphere.h>

#include <Magnum/Shaders/Phong.h>

#include <Magnum/Trade/MeshData.h>

#include <CGAL/Point_set_3.h>

#include "Camera.h"

using namespace Magnum;

using namespace Magnum::Math::Literals;

struct SphereInstanceData {
  Matrix4 transformationMatrix;
  Matrix3x3 normalMatrix;
  Color3 color;
};

template<class Point>
class Viewer : public Gtk::GLArea {
public:

  explicit Viewer(Platform::GLContext &context, const CGAL::Point_set_3<Point> &points) :
          _context(context), _points(points) {

    /* Automatically re-render everything every time it needs to be drawn */
    set_auto_render();

    /* Set size requests and scaling behavior */
    set_hexpand();
    set_vexpand();
    set_size_request(800, 600);

    /* Set desired OpenGL version */
    set_required_version(4, 5);

    /* Connect signals to their respective handlers */
    signal_realize().connect(sigc::mem_fun(this, &Viewer::onRealize));
    signal_render().connect(sigc::mem_fun(this, &Viewer::onRender));
    signal_resize().connect(sigc::mem_fun(this, &Viewer::onResize));
    signal_unrealize().connect(sigc::mem_fun(this, &Viewer::onUnrealize));

  }

private:

  void onRealize() {


    /* Make sure the OpenGL context is current then configure it */
    make_current();
    _context.create();

    // Configure the graphics features
    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    size_t numPoints = _points.size();
    std::cout << "Loaded " << numPoints << " points" << std::endl;

    // Initial Configuration
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Mesh
    _sphereMesh = MeshTools::compile(Primitives::icosphereSolid(1));

    // Instanced rendering
    _sphereInstanceData = Containers::Array<SphereInstanceData>{Containers::NoInit, _points.size()};
    size_t i = 0;
    for (Point &p : _points.points()) {

      _sphereInstanceData[i].transformationMatrix =
              Matrix4::translation({p[0], p[1], p[2]}) *
              Matrix4::scaling(Vector3{0.005});
      _sphereInstanceData[i].normalMatrix =
              _sphereInstanceData[i].transformationMatrix.normalMatrix();
      _sphereInstanceData[i].color =
              Color3{0.9};

      i++;
    }
    _sphereInstanceBuffer = GL::Buffer{};
    _sphereMesh.addVertexBufferInstanced(_sphereInstanceBuffer, 1, 0,
                                         Shaders::Phong::TransformationMatrix{},
                                         Shaders::Phong::NormalMatrix{},
                                         Shaders::Phong::Color3{});
    _sphereMesh.setInstanceCount(_sphereInstanceData.size());

    // Camera
    _camera = new Camera();
    _camera->shader = Shaders::Phong{
            Shaders::Phong::Flag::VertexColor |
            Shaders::Phong::Flag::InstancedTransformation};
  }

  bool onRender(const Glib::RefPtr<Gdk::GLContext> &context) {

    /* Reset state to avoid Gtkmm affecting Magnum */
    GL::Context::current().resetState(GL::Context::State::ExitExternal);

    /* Retrieve the ID of the relevant framebuffer */
    GLint framebufferID;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebufferID);

    /* Attach Magnum's framebuffer manager to the framebuffer provided by Gtkmm */
    auto gtkmmDefaultFramebuffer = GL::Framebuffer::wrap(framebufferID, {{},
                                                                         {get_width(), get_height()}});

    /* Clear the frame */
    gtkmmDefaultFramebuffer.clear(GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */

    _camera->projection =
            Matrix4::perspectiveProjection(
                    35.0_degf, _aspectRatio, 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-35.0f + _zoom));

    _sphereInstanceBuffer.setData(_sphereInstanceData, GL::BufferUsage::DynamicDraw);
    _camera->draw(_sphereMesh);

    _zoom += 0.01;
    queue_render();

    /* Clean up Magnum state and back to Gtkmm */
    GL::Context::current().resetState(GL::Context::State::EnterExternal);
    return true;
  }

  void onResize(int width, int height) {
    _aspectRatio = (float) width / (float) height;
  }

  void onUnrealize() {
    /* TODO: Add your clean-up code here */
  }

  Platform::GLContext &_context;

  GL::Mesh _sphereMesh{NoCreate};
  GL::Buffer _sphereInstanceBuffer{NoCreate};
  Containers::Array<SphereInstanceData> _sphereInstanceData;
  Camera *_camera;

  float _aspectRatio;
  float _zoom = 0;

  const CGAL::Point_set_3<Point> &_points;

};

#endif //PLY_FILE_VIEWER_VIEWER_H
