#include "qtconceptmapeditconceptdialogcloser_test.h"

#include "conceptmapfactory.h"
#include "conceptmaphelper.h"
#include "qtconceptmap.h"
#include "qtconceptmapeditconceptdialog.h"
#include "qtconceptmapeditconceptdialogcloser.h"

using namespace ribi::cmap;

void ribi::cmap::QtEditConceptDialogCloserTest::PressCancel() const noexcept
{
  if (OnTravis()) return;

  QtEditConceptDialogCloser c;
  QtEditConceptDialog d(Concept(), QtEditConceptDialog::EditType::concept);
  QTimer::singleShot(100, &c, SLOT(PressCancel()));
  d.exec();
  QVERIFY(!d.HasUserClickedOk());
}

void ribi::cmap::QtEditConceptDialogCloserTest::PressOk() const noexcept
{
  if (OnTravis()) return;

  QtEditConceptDialogCloser c;
  QtEditConceptDialog d(Concept(), QtEditConceptDialog::EditType::concept);
  QTimer::singleShot(100, &c, SLOT(PressOk()));
  d.exec();
  QVERIFY(d.HasUserClickedOk());
}
