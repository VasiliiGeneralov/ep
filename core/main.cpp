#include "main_window/main_window.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  auto *window = new ep::MainWindow();
  window->resize(1280, 720);

  window->show();

  return app.exec();
}
