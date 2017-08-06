#include "qtconceptmap_test.h"
#include "qtconceptmap.h"
#include "qtconceptmap.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <QApplication>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QKeyEvent>
#include <QPoint>
#include <boost/graph/isomorphism.hpp>

#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "qtconceptmapcollect.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommanddeleteselected.h"
#include "qtconceptmapcommandsetmode.h"
#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapexamplesitem.h"
#include "qtconceptmap.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapcommandselect.h"


void ribi::cmap::qtconceptmap_test::cannot_delete_center_node()
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "center", true));
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Delete, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::qtconceptmap_test::cannot_edit_center_node()
{
  QtConceptMap q;
  q.DoCommand(new CommandSetMode(q, Mode::edit));
  q.DoCommand(new CommandCreateNewNode(q, "center", true));
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_F2, Qt::NoModifier);
  q.SetPopupMode(PopupMode::muted);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::qtconceptmap_test::cannot_move_center_node()
{
  QtConceptMap q;
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
  QVERIFY(!e.isAccepted());
  QVERIFY(pos_before == pos_after);
}

void ribi::cmap::qtconceptmap_test::change_modes()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.SetMode(Mode::edit);
  m.show();
  m.SetMode(Mode::rate);
  m.show();
  m.SetMode(Mode::uninitialized);
  m.show();
}

void ribi::cmap::qtconceptmap_test::click_on_nothing_should_be_ignored()
{
  QtConceptMap m;
  QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(1.0,2.0),Qt::LeftButton,Qt::NoButton,Qt::NoModifier);
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.SetMode(Mode::edit);
  m.show();
  const auto qtnode = GetFirstQtNode(m.GetScene());
  const QPoint nothing{
    qtnode->mapToScene(qtnode->boundingRect().bottomRight()).toPoint()
  };
  QMouseEvent e(QEvent::Type::MouseButtonPress, nothing, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  m.mousePressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::qtconceptmap_test::concept_map_must_fit_window()
{
  #ifdef FIX_ISSUE_135 //https://github.com/richelbilderbeek/Brainweaver/issues/135
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  QTest::qWait(100);
  QVERIFY(!m.verticalScrollBar()->isVisible());
  QVERIFY(!m.horizontalScrollBar()->isVisible());
  #endif // FIX_ISSUE_135
}

void ribi::cmap::qtconceptmap_test::concept_map_must_fit_window_after_setting()
{
  QtConceptMap m;
  m.show();
  m.SetConceptMap(ConceptMapFactory().Get11());
  QTest::qWait(100);
  QVERIFY(!m.verticalScrollBar()->isVisible());
  QVERIFY(!m.horizontalScrollBar()->isVisible());
}

void ribi::cmap::qtconceptmap_test::create_one_edge_command()
{
  //When there are two selected nodes, an edge can be created
  //After adding the edges, only the edge will be selected
  //The edge its center concept will be between the two nodes
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
  const int n{2};
  for (int i=0; i!=n; ++i) {
    m.DoCommand(new CommandCreateNewNode(m));
  }
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  m.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(m));
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  m.show();
}

void ribi::cmap::qtconceptmap_test::create_one_edge_command_and_check_z_order()
{
  QtConceptMap m;
  m.show();
  //Create two nodes
  for (int i=0; i!=2; ++i) {
    m.DoCommand(new CommandCreateNewNode(m));
    m.show();
  }
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));

  //Create edge between nodes
  m.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(m));
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));

  const auto qtnodes = GetQtNodes(m.GetScene());
  const auto qtedges = GetQtEdges(m.GetScene());
  QVERIFY(qtnodes.size() == 2);
  QVERIFY(qtedges.size() == 1);
  const auto qtnode1 = qtnodes[0];
  const auto qtnode2 = qtnodes[1];
  const auto qtedge  = qtedges[0];
  QVERIFY(qtnode1->zValue() > qtedge->zValue());
  QVERIFY(qtnode2->zValue() > qtedge->zValue());
}

void ribi::cmap::qtconceptmap_test::create_one_edge_keyboard()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
}

