
#include "qtconceptmap_test.h"
#include "qtconceptmap.h"
#include "qtconceptmap.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QKeyEvent>
#include <QPoint>

#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapcollect.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommanddeleteselected.h"
#include "qtconceptmapcommandmovenode.h"
#include "qtconceptmapcommandselect.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmapcommandsetmode.h"
#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapcommandunselectnode.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapexamplesitem.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapqtedge.h"

void ribi::cmap::QtConceptMapTest::CannotDeleteCenterNode() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.DoCommand(new CommandCreateNewNode(q, "center", true));
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Delete, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::CannotEditCenterNode() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.DoCommand(new CommandSetMode(q, Mode::edit));
  q.DoCommand(new CommandCreateNewNode(q, "center", true));
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_F2, Qt::NoModifier);
  q.SetPopupMode(PopupMode::muted);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::CannotMoveCenterNode() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.DoCommand(new CommandSetMode(q, Mode::edit));
  q.DoCommand(new CommandCreateNewNode(q, "center", true));
  const auto selected_qt_nodes = GetSelectedQtNodesAlsoOnQtEdge(q);
  assert(selected_qt_nodes.size() == 1);
  const auto qtnode = selected_qt_nodes[0];
  const auto pos_before = qtnode->GetCenterPos();
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Down, Qt::ControlModifier);
  q.keyPressEvent(&e);
  q.show();
  const auto pos_after = qtnode->GetCenterPos();
  assert(!e.isAccepted());
  QVERIFY(!e.isAccepted());
  QVERIFY(pos_before == pos_after);
}

void ribi::cmap::QtConceptMapTest::ChangeModes() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.SetConceptMap(ConceptMapFactory().Get11());
  q.SetMode(Mode::edit);
  q.show();
  q.SetMode(Mode::rate);
  q.show();
  q.SetMode(Mode::uninitialized);
  q.show();
}

void ribi::cmap::QtConceptMapTest::ClickOnNothingShouldBeIgnored() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(1.0,2.0),Qt::LeftButton,Qt::NoButton,Qt::NoModifier);
  q.SetConceptMap(ConceptMapFactory().Get2());
  q.SetMode(Mode::edit);
  q.show();
  const auto qtnode = GetFirstQtNode(q.GetScene());
  const QPoint nothing{
    qtnode->mapToScene(qtnode->boundingRect().bottomRight()).toPoint()
  };
  QMouseEvent e(QEvent::Type::MouseButtonPress, nothing, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  q.mousePressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::ConceptMapMustFitWindow() const noexcept
{
  QtConceptMap q;

  CheckInvariants(q);

  q.showFullScreen();

  CheckInvariants(q);

  q.SetConceptMap(ConceptMapFactory().Get11());
  //m.SetConceptMap(ConceptMapFactory().GetStarShaped());

  CheckInvariants(q);

  q.show();

  CheckInvariants(q);

  QTest::qWait(100);
  qApp->processEvents();
  QVERIFY(!q.verticalScrollBar()->isVisible());
  QVERIFY(!q.horizontalScrollBar()->isVisible());
}

void ribi::cmap::QtConceptMapTest::ConceptMapMustFitWindowAfterSetting() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.SetConceptMap(ConceptMapFactory().Get11());
  QTest::qWait(100);
  QVERIFY(!q.verticalScrollBar()->isVisible());
  QVERIFY(!q.horizontalScrollBar()->isVisible());
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeCommand() const noexcept
{
  //When there are two selected nodes, an edge can be created
  //After adding the edges, only the edge will be selected
  //The edge its center concept will be between the two nodes
  QtConceptMap q;
  q.showFullScreen();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,0));
  const int n{2};
  for (int i=0; i!=n; ++i) {
    q.DoCommand(new CommandCreateNewNode(q));
  }
  q.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,2));
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q));
  q.show();
  QVERIFY(DoubleCheckEdgesAndNodes(q,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,1,0));
  q.show();
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeCommandAndCheckZorder() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();

  //Create two nodes
  for (int i=0; i!=2; ++i) {
    q.DoCommand(new CommandCreateNewNode(q));
    q.show();
  }
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,2));

  //Create edge between nodes
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q));
  q.show();
  QVERIFY(DoubleCheckEdgesAndNodes(q,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,1,0));

  const auto qtnodes = GetQtNodes(q.GetScene());
  const auto qtedges = GetQtEdges(q.GetScene());
  QVERIFY(qtnodes.size() == 2);
  QVERIFY(qtedges.size() == 1);
  const auto qtnode1 = qtnodes[0];
  const auto qtnode2 = qtnodes[1];
  const auto qtedge  = qtedges[0];
  QVERIFY(qtnode1->zValue() > qtedge->zValue());
  QVERIFY(qtnode2->zValue() > qtedge->zValue());
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeKeyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&q, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(q,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,1,0));
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeKeyboardAndUndo() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&q, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(q,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,1,0));
  //Undo
  QTest::keyClick(&q, Qt::Key_Z, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,2));
  assert(DoubleCheckSelectedEdgesAndNodes(q,0,2,true));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,2,true));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,2,true));
  //Redo
  QTest::keyClick(&q, Qt::Key_Z, Qt::ControlModifier | Qt::ShiftModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(q,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,1,0));
}

