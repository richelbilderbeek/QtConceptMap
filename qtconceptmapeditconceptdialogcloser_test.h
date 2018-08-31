#ifndef QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_TEST_H
#define QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtEditConceptDialogCloserTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void PressCancel() const noexcept;
  void PressOk() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDITCONCEPTDIALOGCLOSER_TEST_H
