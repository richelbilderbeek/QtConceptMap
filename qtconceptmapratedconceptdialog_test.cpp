#include "qtconceptmapratedconceptdialog_test.h"

#include "qtconceptmapratedconceptdialog.h"

#include "conceptmapfactory.h"

void ribi::cmap::QtRatedConceptDialogTest::show_assessor()
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node, Role::assessor);
  d.show();
  QTest::qWait(1000);
  d.show();
}

void ribi::cmap::QtRatedConceptDialogTest::show_student()
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node, Role::student);
  d.show();
}

void ribi::cmap::QtRatedConceptDialogTest::show_example()
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto nodes = GetNodes(conceptmap);
  for (const auto node: nodes)
  {
    QtConceptMapRatedConceptDialog d(conceptmap, node, Role::assessor);
    d.show();
  }
}
