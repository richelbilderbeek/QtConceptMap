#include "qtconceptmapratedconceptdialog_test.h"

#include "qtconceptmapratedconceptdialog.h"

#include "conceptmapfactory.h"

void ribi::cmap::qtconceptmapratedconceptdialog_test::show_normal()
{
  const auto conceptmap = ConceptMapFactory().Get11();
  const auto nodes = GetNodes(conceptmap);
  for (const auto node: nodes)
  {
    QtConceptMapRatedConceptDialog d(conceptmap, node);
    d.show();
  }
}
