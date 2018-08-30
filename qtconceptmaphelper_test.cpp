#include "qtconceptmaphelper_test.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnode.h"
#include "conceptmapfactory.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapqtedge.h"

#include "qtconceptmap.h"

void ribi::cmap::HelperTest::ExtractTheOneSelectedQtEdge() const noexcept
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
    assert(CountSelectedQtEdges(q) == 0);
    ::ribi::cmap::ExtractTheOneSelectedQtEdge(q);
    assert(!"Should not get here");
  }
  catch(const std::invalid_argument&)
  {
    QVERIFY("Should get here");
  }
}

void ribi::cmap::HelperTest::FindQtCenterNode() const noexcept
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

void ribi::cmap::HelperTest::GetFirstQtEdge() const noexcept
{
  //No concept map, thus no first edge
  {
    QtConceptMap q;
    const auto qtnode = ::ribi::cmap::GetFirstQtEdge(q);
    QVERIFY(!qtnode);
  }
  //Concept map of one edge
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
    QVERIFY(::ribi::cmap::GetFirstQtEdge(m));
  }
}

void ribi::cmap::HelperTest::GetQtCenterNode() const noexcept
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

void ribi::cmap::HelperTest::GetQtEdges() const noexcept
{
  {
    QtConceptMap m;
    m.SetConceptMap(ConceptMapFactory().GetLonelyCenterNode());
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

void ribi::cmap::HelperTest::GetQtNodeBrushFunction() const noexcept
{
  //Edit + no examples = Gray
  {
    const auto f = ::ribi::cmap::GetQtNodeBrushFunction(Mode::edit);
    const QtNode qtnode;
    QVERIFY(f(qtnode) == QtBrushFactory().CreateGrayGradientBrush());
  }
  //Rate + unrated rated = Red
  {
    const auto f = ::ribi::cmap::GetQtNodeBrushFunction(Mode::rate);
    const QtNode qtnode;
    QVERIFY(f(qtnode) == QtBrushFactory().CreateRedGradientBrush());
  }
  //Uninitialized = White
  {
    const auto f = ::ribi::cmap::GetQtNodeBrushFunction(Mode::uninitialized);
    const QtNode qtnode;
    QVERIFY(f(qtnode) == QtBrushFactory().CreateWhiteGradientBrush());
  }
}

void ribi::cmap::HelperTest::GetQtNodeBrushFunctionEdit() const noexcept
{
  const auto f = ::ribi::cmap::GetQtNodeBrushFunctionEdit();
  //Gold if center node
  {
    const QtNode qtnode(Node(Concept(), NodeType::center));
    QVERIFY(f(qtnode) == QtBrushFactory().CreateGoldGradientBrush());
  }
  //Grey if no examples
  {
    const QtNode qtnode;
    QVERIFY(f(qtnode) == QtBrushFactory().CreateGrayGradientBrush());
  }
  //Red if solitary node (that is, a concept) with examples
  {
    const QtNode qtnode(Node(Concept("", Examples( { Example("A") } ))));
    QVERIFY(f(qtnode) == QtBrushFactory().CreateRedGradientBrush());
  }
  //Blue if relation node with examples
  {
    QtNode from;
    QtNode to;
    const QtEdge qtedge(Edge(Node(Concept("", Examples( { Example("A") } )))), &from, &to);
    QVERIFY(f(*qtedge.GetQtNode()) == QtBrushFactory().CreateBlueGradientBrush());
  }
}

void ribi::cmap::HelperTest::GetQtNodeBrushFunctionRate() const noexcept
{
  const auto f = ::ribi::cmap::GetQtNodeBrushFunctionRate();
  //Gold if center node
  {
    const QtNode qtnode(Node(Concept(), NodeType::center));
    QVERIFY(f(qtnode) == QtBrushFactory().CreateGoldGradientBrush());
  }
  //Blue if relation node with examples
  {
    QtNode from;
    QtNode to;
    const QtEdge qtedge(Edge(Node(Concept("", Examples( { Example("A") } )))), &from, &to);
    QVERIFY(f(*qtedge.GetQtNode()) == QtBrushFactory().CreateBlueGradientBrush());
  }

  //Red if none rated
  {
    const QtNode qtnode;
    QVERIFY(f(qtnode) == QtBrushFactory().CreateRedGradientBrush());
  }
  //Yellow if one rated
  {
    const QtNode qtnode(Node(Concept("", Examples(), true, 2, -1, -1)));
    QVERIFY(f(qtnode) == QtBrushFactory().CreateYellowGradientBrush());
  }
  //Yellow if two rated
  {
    const QtNode qtnode(Node(Concept("", Examples(), true, 2, 2, -1)));
    QVERIFY(f(qtnode) == QtBrushFactory().CreateYellowGradientBrush());
  }
  //Green if fully rated
  {
    const QtNode qtnode(Node(Concept("", Examples(), true, 2, 2, 2)));
    QVERIFY(f(qtnode) == QtBrushFactory().CreateGreenGradientBrush());
  }

}


void ribi::cmap::HelperTest::GetQtNodeBrushFunctionUninitialized() const noexcept
{
  const auto f = ::ribi::cmap::GetQtNodeBrushFunctionUninitialized();
  //Always white
  const QtNode qtnode;
  QVERIFY(f(qtnode) == QtBrushFactory().CreateWhiteGradientBrush());
}

void ribi::cmap::HelperTest::MessUp() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  MoveQtEdgesAndQtNodesRandomly(m);
}