void ribi::cmap::QtConceptMapTest::create_one_node_and_undo_command() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,0));
  q.DoCommand(new CommandCreateNewNode(q));
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,1));
  q.Undo();
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,0));
}

void ribi::cmap::QtConceptMapTest::create_one_node_and_undo_keyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,0));
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,1));
  QTest::keyClick(&q, Qt::Key_Z, Qt::ControlModifier, 100);

  QVERIFY(DoubleCheckEdgesAndNodes(q,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,0));
}

void ribi::cmap::QtConceptMapTest::create_one_node_command() const noexcept
{
  const std::string text = __func__;
  const bool is_center_node{false};
  const double x{314.15};
  const double y{42.69};

  QtConceptMap q;
  q.showFullScreen();
  q.DoCommand(new CommandCreateNewNode(q, text, is_center_node, x, y));
  q.show();
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,1));
  const auto v = GetSelectedQtNodes(q.GetScene());
  QVERIFY(v.size() == 1);
  const auto n = v[0];
  QVERIFY(n);
  QVERIFY(std::abs(n->GetCenterX() - x) < 2.0);
  QVERIFY(std::abs(n->GetCenterY() - y) < 2.0);
}




void ribi::cmap::QtConceptMapTest::create_one_node_keyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  q.show();
  const auto expected_vertices{1};
  const auto measured_vertices{boost::num_vertices(q.GetConceptMap())};
  const auto expected_edges{0};
  const auto measured_edges{boost::num_edges(q.GetConceptMap())};
  QVERIFY(measured_edges == expected_edges);
  QVERIFY(measured_vertices == expected_vertices);
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,1));
}

void ribi::cmap::QtConceptMapTest::create_one_node_mouse() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::mouseDClick(q.viewport(), Qt::MouseButton::LeftButton, 0, QPoint(0.0,0.0), 100);
  q.show();
  const int n_nodes_in_scene{static_cast<int>(Collect<QtNode>(q.GetScene()).size())};
  const int n_nodes_in_conceptmap{static_cast<int>(boost::num_vertices(q.GetConceptMap()))};
  QVERIFY(n_nodes_in_scene == n_nodes_in_conceptmap);
}

void ribi::cmap::QtConceptMapTest::create_ten_nodes_and_undo_command() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,0));

  const int n{10};
  for (int i=0; i!=n; ++i)
  {
    q.DoCommand(new CommandCreateNewNode(q));
    q.show();
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,i + 1));
  }
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,n));
  for (int i=0; i!=n; ++i) { q.Undo(); }
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,0));
}

void ribi::cmap::QtConceptMapTest::create_ten_nodes_and_undo_keyboard() const noexcept
{

}

void ribi::cmap::QtConceptMapTest::create_two_nodes_command() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  for (int i=0; i!=2; ++i)
  {
    q.DoCommand(new CommandCreateNewNode(q));
  }
  q.show();
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,2));
}



void ribi::cmap::QtConceptMapTest::create_two_nodes_keyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  q.show();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  q.show();
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,2));
}

void ribi::cmap::QtConceptMapTest::default_construction() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();

  //No nodes, no edges
  QVERIFY(DoubleCheckEdgesAndNodes(q,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(q,0,0));

  //No examples selected, QGraphicsItem is created though (is that a good idea?)
  QVERIFY(!q.GetQtExamplesItem().GetBuddyItem());

  //No nodes with a Tool icon above it
  QVERIFY(!q.GetQtToolItem().isVisible());

  //No undo info yet
  QVERIFY(q.GetUndo().count() == 0);
}

void ribi::cmap::QtConceptMapTest::delete_node_that_is_connected_to_multiple_edges_keyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.SetConceptMap(ConceptMapFactory().GetStarShaped());
  q.show();
  //Select the node at the center of the star
  while (CountSelectedQtEdges(q) != 0
    || CountSelectedQtNodes(q) != 1
    || (CountSelectedQtNodes(q) > 0 && GetSelectedQtNodes(q)[0]->GetNode().GetConcept().GetName()
      != std::string("X"))
  )
  {
    CheckInvariants(q);
    q.show();
    QTest::keyClick(&q, Qt::Key_Space, Qt::NoModifier, 100);

    CheckInvariants(q);
  }
  q.show();
  QTest::keyClick(&q, Qt::Key_Delete, Qt::NoModifier, 100);
  q.show();
  q.Undo();
  q.show();
  q.Redo();
  q.show();
}

