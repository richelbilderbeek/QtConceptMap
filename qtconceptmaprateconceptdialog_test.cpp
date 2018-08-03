#include "qtconceptmaprateconceptdialog_test.h"

#include "qtconceptmap.h"
#include "qtconceptmaprateconceptdialog.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtConceptMapRateConceptDialogTest::show_normal_concept_map()
{
  auto qtconceptmap{
    std::make_unique<QtConceptMap>()
  };
  qtconceptmap->SetConceptMap(ConceptMapFactory().GetUnrated());
  const auto * const qtnode = GetFirstQtNode(*qtconceptmap);
  assert(qtnode);
  auto d{
    std::make_unique<QtRateConceptDialog>(
      *qtconceptmap, *qtnode
    )
  };
  d->show();
}

void ribi::cmap::QtConceptMapRateConceptDialogTest::show_concept_map_with_only_center_node()
{
  auto qtconceptmap{
    std::make_unique<QtConceptMap>()
  };
  qtconceptmap->SetConceptMap(ConceptMapFactory().Get1());
  const auto * const qtnode = GetFirstQtNode(*qtconceptmap);
  assert(qtnode);
  auto d{
    std::make_unique<QtRateConceptDialog>(
      *qtconceptmap, *qtnode
    )
  };
  d->show();
}
