#include "qtconceptmapqtedge_test.h"

#include <cmath>
#include <boost/lambda/lambda.hpp>

#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapfactory.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtquadbezierarrowitem.h"
#include "qtroundededitrectitem.h"

void ribi::cmap::qtconceptmapqtedge_test::change_focus()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  const auto qtedges = GetQtEdges(m.GetScene());
  assert(qtedges.size() >= 2);
  QtEdge * const qtedge1 = qtedges.front();
  QtEdge * const qtedge2 = qtedges.back();
  assert(qtedge1 != qtedge2);
  m.show();
  qtedge1->setFocus();
  m.show();
  qtedge2->setFocus();
  m.show();
}

void ribi::cmap::qtconceptmapqtedge_test::enable_and_disable()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  const auto qtedge = GetFirstQtEdge(m.GetScene());
  assert(qtedge);
  DisableAll(*qtedge);
  m.show();
  QVERIFY(!qtedge->isEnabled());
  QVERIFY(!qtedge->GetArrow()->isEnabled());
  EnableAll(*qtedge);
  m.show();
  QVERIFY(qtedge->isEnabled());
  QVERIFY(qtedge->GetArrow()->isEnabled());
}

void ribi::cmap::qtconceptmapqtedge_test::show_bounding_rect()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  const auto qtedge = GetLastQtEdge(m.GetScene());
  assert(qtedge);
  qtedge->SetShowBoundingRect(true);
  m.showFullScreen();
  QTest::qWait(1000);
}



void ribi::cmap::qtconceptmapqtedge_test::node_must_have_dotted_black_line_when_selected()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().GetStarShaped());
  m.show();
  //ReallyLoseFocus(m);
  QtEdge * const qtedge = GetLastQtEdge(m.GetScene());
  QtNode * const qtnode = qtedge->GetQtNode();
  qtnode->setSelected(true);
  qtnode->setFocus();
  QVERIFY(qtnode->GetFocusPen().color() == Qt::black);
  QVERIFY(qtnode->GetFocusPen().style() == Qt::DashLine);
}


void ribi::cmap::qtconceptmapqtedge_test::node_must_have_white_border_when_not_selected()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().GetStarShaped());
  m.show();
  const QtEdge * const qtedge = GetLastQtEdge(m.GetScene());
  const QtNode * const qtnode = qtedge->GetQtNode();
  QVERIFY(qtnode->GetContourPen().color() == Qt::white);
  QVERIFY(qtnode->GetContourPen().style() == Qt::SolidLine);
}

void ribi::cmap::qtconceptmapqtedge_test::to_str()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  const auto qtedge = GetFirstQtEdge(m.GetScene());
  assert(qtedge);
  QVERIFY(!ToStr(*qtedge).empty());
}

void ribi::cmap::qtconceptmapqtedge_test::to_stream()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  const auto qtedge = GetFirstQtEdge(m.GetScene());
  std::stringstream s;
  s << (*qtedge);
  QVERIFY(!s.str().empty());
}

void ribi::cmap::qtconceptmapqtedge_test::all_tests()
{
  using namespace ribi;
  using namespace ribi::cmap;
  const int node_test_index{0};
  const int edge_test_index{0};
  const Node node_from{NodeFactory().GetTest(node_test_index)};
  const Node node_to{NodeFactory().GetTest(node_test_index)};
  const boost::shared_ptr<QtNode> qtnode_from{new QtNode(node_from)};
  const boost::shared_ptr<QtNode> qtnode_to{new QtNode(node_to)};
  const Edge edge{EdgeFactory().GetTest(edge_test_index)};
  const boost::shared_ptr<QtEdge> qtedge{new QtEdge(edge,qtnode_from.get(),qtnode_to.get())};
  //const boost::shared_ptr<QtEdge> qtedge{new QtEdge(edge,qtnode_from,qtnode_to)};
  QtRoundedEditRectItem * qtitem{dynamic_cast<QtRoundedEditRectItem*>(qtedge->GetQtNode())};
  //if (verbose) { TRACE("QtEdge must accept hover events"); } //NOT SURE
  {
    //QVERIFY(qtedge->acceptHoverEvents()); //Must remove the 's' in Qt5?
  }
  //if (verbose) { TRACE("QtEdge its arrow must accept hover events"); }
  {
    QVERIFY(qtedge->GetArrow()->acceptHoverEvents()); //Must remove the 's' in Qt5?
  }
  //Text
  //if (verbose) { TRACE("Text of QtEdge must be one line"); }
  {
    QVERIFY(qtitem->GetText().size() == 1);
  }
  //if (verbose) { TRACE("Text of QtEdge and QtRoundedEditRectItem must match at creation"); }
  {
    const std::string qtitem_name{qtitem->GetText()[0]};
    const std::string qtedge_name{qtedge->GetEdge().GetNode().GetConcept().GetName()};
    QVERIFY(qtitem_name == qtedge_name);
  }
  //X
  //if (verbose) { TRACE("X of QtEdge and QtRoundedEditRectItem must match at creation"); }
  {
    const double edge_x{edge.GetNode().GetX()};
    const double qtedge_x{qtitem->GetCenterX()};
    QVERIFY(edge_x == qtedge_x);
  }
  //if (verbose) { TRACE("If X is set via QtEdge, QtRoundedEditRectItem must sync"); }
  {
    const double old_x{qtedge->GetQtNode()->GetCenterX()};
    const double new_x{old_x + 10.0};
    qtedge->GetQtNode()->SetCenterX(new_x);
    QVERIFY(std::abs(qtitem->GetCenterX() - new_x) < 2.0);
  }
  //if (verbose) { TRACE("If X is set via QtRoundedEditRectItem, QtEdge must sync"); }
  {
    const double old_x{qtitem->GetCenterX()};
    const double new_x{old_x + 10.0};
    qtitem->SetCenterX(new_x);
    QVERIFY(std::abs(qtedge->GetQtNode()->GetCenterX() - new_x) < 2.0);
  }
  //Y
//  //if (verbose) { TRACE("Y of QtEdge and QtRoundedEditRectItem must match at creation"); }
//  {
//    const double edge_y{edge.GetNode().GetY()};
//    const double qtedge_y{qtitem->GetCenterY()};
//    QVERIFY(edge_y == qtedge_y);
//  }
  //if (verbose) { TRACE("If Y is set via QtEdge, QtRoundedEditRectItem must sync"); }
  {
    const double old_y{qtedge->GetQtNode()->GetCenterY()};
    const double new_y{old_y + 10.0};
    qtedge->GetQtNode()->SetCenterY(new_y);
    QVERIFY(std::abs(qtitem->GetCenterY() - new_y) < 2.0);
  }
  //if (verbose) { TRACE("If Y is set via QtRoundedEditRectItem, QtEdge must sync"); }
  {
    const double old_y{qtitem->GetCenterY()};
    const double new_y{old_y + 10.0};
    qtitem->SetCenterY(new_y);
    QVERIFY(std::abs(qtedge->GetQtNode()->GetCenterY() - new_y) < 2.0);
  }
  //Center
  {
    const double new_x{qtedge->GetQtNode()->GetCenterX() + 123.45};
    qtedge->GetQtNode()->SetCenterX(new_x);
    QVERIFY(qtedge->GetQtNode()->GetCenterX() == new_x);
  }
  {
    const double new_y{qtedge->GetQtNode()->GetCenterY() + 123.45};
    qtedge->GetQtNode()->SetCenterY(new_y);
    QVERIFY(qtedge->GetQtNode()->GetCenterY() == new_y);
  }
}