void ribi::cmap::QtConceptMapTest::DeleteNodeThatIsHeadOfEdgeKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  //Create nodes and edge
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  assert(qtedges.size() == 1);
  const auto qtedge = qtedges[0];
  assert(qtedge);
  const auto head = qtedge->GetTo();
  std::srand(42);
  //Select only one single QtNode (not on edge)
  while (CountSelectedQtEdges(m.GetScene()) != 0
    || CountSelectedQtNodes(m.GetScene()) != 1
    || GetSelectedQtNodes(m.GetScene())[0] != head
  ) {
    m.show();
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
  }
  QVERIFY(CountSelectedQtEdges(m.GetScene()) == 0);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::QtConceptMapTest::DeleteNodeThatIsHeadOfEdgeAndUndoKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  //Create nodes and edge
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  assert(qtedges.size() == 1);
  const auto qtedge = qtedges[0];
  assert(qtedge);
  const auto head = qtedge->GetTo();
  std::srand(42);
  //Select only one single QtNode (not on edge)
  while (CountSelectedQtEdges(m.GetScene()) != 0
    || CountSelectedQtNodes(m.GetScene()) != 1
    || GetSelectedQtNodes(m.GetScene())[0] != head
  ) {
    m.show();
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
  }
  QVERIFY(CountSelectedQtEdges(m.GetScene()) == 0);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
  m.Undo(); //New
}

void ribi::cmap::QtConceptMapTest::DeleteNodeThatIsTailOfEdgeKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  //Create nodes and edge
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  assert(qtedges.size() == 1);
  const auto qtedge = qtedges[0];
  assert(qtedge);
  const auto tail = qtedge->GetFrom();
  std::srand(42);
  //Select only one single QtNode (not on edge)
  while (CountSelectedQtEdges(m.GetScene()) != 0
    || CountSelectedQtNodes(m.GetScene()) != 1
    || GetSelectedQtNodes(m.GetScene())[0] != tail
  ) {
    m.show();
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
  }
  QVERIFY(CountSelectedQtEdges(m.GetScene()) == 0);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::QtConceptMapTest::DeleteNodesThatAreHeadAndTailOfEdgeKeyboard() const noexcept
{
  const double delay = 10;
  QtConceptMap m;
  m.showFullScreen();
  //Create nodes and edge
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, delay);
  for (int i=0; i!=10; ++i)
  {
    QTest::keyClick(&m, Qt::Key_Left, Qt::ControlModifier, delay);
  }
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, delay);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, delay);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  assert(qtedges.size() == 1);
  const auto qtedge = qtedges[0];
  assert(qtedge);
  std::srand(42);
  //Select only one single QtNode (not on edge)
  while (CountSelectedQtEdges(m.GetScene()) != 0
    || CountSelectedQtNodes(m.GetScene()) != 2
  ) {
    m.show();
    //Select first
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, delay);
    //Add-select second
    QTest::keyClick(&m, Qt::Key_Space, Qt::ShiftModifier, delay);
  }
  QVERIFY(CountSelectedQtEdges(m.GetScene()) == 0);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, delay);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::QtConceptMapTest::delete_one_edge_by_node_command() const noexcept
{

}

void ribi::cmap::QtConceptMapTest::delete_one_edge_by_node_keyboard() const noexcept
{

}

void ribi::cmap::QtConceptMapTest::delete_one_edge_command() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
  const int n{2};
  for (int i=0; i!=n; ++i)
  {
    m.DoCommand(new CommandCreateNewNode(m));
    m.show();
  }
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  m.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(m));
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  m.DoCommand(new CommandDeleteSelected(m));
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));

}

void ribi::cmap::QtConceptMapTest::delete_one_edge_keyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::QtConceptMapTest::delete_one_node_command() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  m.DoCommand(new CommandCreateNewNode(m));
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  m.DoCommand(new CommandDeleteSelected(m));
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
}

void ribi::cmap::QtConceptMapTest::delete_one_node_command_and_undo() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  m.DoCommand(new CommandCreateNewNode(m));
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  m.DoCommand(new CommandDeleteSelected(m));
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  m.Undo();
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
}

void ribi::cmap::QtConceptMapTest::delete_one_node_keyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
}


void ribi::cmap::QtConceptMapTest::delete_two_nodes_command() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  for (int i{0}; i!=2; ++i)
  {
    m.DoCommand(new CommandCreateNewNode(m));
    m.show();
  }
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));

  for (int i{0}; i!=2; ++i) {
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
    m.show();
    const int n_selected_measured{m.scene()->selectedItems().count()};
    const int n_selected_expected{1};
    assert(n_selected_measured == n_selected_expected);
    m.DoCommand(new CommandDeleteSelected(m));
    m.show();
  }
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::QtConceptMapTest::DeleteTwoNodesKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::QtConceptMapTest::DoubleClick() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  assert(boost::num_vertices(m.GetConceptMap()) == 0);
  QTest::mouseDClick(m.viewport(), Qt::LeftButton);
  QVERIFY(boost::num_vertices(m.GetConceptMap()) == 1);
}

