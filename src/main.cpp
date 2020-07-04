
#include <fstream>

#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <Magnum/Platform/GLContext.h>

#define CGAL_TRACE_STREAM std::cerr
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/IO.h>

#include "Viewer.h"

using namespace Magnum;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Point_set_3<Point> Point_set;

int main(int argc, char** argv) {

  // Load the file
  std::ifstream stream("../data/archer_cleaned.ply");
  Point_set points;
  stream >> points;


  auto app = Gtk::Application::create(argc, argv, "JacksonCampolattaro.PLY-file-viewer");

  Platform::GLContext context{NoCreate, argc, argv};
  Viewer<Point> widget{context, points};

  Gtk::Window window{};
  window.add(widget);
  widget.show();

  return app->run(window);
}