#ifndef QTCONCEPTMAP_CONCEPTDIALOG_TEST_H
#define QTCONCEPTMAP_CONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void just_show();
  void set_ui_name();
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_CONCEPTDIALOG_TEST_H
