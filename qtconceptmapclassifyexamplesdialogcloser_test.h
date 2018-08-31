#ifndef QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_TEST_H
#define QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtClassifyExamplesDialogCloserTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void Modify() const noexcept;
  void PressCancel() const noexcept;
  void PressOk() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCLASSIFYEXAMPLESDIALOGCLOSER_TEST_H
