#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <Magnum/Platform/GLContext.h>

#define CGAL_TRACE_STREAM std::cerr
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>

#include "Viewer.h"

using namespace Magnum;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Point_set_3<Point> Point_set;

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