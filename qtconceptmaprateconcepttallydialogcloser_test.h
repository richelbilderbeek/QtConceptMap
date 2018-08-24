#ifndef QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_TEST_H
#define QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRateConceptTallyDialogCloserTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void PressCancel() const noexcept;
  void PressOk() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTTALLYDIALOGCLOSER_TEST_H