void ribi::cmap::QtConceptMapTest::DoubleClickTwice() const noexcept
{
  QtConceptMap m;
  m.show();
  assert(boost::num_vertices(m.GetConceptMap()) == 0);
  //Creates a new node
  QTest::mouseDClick(m.viewport(), Qt::LeftButton);
  m.show();
  assert(boost::num_vertices(m.GetConceptMap()) == 1);
  //Does not create a new node, as the double-click took place on an existing node
  QTest::mouseDClick(m.viewport(), Qt::LeftButton);
  m.show();
  QVERIFY(boost::num_vertices(m.GetConceptMap()) == 1);
}

void ribi::cmap::QtConceptMapTest::EditModeFlags() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.show();
  m.SetMode(Mode::edit);
  for (const auto qtnode: GetQtNodes(m.GetScene()))
  {
    if (IsQtCenterNode(qtnode))
    {
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsMovable));
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsSelectable));
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsFocusable));
    }
    else
    {
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsMovable));
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsSelectable));
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsFocusable));
    }
  }
}

void ribi::cmap::QtConceptMapTest::RateConceptMapHasLessFocusableItems() const noexcept
{
  //In rate mode, the center node cannot be focused on
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.SetMode(Mode::edit);
  assert(CountCenterNodes(m.GetConceptMap()) > 0);
  assert(CountQtCenterNodes(m.GetScene()) > 0);
  assert(CountCenterNodes(m.GetConceptMap()) == CountQtCenterNodes(m.GetScene()));
  const auto n_edit = GetFocusableItems(m).size();
  m.SetMode(Mode::rate);
  const auto n_rate = GetFocusableItems(m).size();
  QVERIFY(n_rate < n_edit);
}

void ribi::cmap::QtConceptMapTest::RateModeFlags() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.show();
  m.SetMode(Mode::rate);
  for (const auto qtnode: GetQtNodes(m.GetScene()))
  {
    if (IsQtCenterNode(qtnode))
    {
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsMovable));
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsSelectable));
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsFocusable));
    }
    else
    {
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsMovable));
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsSelectable));
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsFocusable));
    }
  }
}


void ribi::cmap::QtConceptMapTest::GetHighlighter() const noexcept
{
  QtConceptMap m;
  const auto& h = m.GetQtHighlighter();
  //Nothing to highlight, thus not item
  QVERIFY(!h.GetItem());
}

void ribi::cmap::QtConceptMapTest::HideExamples() const noexcept
{
  QtConceptMap m;
  HideExamplesItem(m);
}

void ribi::cmap::QtConceptMapTest::IsCommandPutOnUndoStack() const noexcept
{
  QtConceptMap m;
  m.show();
  CommandCreateNewNode * const command = new CommandCreateNewNode(m);
  QVERIFY(m.GetUndo().count() == 0);
  m.DoCommand(command);
  m.show();
  QVERIFY(m.GetUndo().count() == 1);
}

void ribi::cmap::QtConceptMapTest::MouseWheel() const noexcept
{
  QtConceptMap m;
  m.show();
  QWheelEvent e(QPoint(10,10), 10,Qt::NoButton,Qt::NoModifier);
  m.wheelEvent(&e);
}

void ribi::cmap::QtConceptMapTest::NcenterNodesAndQtCenterNodesMustMatchEdit() const noexcept
{
  for (const auto conceptmap: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    m.SetConceptMap(conceptmap);
    m.SetMode(Mode::edit);
    const int n_center_nodes{CountCenterNodes(GetNodes(conceptmap))};
    const int n_qt_center_nodes{CountQtCenterNodes(m.GetScene())};
    QVERIFY(n_center_nodes == n_qt_center_nodes);
  }
}

void ribi::cmap::QtConceptMapTest::NcenterNodesAndQtCenterNodesMustMatchRate() const noexcept
{
  for (const auto conceptmap: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    m.SetConceptMap(conceptmap);
    m.SetMode(Mode::edit);
    const int n_center_nodes{CountCenterNodes(GetNodes(conceptmap))};
    const int n_qt_center_nodes{CountQtCenterNodes(m.GetScene())};
    QVERIFY(n_center_nodes == n_qt_center_nodes);
  }
}

void ribi::cmap::QtConceptMapTest::NcenterNodesAndQtCenterNodesMustMatchUninitialized() const noexcept
{
  for (const auto conceptmap: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    m.SetConceptMap(conceptmap);
    const int n_center_nodes{CountCenterNodes(GetNodes(conceptmap))};
    const int n_qt_center_nodes{CountQtCenterNodes(m.GetScene())};
    QVERIFY(n_center_nodes == n_qt_center_nodes);
  }
}

