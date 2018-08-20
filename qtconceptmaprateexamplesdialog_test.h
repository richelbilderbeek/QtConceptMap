#ifndef qtconceptmaprateexamplesdialog_test_H
#define qtconceptmaprateexamplesdialog_test_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRateExamplesDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void GetRatedExamples();
  void KeyPresses();
  void KeyPressesFromSlots();
  void UserHasNotClickedOkAtConstruction();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmaprateexamplesdialog_test_H
