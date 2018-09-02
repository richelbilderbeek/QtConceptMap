#include "qtconceptmapclassifyexamplesdialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapclassifyexamplesdialog.h"
#include "qtconceptmapclassifyexamplesdialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtClassifyExamplesDialogCloserTest::Modify() const noexcept
{
  QtClassifyExamplesDialogCloser c;
  const Concept concept("A", Examples( { Example("1") } ));
  const auto examples_before = concept.GetExamples();
  QtClassifyExamplesDialog d(concept);
  assert(examples_before == d.GetRatedExamples());
  QTimer::singleShot(500, &c, SLOT(ModifyAndOk()));
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
  QTimer::singleShot(500, &c, SLOT(PressCancel()));
  d.exec();
  QVERIFY(!d.HasUserClickedOk());
}

void ribi::cmap::QtClassifyExamplesDialogCloserTest::PressOk() const noexcept
{
  QtClassifyExamplesDialogCloser c;
  const Concept concept("A", Examples( { Example("1") } ));
  QtClassifyExamplesDialog d(concept);
  QTimer::singleShot(500, &c, SLOT(PressOk()));
  d.exec();
  QVERIFY(d.HasUserClickedOk());
}