void ribi::cmap::QtConceptMapTest
  ::PressCtrlRightMovesNonCentralNode() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "from", false));

  const auto x_before = GetX(*GetQtNodes(q)[0]);
  const auto y_before = GetY(*GetQtNodes(q)[0]);

  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Right, Qt::ControlModifier);
  q.keyPressEvent(&e);

  QVERIFY(e.isAccepted());
  const auto x_after = GetX(*GetQtNodes(q)[0]);
  const auto y_after = GetY(*GetQtNodes(q)[0]);
  QVERIFY(x_after > x_before);
  QVERIFY(y_after == y_before);
}

void ribi::cmap::QtConceptMapTest::PressEscape() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_Escape);
}

void ribi::cmap::QtConceptMapTest
  ::PressF1OnEmptyConceptMapIsRejected() const noexcept
{
  QtConceptMap m;
  m.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  m.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF2OnEmptyConceptMapIsRejected() const noexcept
{
  QtConceptMap m;
  m.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  m.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF2CannotEditFocalQuestion() const noexcept
{
  QtConceptMap m;
  m.SetMode(Mode::edit);
  m.SetConceptMap(ConceptMapFactory().Get1());
  m.SetPopupMode(PopupMode::normal); //
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  m.keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF2CanEditNonFocalQuestion() const noexcept
{
  //Cannot do this test: the popup freezes the test
  //Can edit a non-center node in edit mode
  QtConceptMap m;
  m.SetMode(Mode::edit);
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.SetPopupMode(PopupMode::muted); //
  //Press space until other non-center QtNode is selected
  while (1)
  {
    QTest::keyClick(&m, Qt::Key_Space);
    const auto qtnodes = GetSelectedQtNodes(m.GetScene());
    QVERIFY(qtnodes.size() == 1);
    if (!IsQtCenterNode(qtnodes[0])) break;

  }
  //F2 should activate 'Edit Concept' popup
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  m.keyPressEvent(event);
  QVERIFY(event->isAccepted());
}



void ribi::cmap::QtConceptMapTest::PressF4IsRejected() const noexcept
{
  //F4 has no purpose
  QtConceptMap m;
  m.show();
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
  m.keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressH() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier);
}

void ribi::cmap::QtConceptMapTest::PressN() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier);
}

void ribi::cmap::QtConceptMapTest::PressQuestionMark() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_Question);
}

void ribi::cmap::QtConceptMapTest
  ::PressShiftRightSelectsEdgeAdditively() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "left", false));
  q.DoCommand(new CommandCreateNewNode(q, "right", false));
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q, "between"));
  UnselectAll(q);
  QtNode * const left_qtnode
    = FindFirstQtNode(q, [](QtNode * const qtnode) { return GetText(*qtnode) == "left"; } );
  QtNode * const right_qtnode
    = FindFirstQtNode(q, [](QtNode * const qtnode) { return GetText(*qtnode) == "right"; } );
  q.DoCommand(new CommandMoveNode(q, left_qtnode , -100.0, 0.0));
  q.DoCommand(new CommandMoveNode(q, right_qtnode,  100.0, 0.0));
  q.DoCommand(new CommandSelectNode(q, left_qtnode));
  left_qtnode->setFocus();
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);

  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Right, Qt::ShiftModifier);
  q.keyPressEvent(&e);

  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 1);
  QVERIFY(e.isAccepted());
}

void ribi::cmap::QtConceptMapTest
  ::PressShiftRightSelectsNodeAdditively() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "left", false));
  q.DoCommand(new CommandCreateNewNode(q, "right", false));
  UnselectAllQtNodes(q);
  QtNode * const left_qtnode
    = FindFirstQtNode(q, [](QtNode * const qtnode) { return GetText(*qtnode) == "left"; } );
  QtNode * const right_qtnode
    = FindFirstQtNode(q, [](QtNode * const qtnode) { return GetText(*qtnode) == "right"; } );
  q.DoCommand(new CommandMoveNode(q, left_qtnode , -100.0, 0.0));
  q.DoCommand(new CommandMoveNode(q, right_qtnode,  100.0, 0.0));
  q.DoCommand(new CommandSelectNode(q, left_qtnode));
  left_qtnode->setFocus();
  assert(CountSelectedQtNodes(q) == 1);

  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Right, Qt::ShiftModifier);
  q.keyPressEvent(&e);

  QVERIFY(CountSelectedQtNodes(q) == 2);
  QVERIFY(e.isAccepted());
}

void ribi::cmap::QtConceptMapTest
  ::PressRightSelectsEdgeExclusively() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "left", false));
  q.DoCommand(new CommandCreateNewNode(q, "right", false));
  q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q, "between"));
  UnselectAll(q);
  QtNode * const left_qtnode
    = FindFirstQtNode(q, [](QtNode * const qtnode) { return GetText(*qtnode) == "left"; } );
  QtNode * const right_qtnode
    = FindFirstQtNode(q, [](QtNode * const qtnode) { return GetText(*qtnode) == "right"; } );
  q.DoCommand(new CommandMoveNode(q, left_qtnode , -100.0, 0.0));
  q.DoCommand(new CommandMoveNode(q, right_qtnode,  100.0, 0.0));
  q.DoCommand(new CommandSelectNode(q, left_qtnode));
  left_qtnode->setFocus();
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);

  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Right, Qt::NoModifier);
  q.keyPressEvent(&e);

  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  QVERIFY(GetText(*GetSelectedQtEdges(q)[0]) == "between");
  QVERIFY(e.isAccepted());
}

