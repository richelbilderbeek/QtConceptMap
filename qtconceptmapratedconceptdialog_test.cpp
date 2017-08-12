#include "qtconceptmapratedconceptdialog_test.h"

#include "qtconceptmapratedconceptdialog.h"

#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapRatedConceptDialogTest::do_resize_lists()
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node);
  d.show();
  QTest::qWait(1000);
  d.show();
}

void ribi::cmap::QtConceptMapRatedConceptDialogTest::hide_rating()
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto node = GetFirstNode(conceptmap);
  QtConceptMapRatedConceptDialog d(conceptmap, node);
  d.show();
  d.HideRating();
  d.show();
}

void ribi::cmap::QtConceptMapRatedConceptDialogTest::show_example()
{
  const auto conceptmap = ConceptMapFactory().GetQtRatedConceptDialogExample();
  const auto nodes = GetNodes(conceptmap);
  for (const auto node: nodes)
  {
    QtConceptMapRatedConceptDialog d(conceptmap, node);
    d.show();
  }
}
