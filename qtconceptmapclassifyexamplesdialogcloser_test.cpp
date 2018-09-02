#include "qtconceptmapclassifyexamplesdialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapclassifyexamplesdialog.h"
#include "qtconceptmapclassifyexamplesdialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtClassifyExamplesDialogCloserTest::ModifyAndCancelDiscardsChanges() const noexcept
{
  if (OnTravis()) return; //Does not work with 2 full seconds

  QtClassifyExamplesDialogCloser c;
  const Concept concept("A", Examples( { Example("1") } ));
  const auto examples_before = concept.GetExamples();
  QtClassifyExamplesDialog d(concept);
  assert(examples_before == d.GetRatedExamples());
  QTimer::singleShot(2000, &c, SLOT(ModifyAndCancel()));
  d.exec();
  assert(d.HasUserClickedOk());
  const auto examples_after = d.GetRatedExamples();
  QVERIFY(examples_before == examples_after);
}

void ribi::cmap::QtClassifyExamplesDialogCloserTest::ModifyAndOkKeepsChanges() const noexcept
{
  QtClassifyExamplesDialogCloser c;
  const Concept concept("A", Examples( { Example("1") } ));
  const auto examples_before = concept.GetExamples();
  QtClassifyExamplesDialog d(concept);
  assert(examples_before == d.GetRatedExamples());
  QTimer::singleShot(4000, &c, SLOT(ModifyAndOk()));
  d.exec();
  assert(d.HasUserClickedOk());
  const auto examples_after = d.GetRatedExamples();
  QVERIFY(examples_before != examples_after);
}

void ribi::cmap::QtClassifyExamplesDialogCloserTest::PressCancel() const noexcept
{
  QtClassifyExamplesDialogCloser c;
  const Concept concept("A", Examples( { Example("1") } ));
  QtClassifyExamplesDialog d(concept);
  QTimer::singleShot(4000, &c, SLOT(PressCancel()));
  d.exec();
  QVERIFY(!d.HasUserClickedOk());
}

void ribi::cmap::QtClassifyExamplesDialogCloserTest::PressOk() const noexcept
{
  QtClassifyExamplesDialogCloser c;
  const Concept concept("A", Examples( { Example("1") } ));
  QtClassifyExamplesDialog d(concept);
  QTimer::singleShot(4000, &c, SLOT(PressOk()));
  d.exec();
  QVERIFY(d.HasUserClickedOk());
}