void ribi::cmap::QtConceptMapTest
  ::PressRightSelectsNodeExclusively() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "left", false));
  q.DoCommand(new CommandCreateNewNode(q, "right", false));
  QtNode * const left_qtnode = FindFirstQtNode(q, QtNodeHasName("left"));
  QtNode * const right_qtnode = FindFirstQtNode(q, QtNodeHasName("right"));
  q.DoCommand(new CommandMoveNode(q, left_qtnode , -100.0, 0.0));
  q.DoCommand(new CommandMoveNode(q, right_qtnode,  100.0, 0.0));
  q.DoCommand(new CommandUnselectNode(q, left_qtnode));
  q.DoCommand(new CommandUnselectNode(q, right_qtnode));
  q.DoCommand(new CommandSelectNode(q, left_qtnode));
  left_qtnode->setFocus();
  assert(CountSelectedQtNodes(q) == 1);

  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Right, Qt::NoModifier);
  q.keyPressEvent(&e);

  qDebug() << "\nCountSelectedQtNodes(q): " << CountSelectedQtNodes(q);

  assert(CountSelectedQtNodes(q) == 1);
  assert(GetText(*GetSelectedQtNodes(q)[0]) == "right");
  QVERIFY(GetText(*GetSelectedQtNodes(q)[0]) == "right");
  QVERIFY(CountSelectedQtNodes(q) == 1);
  QVERIFY(e.isAccepted());
}




void ribi::cmap::QtConceptMapTest::PressSpaceOnEmptyConceptMapIsRejected() const noexcept
{
  QtConceptMap m;
  m.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
  m.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressT() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_T, Qt::ControlModifier);
}

void ribi::cmap::QtConceptMapTest::PressZ() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier);
}

void ribi::cmap::QtConceptMapTest::QtNodeInCenterMustBeGold() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get1());
  m.SetMode(Mode::edit);
  m.show();
}

void ribi::cmap::QtConceptMapTest::QtNodesMustShowExampleWhenFocused() const noexcept
{
  QtConceptMap m;
  m.show();
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  for (int i=0; i!=100; ++i)
  {
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
    m.show();
  }
}

void ribi::cmap::QtConceptMapTest::SelectRandomNodeKeyboardEdit() const noexcept
{
  if (OnTravis()) return;
  QtConceptMap m;
  m.SetMode(Mode::rate);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();

  QVERIFY(GetSelectedQtNodes(m.GetScene()).size() == 2);

  std::vector<int> ids;
  for (int i=0; i!=100; ++i)
  {
    CheckInvariants(m);
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 10);
    m.show();
    CheckInvariants(m);
    //The QtNodes on the edges can also be selected
    const auto n_items_selected = m.GetScene().selectedItems().size();
    const auto n_qtnodes_selected = GetSelectedQtNodesAlsoOnQtEdge(m.GetScene()).size();
    QVERIFY(n_items_selected == 1);
    QVERIFY(n_qtnodes_selected == 1);
    assert(GetSelectedQtNodes(m.GetScene()).size() == 1);
    ids.push_back(GetSelectedQtNodes(m.GetScene())[0]->GetNode().GetId());
  }
  //Test if there are two IDs, both occuring multiple times
  QVERIFY(std::count(std::begin(ids),std::end(ids),ids[0])
    != static_cast<int>(ids.size())
  ); //Good enough?
}

void ribi::cmap::QtConceptMapTest::SetConceptMap4() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::qWait(100);
  m.show();
  m.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  m.show();
  QTest::qWait(100);
  m.show();
  m.SetConceptMap(ConceptMap());
  m.show();
  QTest::qWait(100);
  m.show();
}

void ribi::cmap::QtConceptMapTest::SetConceptMaps() const noexcept
{
  for (const auto concept_map: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    m.show();
    m.SetConceptMap(concept_map);
    m.show();
    m.SetConceptMap(ConceptMap());
    m.show();
    m.SetConceptMap(concept_map);
    m.show();
  }
}

void ribi::cmap::QtConceptMapTest::SetEmptyConceptMapOneSelectedEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetQtRatedConceptDialogExample());
  q.showFullScreen();
  //Select node on edge
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("prerequisite"))));
  q.show();
  q.SetConceptMap(ConceptMap());
  q.show();
}

void ribi::cmap::QtConceptMapTest::SetEmptyConceptMapOneSelectedNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetStarShaped());
  q.show();
  //Select node
  q.DoCommand(new CommandSelect(q, *FindFirstQtNode(q, QtNodeHasName("Useful"))));
  //m.DoCommand(new CommandSelect(m, "Useful"));
  q.show();
  q.SetConceptMap(ConceptMap());
  q.show();
}

