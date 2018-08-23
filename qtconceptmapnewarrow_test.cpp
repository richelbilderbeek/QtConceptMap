#include "qtconceptmapnewarrow_test.h"

#include "qtconceptmapnewarrow.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "conceptmapfactory.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaptoolsitem.h"

void ribi::cmap::QtNewArrowTest::CancelActiveArrow() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodes());
  q.SetMode(Mode::edit);
  q.showFullScreen();

  const QtNode * const first_qtnode = GetFirstQtNode(q);
  const QtNode * const last_qtnode = GetLastQtNode(q);
  assert(first_qtnode);
  assert(last_qtnode);
  assert(first_qtnode != last_qtnode);

  //Click on first node to start tool icon
  const QPoint first_pos{q.mapFromScene(first_qtnode->pos())};
  QTest::mouseMove(q.viewport(), first_pos);
  QTest::mouseClick(
    q.viewport(),
    Qt::LeftButton,
    Qt::NoModifier,
    first_pos
  );

  QTest::qWait(100);

  //Click on tool icon
  const QPoint second_pos{
    q.mapFromScene(
      QPointF(
        q.GetQtToolItem().pos().x() + 12.0,
        q.GetQtToolItem().pos().y() + 12.0
      )
    )
  };
  QTest::mouseMove(q.viewport(), second_pos);
  QTest::mouseClick(
    q.viewport(),
    Qt::LeftButton,
    Qt::NoModifier,
    second_pos
  );

  QTest::qWait(100);

  //Click on second node to create edge
  const QPoint third_pos{q.mapFromScene(last_qtnode->pos())};
  QTest::mouseMove(q.viewport(), third_pos);

  assert(q.GetQtNewArrow().isVisible());
  QTest::keyPress(&q, Qt::Key_Escape);
  QVERIFY(!q.GetQtNewArrow().isVisible());
}

void ribi::cmap::QtNewArrowTest::CreateNewQtEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodes());
  q.SetMode(Mode::edit);
  q.showFullScreen();

  const QtNode * const first_qtnode = GetFirstQtNode(q);
  const QtNode * const last_qtnode = GetLastQtNode(q);
  assert(first_qtnode);
  assert(last_qtnode);
  assert(first_qtnode != last_qtnode);

  //Click on first node to start tool icon
  const QPoint first_pos{q.mapFromScene(first_qtnode->pos())};
  QTest::mouseMove(q.viewport(), first_pos);
  QTest::mouseClick(
    q.viewport(),
    Qt::LeftButton,
    Qt::NoModifier,
    first_pos
  );

  QTest::qWait(100);

  //Click on tool icon
  const QPoint second_pos{
    q.mapFromScene(
      QPointF(
        q.GetQtToolItem().pos().x() + 12.0,
        q.GetQtToolItem().pos().y() + 12.0
      )
    )
  };
  QTest::mouseMove(q.viewport(), second_pos);
  QTest::mouseClick(
    q.viewport(),
    Qt::LeftButton,
    Qt::NoModifier,
    second_pos
  );

  QTest::qWait(100);
  assert(CountQtEdges(q) == 0);

  //Click on second node to create edge
  const QPoint third_pos{q.mapFromScene(last_qtnode->pos())};
  QTest::mouseMove(q.viewport(), third_pos);
  QTest::mouseClick(
    q.viewport(),
    Qt::LeftButton,
    Qt::NoModifier,
    third_pos,
    100 //Really need this delay
  );

  QTest::qWait(100);
  QVERIFY(CountQtEdges(q) == 1);
}

void ribi::cmap::QtNewArrowTest::StartSetsSourceQtNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodes());
  q.showFullScreen();
  const auto qtnode = GetFirstQtNode(q);
  q.GetQtNewArrow().Start(qtnode);
  QVERIFY(q.GetQtNewArrow().GetFrom() == qtnode);
}
