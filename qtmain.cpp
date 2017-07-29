#include "qtconceptmap.h"
#include <QApplication>
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ribi::cmap::QtConceptMap d;
  d.show();
  return a.exec();
}
