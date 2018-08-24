#ifndef QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_TEST_H
#define QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRateConceptDialogCloserTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void PressCancel() const noexcept;
  void PressOk() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOGCLOSER_TEST_H
