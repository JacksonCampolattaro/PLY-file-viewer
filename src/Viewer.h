//
// Created by jackcamp on 7/3/20.
//

#ifndef PLY_FILE_VIEWER_VIEWER_H
#define PLY_FILE_VIEWER_VIEWER_H

#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/glarea.h>

#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderer.h>

using namespace Magnum;

class Viewer: public Gtk::GLArea {
public:
  explicit Viewer(Platform::GLContext& context) : _context(context) {}

private:

  void onRealize() {

  }

  bool onRender(const Glib::RefPtr<Gdk::GLContext>& context) {

    return true;
  }

  void onResize(int width, int height) {

  }

  void onUnrealize() {

  }

  Platform::GLContext& _context;
};

#endif //PLY_FILE_VIEWER_VIEWER_H