void ribi::cmap::qtconceptmap_test::create_one_edge_keyboard_and_undo()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  //Undo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  //Redo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier | Qt::ShiftModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
}

void ribi::cmap::qtconceptmap_test::create_one_node_and_undo_command()
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
  m.DoCommand(new CommandCreateNewNode(m));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  m.Undo();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::qtconceptmap_test::create_one_node_and_undo_keyboard()
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);

  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::qtconceptmap_test::create_one_node_command()
{
  const std::string text = __func__;
  const bool is_center_node{false};
  const double x{314.15};
  const double y{42.69};

  QtConceptMap m;
  m.show();
  m.DoCommand(new CommandCreateNewNode(m, text, is_center_node, x, y));
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  const auto v = GetSelectedQtNodes(m.GetScene());
  QVERIFY(v.size() == 1);
  const auto n = v[0];
  QVERIFY(n);
  QVERIFY(std::abs(n->GetCenterX() - x) < 2.0);
  QVERIFY(std::abs(n->GetCenterY() - y) < 2.0);
}




void ribi::cmap::qtconceptmap_test::create_one_node_keyboard()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  const auto expected_vertices{1};
  const auto measured_vertices{boost::num_vertices(m.GetConceptMap())};
  const auto expected_edges{0};
  const auto measured_edges{boost::num_edges(m.GetConceptMap())};
  QVERIFY(measured_edges == expected_edges);
  QVERIFY(measured_vertices == expected_vertices);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
}

void ribi::cmap::qtconceptmap_test::create_one_node_mouse()
{
  QtConceptMap m;
  m.show();
  QTest::mouseDClick(m.viewport(), Qt::MouseButton::LeftButton, 0, QPoint(0.0,0.0), 100);
  m.show();
  const int n_nodes_in_scene{static_cast<int>(Collect<QtNode>(m.GetScene()).size())};
  const int n_nodes_in_conceptmap{static_cast<int>(boost::num_vertices(m.GetConceptMap()))};
  QVERIFY(n_nodes_in_scene == n_nodes_in_conceptmap);
}

void ribi::cmap::qtconceptmap_test::create_ten_nodes_and_undo_command()
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));

  const int n{10};
  for (int i=0; i!=n; ++i)
  {
    m.DoCommand(new CommandCreateNewNode(m));
    m.show();
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,i + 1));
  }
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,n));
  for (int i=0; i!=n; ++i) { m.Undo(); }
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
}

void ribi::cmap::qtconceptmap_test::create_ten_nodes_and_undo_keyboard()
{

}

void ribi::cmap::qtconceptmap_test::create_two_nodes_command()
{
  QtConceptMap m;
  m.show();
  for (int i=0; i!=2; ++i)
  {
    m.DoCommand(new CommandCreateNewNode(m));
  }
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
}



void ribi::cmap::qtconceptmap_test::create_two_nodes_keyboard()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
}

void ribi::cmap::qtconceptmap_test::default_construction()
{
  QtConceptMap m;
  m.show();

  //No nodes, no edges
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));

  //No examples selected, QGraphicsItem is created though (is that a good idea?)
  QVERIFY(!m.GetQtExamplesItem().GetBuddyItem());

  //No nodes with a Tool icon above it
  QVERIFY(!m.GetQtToolItem().isVisible());

  //No undo info yet
  QVERIFY(m.GetUndo().count() == 0);
}

void ribi::cmap::qtconceptmap_test::delete_node_that_is_connected_to_multiple_edges_keyboard()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().GetStarShaped());
  m.showFullScreen();
  //Select the node at the center of the star
  while (CountSelectedQtEdges(m.GetScene()) != 0
    || CountSelectedQtNodes(m.GetScene()) != 1
    || GetSelectedQtNodes(m.GetScene())[0]->GetNode().GetConcept().GetName()
      != std::string("X")
  ) {
    m.show();
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
  }
  m.show();
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  m.show();
  m.Undo();
  m.show();
  m.Redo();
  m.show();
}

void ribi::cmap::qtconceptmap_test::delete_node_that_is_head_of_edge_keyboard()
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