void ribi::cmap::QtConceptMapTest::SetEmptyConceptMapTwoSelectedNodes() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetStarShaped());
  q.showFullScreen();
  q.SetConceptMap(ConceptMapFactory().GetQtRatedConceptDialogExample());
  q.show();
  //Select node on edge with examples
  q.DoCommand(new CommandSelect(q, *FindFirstQtEdge(q, QtEdgeHasName("strengthen"))));
  //m.DoCommand(new CommandSelect(m, "strengthen"));
  //Select node
  q.DoCommand(new CommandSelect(q, *FindFirstQtNode(q, QtNodeHasName("Order"))));
  //m.DoCommand(new CommandSelect(m, "Order"));
  q.show();
  q.SetConceptMap(ConceptMap());
  q.show();
}

void ribi::cmap::QtConceptMapTest::SettingConceptMapsEdgesQtEdgesNodesQtNodesMustMatch() const noexcept
{
  for (const auto conceptmap: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    m.show();
    QVERIFY(CountQtNodes(m.GetScene()) == 0);
    m.SetConceptMap(conceptmap);
    m.show();
    const auto n_nodes = static_cast<int>(boost::num_vertices(conceptmap));
    const auto n_edges = static_cast<int>(boost::num_edges(conceptmap));
    const auto n_qtnodes = CountQtNodes(m.GetScene());
    const auto n_qtedges = CountQtEdges(m.GetScene());
    QVERIFY(n_nodes == n_qtnodes);
    QVERIFY(n_edges == n_qtedges);
    QVERIFY(DoubleCheckEdgesAndNodes(m,n_edges,n_nodes));
  }
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithHeadCommand() const noexcept
{
  //When there are two selected nodes, an edge can be created
  //After adding the edges, only the edge will be selected
  //The edge its center concept will be between the two nodes
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
  try
  {
    const int n{2};
    for (int i=0; i!=n; ++i)
    {
      m.DoCommand(new CommandCreateNewNode(m));
    }
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  }
  catch (std::exception& e)
  {
    qDebug() << __func__ << ": caught exception " << e.what();
    QVERIFY(!"Should not get here");
  }

  try
  {
    m.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(m));
    QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  }
  catch (std::exception& e)
  {
    qDebug() << __func__ << ": caught exception " << e.what();
    QVERIFY(!"Should not get here");
  }

  //Preconditions
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  QVERIFY(qtedges.size() == 1);
  const auto qtedge = qtedges.back();
  QVERIFY(!qtedge->GetEdge().HasHeadArrow());

  try
  {
    m.DoCommand(new CommandToggleArrowHead(m));

    //Postconditions
    QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
    const auto qtedges_now = GetQtEdges(m.GetScene());
    QVERIFY(qtedges_now.size() == 1);
    const auto qtedge_now = qtedges_now.back();
    QVERIFY(qtedge_now->GetEdge().HasHeadArrow());

  }
  catch (std::exception& e)
  {
    qDebug() << __func__ << ": caught exception " << e.what();
    QVERIFY(!"Should not get here");
  }
  const std::string dot_filename{"create_one_edge_with_head_command.dot"};
  SaveToFile(m.GetConceptMap(), dot_filename);

  const auto concept_map_again = LoadFromFile(dot_filename);

  QVERIFY(HasSimilarData(m.GetConceptMap(), concept_map_again, 0.001));
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithHeadKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  QVERIFY(qtedges.size() == 1);
  const auto qtedge = qtedges.back();
  QVERIFY(qtedge->GetEdge().HasHeadArrow());
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithHeadAndToggleKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(qtedge->GetEdge().HasHeadArrow());
  }
  //Toggle
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  m.show();
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(!qtedge->GetEdge().HasHeadArrow());
  }
  //Toggle
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  m.show();
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(qtedge->GetEdge().HasHeadArrow());
  }
  //Toggle
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  m.show();
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(!qtedge->GetEdge().HasHeadArrow());
  }
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithHeadAndUndoKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(qtedge->GetEdge().HasHeadArrow());
  }
  //Undo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);
  m.show();
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(!qtedge->GetEdge().HasHeadArrow());
  }
  //Redo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier | Qt::ShiftModifier, 100);
  m.show();
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(qtedge->GetEdge().HasHeadArrow());
  }
  //Undo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);
  m.show();
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(!qtedge->GetEdge().HasHeadArrow());
  }
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithTailCommand() const noexcept
{
  //When there are two selected nodes, an edge can be created
  //After adding the edges, only the edge will be selected
  //The edge its center concept will be between the two nodes
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
  try
  {
    const int n{2};
    for (int i=0; i!=n; ++i)
    {
      m.DoCommand(new CommandCreateNewNode(m));
      m.show();
    }
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  }
  catch (std::exception& e)
  {
    qDebug() << __func__ << ": caught exception " << e.what();
    QVERIFY(!"Should not get here");
  }

  try
  {
    m.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(m));
    m.show();
    QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  }
  catch (std::exception& e)
  {
    qDebug() << __func__ << ": caught exception " << e.what();
    QVERIFY(!"Should not get here");
  }

  //Preconditions
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  QVERIFY(qtedges.size() == 1);
  const auto qtedge = qtedges.back();
  QVERIFY(!qtedge->GetEdge().HasHeadArrow());

  try
  {
    m.DoCommand(new CommandToggleArrowTail(m));
    m.show();
    //Postconditions
    QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
    const auto qtedges_now = GetQtEdges(m.GetScene());
    QVERIFY(qtedges_now.size() == 1);
    const auto qtedge_now = qtedges_now.back();
    QVERIFY(qtedge_now->GetEdge().HasTailArrow());

  }
  catch (std::exception& e)
  {
    qDebug() << __func__ << ": caught exception " << e.what();
    QVERIFY(!"Should not get here");
  }
  const std::string dot_filename{"create_one_edge_with_head_command.dot"};
  SaveToFile(m.GetConceptMap(), dot_filename);

  const auto concept_map_again = LoadFromFile(dot_filename);

  QVERIFY(HasSimilarData(m.GetConceptMap(), concept_map_again, 0.001));

}


