//
// Created by jackcamp on 7/3/20.
//

#ifndef PLY_FILE_VIEWER_VIEWER_H
#define PLY_FILE_VIEWER_VIEWER_H

#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

#include <CGAL/Point_set_3.h>

using namespace Magnum;

template<class Point>
class Viewer : public Gtk::GLArea {
public:

  explicit Viewer(Platform::GLContext &context, CGAL::Point_set_3<Point> points) :
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

    /* TODO: Add your initialization code here */
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

    /* Clean up Magnum state and back to Gtkmm */
    GL::Context::current().resetState(GL::Context::State::EnterExternal);
    return true;
  }

  void onResize(int width, int height) {
    /* TODO: Add your window-resize handling code here */
  }

  void onUnrealize() {
    /* TODO: Add your clean-up code here */
  }

  Platform::GLContext &_context;

  CGAL::Point_set_3<Point> _points;

};

#endif //PLY_FILE_VIEWER_VIEWER_H
