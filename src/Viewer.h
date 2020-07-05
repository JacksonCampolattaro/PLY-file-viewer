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

#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>

#include <Magnum/Primitives/Icosphere.h>

#include <Magnum/Shaders/Phong.h>

#include <Magnum/Trade/MeshData.h>

#include <CGAL/Point_set_3.h>

using namespace Magnum;

using namespace Magnum::Math::Literals;

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
    set_halign(Gtk::ALIGN_FILL);
    set_valign(Gtk::ALIGN_FILL);
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
    _mesh = new GL::Mesh();
    Trade::MeshData sphere = Primitives::icosphereSolid(3);
    GL::Buffer vertices;
    vertices.setData(MeshTools::interleave(sphere.positions3DAsArray(), sphere.normalsAsArray()));
    std::pair<Containers::Array<char>, MeshIndexType> compressed = MeshTools::compressIndices(sphere.indicesAsArray());
    GL::Buffer indices;
    indices.setData(compressed.first);
    _mesh->setPrimitive(sphere.primitive())
            .setCount(sphere.indexCount())
            .addVertexBuffer(std::move(vertices), 0, Shaders::Phong::Position{},
                             Shaders::Phong::Normal{})
            .setIndexBuffer(std::move(indices), 0, compressed.second);

    // Color
    _color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});

    // Shader
    _shader = new Shaders::Phong();
    _shader->setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setDiffuseColor(_color)
            .setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}));
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


    std::cout << "Preparing primitive" << std::endl;



    _transformation =
            Matrix4::rotationX(30.0_degf) * Matrix4::rotationY(40.0_degf);
    _projection =
            Matrix4::perspectiveProjection(
                    35.0_degf, _aspectRatio, 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-10.0f));

    _shader->setTransformationMatrix(_transformation)
            .setNormalMatrix(_transformation.normalMatrix())
            .setProjectionMatrix(_projection)
            .draw(*_mesh);

    std::cout << "Done" << std::endl;

//    GL::Mesh mesh;
//    Shaders::Phong shader;
//    Color3 color;
//    Matrix4 transformation, projection;

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

  Matrix4 _transformation, _projection;
  Color3 _color;
  GL::Mesh *_mesh;
  Shaders::Phong *_shader;

  float _aspectRatio = 1;

  const CGAL::Point_set_3<Point> &_points;

};

#endif //PLY_FILE_VIEWER_VIEWER_H