void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithTailKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_T, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  const auto qtedges = GetQtEdges(m.GetScene());
  QVERIFY(qtedges.size() == 1);
  const auto qtedge = qtedges.back();
  QVERIFY(qtedge->GetEdge().HasTailArrow());
}

void ribi::cmap::QtConceptMapTest::SingleClickOnEmptyConceptMap() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  assert(boost::num_vertices(m.GetConceptMap()) == 0);
  QTest::mouseClick(m.viewport(), Qt::LeftButton);
  QVERIFY(boost::num_vertices(m.GetConceptMap()) == 0);
}

void ribi::cmap::QtConceptMapTest::SingleClickOnEmptyConceptMapIsNotAccepted() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  assert(boost::num_vertices(m.GetConceptMap()) == 0);
  const QPoint nothing;
  QMouseEvent e(QEvent::Type::MouseButtonPress, nothing, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  m.mousePressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::SingleClickOnNodeIsAccepted() const noexcept
{
  QtConceptMap m;
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier);
  UnselectAll(m);
  m.showFullScreen();
  const auto pos = m.mapFromScene(GetFirstQtNode(m)->GetCenterPos().toPoint());
  QMouseEvent e(QEvent::Type::MouseButtonPress, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  m.mousePressEvent(&e);
  QVERIFY(e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::SingleClickOnNodeSelectsNode() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier);
  QtNode * const qtnode = GetFirstQtNode(q);
  q.DoCommand(new CommandUnselectNode(q, qtnode));
  const auto pos = q.mapFromScene(qtnode->GetCenterPos().toPoint());
  QMouseEvent e(QEvent::Type::MouseButtonPress, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  q.mousePressEvent(&e);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapTest::TwoClicksOnEdgeSelectsAndUnselectsIt() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  m.showFullScreen();

  assert(CountSelectedQtEdges(m) == 0);

  //const auto pos = m.mapFromScene(GetCenterPos(*GetFirstQtNode(m)).toPoint());
  const auto pos = m.mapFromScene(GetCenterPos(*GetFirstQtEdge(m)).toPoint());
  qDebug() << "\npos: " << pos;

  QMouseEvent e(QEvent::Type::MouseButtonPress, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  m.mousePressEvent(&e);

  assert(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);

  m.mousePressEvent(&e);

  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::TwoClicksOnNodeSelectsAndUnselectsIt() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();

  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier);
  QtNode * const qtnode = GetFirstQtNode(q);
  q.DoCommand(new CommandUnselectNode(q, qtnode));

  const auto first_pos = q.mapFromScene(GetFirstQtNode(q)->GetCenterPos().toPoint());
  QMouseEvent first_click(QEvent::Type::MouseButtonPress, first_pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  q.mousePressEvent(&first_click);

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);

  const auto second_pos = q.mapFromScene(GetFirstQtNode(q)->GetCenterPos().toPoint());
  QMouseEvent second_click(QEvent::Type::MouseButtonPress, second_pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  q.mousePressEvent(&second_click);

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapTest::UninitializedModeFlags() const noexcept
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.show();
  m.SetMode(Mode::uninitialized);
  for (const auto qtnode: GetQtNodes(m.GetScene()))
  {
    if (IsQtCenterNode(qtnode))
    {
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsMovable));
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsSelectable));
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsFocusable));
    }
    else
    {
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsMovable));
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsSelectable));
      QVERIFY(!(qtnode->flags() & QGraphicsItem::ItemIsFocusable));
    }
  }
}
