#include "qtconceptmapcommand_test.h"

#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommand.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "conceptmaphelper.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

#include <QDebug>

void ribi::cmap::QtCommandTest::AllConstGettersWork() const noexcept
{
  QtConceptMap q;
  const CommandCreateNewNode c(q);
  QVERIFY(&q.GetQtToolItem() == &GetQtToolItem(c));
  QVERIFY(&q.GetScene() == &GetScene(c));
}

void ribi::cmap::QtCommandTest::AllGettersWork() const noexcept
{
  QtConceptMap q;
  CommandCreateNewNode c(q);
  QVERIFY(&q.GetQtToolItem() == &GetQtToolItem(c));
  QVERIFY(&q.GetScene() == &GetScene(c));
}
