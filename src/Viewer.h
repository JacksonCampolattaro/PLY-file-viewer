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

    std::cout << _points.size() << " points" << std::endl;

    // Initial Configuration
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Mesh
    _sphereMesh = MeshTools::compile(Primitives::icosphereSolid(0));

    // Camera
    _camera = new Camera();
    _camera->shader.setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setAmbientColor(Color3{0.4f});
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

    for (Point &p : _points.points()) {

      //Color3 color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});
      Color3 color = Color3::fromSrgb({0.9, 0.9, 0.9});

      Matrix4 transformation =
              Matrix4::translation({p[0], p[1], p[2]}) * Matrix4::scaling({0.05, 0.05, 0.05});
      _camera->draw(_sphereMesh, transformation, color);
    }

    /* Clean up Magnum state and back to Gtkmm */
    GL::Context::current().resetState(GL::Context::State::EnterExternal);
    return true;
  }

  void onResize(int width, int height) {
    _camera->projection =
            Matrix4::perspectiveProjection(
                    35.0_degf, (float) width / (float) height, 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-35.0f));
  }

  void onUnrealize() {
    /* TODO: Add your clean-up code here */
  }

  Platform::GLContext &_context;

  GL::Mesh _sphereMesh{NoCreate};
  Containers::Array<SphereInstanceData> _sphereInstanceData;
  GL::Buffer _sphereInstanceBuffer{NoCreate};
  Camera *_camera;

  const CGAL::Point_set_3<Point> &_points;

};

#endif //PLY_FILE_VIEWER_VIEWER_H
