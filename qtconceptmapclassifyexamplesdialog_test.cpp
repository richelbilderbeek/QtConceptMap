#include "qtconceptmapclassifyexamplesdialog_test.h"
#include "qtconceptmapclassifyexamplesdialog.h"


#include <QDesktopWidget>
#include <QKeyEvent>
#include <QListWidgetItem>

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapexamplefactory.h"
#include "conceptmapexamplesfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmapcompetency.h"
#include "ui_qtconceptmapclassifyexamplesdialog.h"

void ribi::cmap::QtClassifyExamplesDialogTest::GetRatedExamples() const noexcept
{

  const int sz = ConceptFactory().GetTests().size();
  for (int i=0; i!=sz; ++i)
  {
    const Concept concept_1 = ConceptFactory().GetTests().at(i);
    if (CountExamples(concept_1) == 0) continue;
    const auto a = QtClassifyExamplesDialog(concept_1).GetRatedExamples();
    for (int j=0; j!=sz; ++j)
    {
      const Concept concept_2 = ConceptFactory().GetTests().at(j);
      if (CountExamples(concept_2) == 0) continue;
      const auto b = QtClassifyExamplesDialog(concept_2).GetRatedExamples();
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

void ribi::cmap::QtClassifyExamplesDialogTest::KeyPresses() const noexcept
{
  QtClassifyExamplesDialog d(ConceptFactory().Get1());
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

void ribi::cmap::QtClassifyExamplesDialogTest::KeyPressesFromSlots() const noexcept
{
  QtClassifyExamplesDialog d(ConceptFactory().Get1());
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

void ribi::cmap::QtClassifyExamplesDialogTest::ThrowsIfNoExamples() const noexcept
{
  const Concept empty_concept;
  assert(CountExamples(empty_concept) == 0);
  try
  {
    const QtClassifyExamplesDialog d{empty_concept};
    assert(!"Should not get here");
  }
  catch (const std::exception&)
  {
    QVERIFY("Should get here");
  }
}

void ribi::cmap::QtClassifyExamplesDialogTest::UserHasNotClickedOkAtConstruction() const noexcept
{
  const QtClassifyExamplesDialog d{
    Concept("A", Examples( { Example("1") } ))
  };
  QVERIFY(!d.HasUserClickedOk());
}
