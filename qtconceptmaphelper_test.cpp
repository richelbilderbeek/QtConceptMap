#include "qtconceptmaphelper_test.h"
#include "qtconceptmaphelper.h"
#include "conceptmapfactory.h"

#include "qtconceptmap.h"

void ribi::cmap::QtConceptMapHelperTest::ExtractTheOneSelectedQtEdge() const noexcept
{
  //Should throw on empty Scene
  try
  {
    ::ribi::cmap::ExtractTheOneSelectedQtEdge(QGraphicsScene());
    assert(!"Should not get here");
  }
  catch(const std::invalid_argument&)
  {
    QVERIFY("Should get here");
  }

  //Should throw when there is no selected QtEdge
  try
  {
    QtConceptMap q;
    q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
    ::ribi::cmap::ExtractTheOneSelectedQtEdge(q.GetScene());
    assert(!"Should not get here");
  }
  catch(const std::invalid_argument&)
  {
    QVERIFY("Should get here");
  }
}


void ribi::cmap::QtConceptMapHelperTest::FindQtCenterNode() const noexcept
{
  //No concept map, thus no center node
  {
    QtConceptMap m;
    QVERIFY(!::ribi::cmap::FindQtCenterNode(m.GetScene()));
  }
  //Concept map of one center node
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
    QVERIFY(::ribi::cmap::FindQtCenterNode(m.GetScene()));
  }
  //Concept map without center node
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
    QVERIFY(!::ribi::cmap::FindQtCenterNode(m.GetScene()));
  }
}

void ribi::cmap::QtConceptMapHelperTest::GetQtCenterNode() const noexcept
{
  //No concept map, thus no center node
  {
    QtConceptMap m;
    const auto qtnode = ::ribi::cmap::GetQtCenterNode(m);
    QVERIFY(!qtnode);
  }
  //Concept map of one center node
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
    QVERIFY(::ribi::cmap::GetQtCenterNode(m));
  }
}

void ribi::cmap::QtConceptMapHelperTest::GetQtEdges() const noexcept
{
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetLonelyQtCenterNode());
    QVERIFY(::ribi::cmap::GetQtEdges(m.GetScene()).size() == 0);
  }
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
    QVERIFY(::ribi::cmap::GetQtEdges(m.GetScene()).size() == 1);
  }
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
    QVERIFY(::ribi::cmap::GetQtEdges(m.GetScene()).size() == 2);
  }
}

void ribi::cmap::QtConceptMapHelperTest::GetQtNodeBrushFunction() const noexcept
{
  ::ribi::cmap::GetQtNodeBrushFunction(Mode::edit);
  ::ribi::cmap::GetQtNodeBrushFunction(Mode::rate);
  ::ribi::cmap::GetQtNodeBrushFunction(Mode::uninitialized);
}

void ribi::cmap::QtConceptMapHelperTest::MessUp() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  MoveQtEdgesAndQtNodesRandomly(m);
}


