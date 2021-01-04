#include <QApplication>
#include "DarkStyle.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setStyle(new DarkStyle);
  MainWindow mainWindow;
  mainWindow.show();
 return a.exec();
}
