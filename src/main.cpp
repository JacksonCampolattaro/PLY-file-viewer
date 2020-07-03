#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <Magnum/Platform/GLContext.h>

#include "Viewer.h"

using namespace Magnum;

int main(int argc, char** argv) {

  Platform::GLContext context{NoCreate, argc, argv};

  /* Create an application with a reasonable application ID */
  auto app = Gtk::Application::create(argc, argv, "JacksonCampolattaro.PLY-file-viewer");

  /* Create a window for the application */
  Gtk::Window window{};

  /* Create a graphics widget and add it to the window */
  Viewer widget{context};
  window.add(widget);
  widget.show();

  /* Hand over control to Gtk */
  return app->run(window);
}