void ribi::cmap::qtconceptmap_test::delete_node_that_is_head_of_edge_and_undo_keyboard()
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

void ribi::cmap::qtconceptmap_test::delete_node_that_is_tail_of_edge_keyboard()
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

void ribi::cmap::qtconceptmap_test::delete_nodes_that_are_head_and_tail_of_edge_keyboard()
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
  std::srand(42);
  //Select only one single QtNode (not on edge)
  while (CountSelectedQtEdges(m.GetScene()) != 0
    || CountSelectedQtNodes(m.GetScene()) != 2
  ) {
    m.show();
    //Select first
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
    //Add-select second
    QTest::keyClick(&m, Qt::Key_Space, Qt::ShiftModifier, 100);
  }
  QVERIFY(CountSelectedQtEdges(m.GetScene()) == 0);
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,0));
}

void ribi::cmap::qtconceptmap_test::delete_one_edge_by_node_command()
{

}

void ribi::cmap::qtconceptmap_test::delete_one_edge_by_node_keyboard()
{

}

void ribi::cmap::qtconceptmap_test::delete_one_edge_command()
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

void ribi::cmap::qtconceptmap_test::delete_one_edge_keyboard()
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

void ribi::cmap::qtconceptmap_test::delete_one_node_command()
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

void ribi::cmap::qtconceptmap_test::delete_one_node_command_and_undo()
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

void ribi::cmap::qtconceptmap_test::delete_one_node_keyboard()
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


void ribi::cmap::qtconceptmap_test::delete_two_nodes_command()
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

void ribi::cmap::qtconceptmap_test::delete_two_nodes_keyboard()
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

void ribi::cmap::qtconceptmap_test::double_click()
{
  QtConceptMap m;
  m.show();
  assert(boost::num_vertices(m.GetConceptMap()) == 0);
  QTest::mouseDClick(m.viewport(), Qt::LeftButton);
  QVERIFY(boost::num_vertices(m.GetConceptMap()) == 1);
}

void ribi::cmap::qtconceptmap_test::double_click_twice()
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

void ribi::cmap::qtconceptmap_test::edit_mode_flags()
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

void ribi::cmap::qtconceptmap_test::rate_concept_map_has_less_focusable_items()
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

void ribi::cmap::qtconceptmap_test::rate_mode_flags()
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


void ribi::cmap::qtconceptmap_test::get_highlighter()
{
  QtConceptMap m;
  const auto& h = m.GetQtHighlighter();
  //Nothing to highlight, thus not item
  QVERIFY(!h.GetItem());
}

void ribi::cmap::qtconceptmap_test::hide_examples()
{
  QtConceptMap m;
  HideExamplesItem(m);
}

void ribi::cmap::qtconceptmap_test::is_command_put_on_undo_stack()
{
  QtConceptMap m;
  m.show();
  CommandCreateNewNode * const command = new CommandCreateNewNode(m);
  QVERIFY(m.GetUndo().count() == 0);
  m.DoCommand(command);
  m.show();
  QVERIFY(m.GetUndo().count() == 1);
}

void ribi::cmap::qtconceptmap_test::mouse_wheel()
{
  QtConceptMap m;
  m.show();
  QWheelEvent e(QPoint(10,10), 10,Qt::NoButton,Qt::NoModifier);
  m.wheelEvent(&e);
}

void ribi::cmap::qtconceptmap_test::n_center_nodes_and_qt_center_nodes_must_match_edit()
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

void ribi::cmap::qtconceptmap_test::n_center_nodes_and_qt_center_nodes_must_match_rate()
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

void ribi::cmap::qtconceptmap_test::n_center_nodes_and_qt_center_nodes_must_match_uninitialized()
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

void ribi::cmap::qtconceptmap_test::press_escape()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_Escape);
}

