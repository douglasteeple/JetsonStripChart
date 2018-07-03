#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  if (argc >= 2 && QCoreApplication::arguments().at(1) == "-v") {
      w.setverbose(true);
  }
  w.show();
  
  return a.exec();
}
