#ifndef QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapratedconceptdialog_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void do_resize_lists();
  void hide_rating();
  void show_example();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