void ribi::cmap::qtconceptmap_test
  ::press_f1_on_empty_concept_map_is_rejected()
{
  QtConceptMap m;
  m.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  m.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::qtconceptmap_test::press_f2_on_empty_concept_map_is_rejected()
{
  QtConceptMap m;
  m.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  m.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::qtconceptmap_test::press_f2_cannot_edit_focal_question()
{
  QtConceptMap m;
  m.SetMode(Mode::edit);
  m.SetConceptMap(ConceptMapFactory().Get1());
  m.SetPopupMode(PopupMode::normal); //
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  m.keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::qtconceptmap_test::press_f2_can_edit_non_focal_question()
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



void ribi::cmap::qtconceptmap_test::press_f4_is_rejected()
{
  //F4 has no purpose
  QtConceptMap m;
  m.show();
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
  m.keyPressEvent(event);
  QVERIFY(!event->isAccepted());
}

void ribi::cmap::qtconceptmap_test::press_h()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier);
}

void ribi::cmap::qtconceptmap_test::press_n()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier);
}

void ribi::cmap::qtconceptmap_test::press_question_mark()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_Question);
}

void ribi::cmap::qtconceptmap_test::press_space_on_empty_concept_map_is_rejected()
{
  QtConceptMap m;
  m.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
  m.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::qtconceptmap_test::press_t()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_T, Qt::ControlModifier);
}

void ribi::cmap::qtconceptmap_test::press_z()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier);
}

void ribi::cmap::qtconceptmap_test::qtcenter_node_must_be_gold()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get1());
  m.SetMode(Mode::edit);
  m.show();
}

void ribi::cmap::qtconceptmap_test::qtnodes_must_show_example_when_focused()
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

void ribi::cmap::qtconceptmap_test::select_left_node_keyboard()
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  QTest::keyClick(&m, Qt::Key_Left, Qt::NoModifier, 100);
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(GetSelectedQtNodes(m.GetScene()).size() == 1);
  QVERIFY(GetSelectedQtEdges(m.GetScene()).size() == 0);
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
}

void ribi::cmap::qtconceptmap_test::select_random_node_keyboard_edit()
{
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

void ribi::cmap::qtconceptmap_test::set_concept_maps()
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

void ribi::cmap::qtconceptmap_test::set_empty_concept_map_one_selected_edge()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().GetQtRatedConceptDialogExample());
  m.show();
  //Select node on edge
  m.DoCommand(new CommandSelect(m, "prerequisite"));
  m.show();
  m.SetConceptMap(ConceptMap());
  m.show();
}

void ribi::cmap::qtconceptmap_test::set_empty_concept_map_one_selected_node()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().GetStarShaped());
  m.show();
  //Select node
  m.DoCommand(new CommandSelect(m, "Useful"));
  m.show();
  m.SetConceptMap(ConceptMap());
  m.show();
}

void ribi::cmap::qtconceptmap_test::set_empty_concept_map_two_selected_nodes()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().GetStarShaped());
  m.show();
  m.SetConceptMap(ConceptMapFactory().GetQtRatedConceptDialogExample());
  m.show();
  //Select node on edge with examples
  m.DoCommand(new CommandSelect(m, "strengthen"));
  //Select node
  m.DoCommand(new CommandSelect(m, "order"));
  m.show();
  m.SetConceptMap(ConceptMap());
  m.show();
}

void ribi::cmap::qtconceptmap_test::setting_concept_maps_edges_qtedges_nodes_qtnodes_must_match()
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

void ribi::cmap::qtconceptmap_test::create_one_edge_with_head_command()
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

  QVERIFY(m.GetConceptMap() == concept_map_again);

}

void ribi::cmap::qtconceptmap_test::create_one_edge_with_head_keyboard()
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

void ribi::cmap::qtconceptmap_test::create_one_edge_with_head_and_toggle_keyboard()
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

void ribi::cmap::qtconceptmap_test::create_one_edge_with_head_and_undo_keyboard()
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

void ribi::cmap::qtconceptmap_test::create_one_edge_with_tail_command()
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

  QVERIFY(m.GetConceptMap() == concept_map_again);

}


void ribi::cmap::qtconceptmap_test::create_one_edge_with_tail_keyboard()
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

void ribi::cmap::qtconceptmap_test::uninitialized_mode_flags()
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
