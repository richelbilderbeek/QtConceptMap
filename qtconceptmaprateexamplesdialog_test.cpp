#include "qtconceptmaprateexamplesdialog_test.h"
#include "qtconceptmaprateexamplesdialog.h"


#include <QDesktopWidget>
#include <QKeyEvent>
#include <QListWidgetItem>

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmapcompetency.h"
#include "ui_qtconceptmaprateexamplesdialog.h"

void ribi::cmap::QtConceptMapRateExamplesDialogTest::GetRatedExamples()
{
  const int sz = ConceptFactory().GetTests().size();
  for (int i=0; i!=sz; ++i)
  {
    QVERIFY(i < static_cast<int>(ConceptFactory().GetTests().size()));
    const auto a = QtRateExamplesDialog(ConceptFactory().GetTests()[i]).GetRatedExamples();
    for (int j=0; j!=sz; ++j)
    {
      QVERIFY(j < static_cast<int>(ConceptFactory().GetTests().size()));
      const auto b = QtRateExamplesDialog(ConceptFactory().GetTests()[j]).GetRatedExamples();
      if (i == j)
      {
        QVERIFY(a == b);
      }
      else
      {
        QVERIFY(a != b);
      }
    }
  }
}

void ribi::cmap::QtConceptMapRateExamplesDialogTest::KeyPresses()
{
  QtRateExamplesDialog d(ConceptFactory().Get1());
  d.show();
  const int delay = 100;
  QTest::qWait(delay);
  //Press all buttons
  QTest::keyPress(&d, Qt::Key_B, Qt::AltModifier);
  d.show();
  QTest::qWait(delay);
  QTest::keyPress(&d, Qt::Key_R, Qt::AltModifier);
  d.show();
  QTest::qWait(delay);
  QTest::keyPress(&d, Qt::Key_S, Qt::AltModifier);
  d.show();
  QTest::qWait(delay);
  QTest::keyPress(&d, Qt::Key_D, Qt::AltModifier);
  d.show();
  QTest::qWait(delay);
  QTest::keyPress(&d, Qt::Key_T, Qt::AltModifier);
  d.show();
  QTest::qWait(delay);
  QTest::keyPress(&d, Qt::Key_P, Qt::AltModifier);
  d.show();
  QTest::qWait(delay);
  QTest::keyPress(&d, Qt::Key_V, Qt::AltModifier);
  d.show();
  QTest::qWait(delay);
  //Translate
  QTest::keyPress(&d, Qt::Key_T, Qt::ControlModifier | Qt::ShiftModifier);
  d.show();
  QTest::qWait(delay);
}

void ribi::cmap::QtConceptMapRateExamplesDialogTest::KeyPressesFromSlots()
{
  QtRateExamplesDialog d(ConceptFactory().Get1());
  d.show();
  const int delay = 100;
  QTest::qWait(delay);
  d.on_button_prof_clicked();
  d.show();
  QTest::qWait(delay);
  d.on_button_organisations_clicked();
  d.show();
  QTest::qWait(delay);
  d.on_button_social_clicked();
  d.show();
  QTest::qWait(delay);
  d.on_button_target_audience_clicked();
  d.show();
  QTest::qWait(delay);
  d.on_button_prof_development_clicked();
  d.show();
  QTest::qWait(delay);
  d.on_button_misc_clicked();
  d.show();
  QTest::qWait(delay);
  d.on_button_ti_knowledge_clicked();
  d.show();
  QTest::qWait(delay);
  d.on_button_ok_clicked();
  d.show();
  QTest::qWait(delay);
}

void ribi::cmap::QtConceptMapRateExamplesDialogTest::UserHasNotClickedOkAtConstruction()
{
  const QtRateExamplesDialog d{Concept()};
  QVERIFY(!d.HasClickedOk());
}
