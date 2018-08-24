
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
#include "qtconceptmapcommandunselectall.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmaprateconceptdialogcloser.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapqtedge.h"

void ribi::cmap::QtConceptMapTest::CannotDeleteCenterNode() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.DoCommand(new CommandCreateNewNode(q, "center", NodeType::center));
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Delete, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::CannotEditCenterNode() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.DoCommand(new CommandSetMode(q, Mode::edit));
  q.DoCommand(new CommandCreateNewNode(q, "center", NodeType::center));
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_F2, Qt::NoModifier);
  QTimer::singleShot(100, qApp, SLOT(closeAllWindows()));
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::CannotMoveCenterNode() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.DoCommand(new CommandSetMode(q, Mode::edit));
  q.DoCommand(new CommandCreateNewNode(q, "center", NodeType::center));
  const auto selected_qt_nodes = GetSelectedQtNodesAlsoOnQtEdge(q);
  assert(selected_qt_nodes.size() == 1);
  const auto qtnode = selected_qt_nodes[0];
  const auto pos_before = qtnode->pos();
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_Down, Qt::ControlModifier);
  q.keyPressEvent(&e);
  q.show();
  const auto pos_after = qtnode->pos();
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

void ribi::cmap::QtConceptMapTest::ClickOnNothingShouldUnselectAll() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  q.SetConceptMap(ConceptMapFactory().Get2());
  q.SetMode(Mode::edit);
  q.show();
  q.DoCommand(new CommandSelectNode(q, GetFirstQtNode(q)));
  assert(CountSelectedQtNodes(q) > 0);
  const auto qtnode = GetFirstQtNode(q);
  const QPoint nothing{
    qtnode->mapToScene(qtnode->boundingRect().bottomRight()).toPoint()
  };
  QMouseEvent e(QEvent::Type::MouseButtonPress, nothing, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  q.mousePressEvent(&e);
  QVERIFY(e.isAccepted());
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeCommand() const noexcept
{
  //When there are two selected nodes, an edge can be created
  //After adding the edges, only the edge will be selected
  //The edge its center concept will be between the two nodes
  QtConceptMap m;
  m.showFullScreen();
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
  const int n{2};
  for (int i=0; i!=n; ++i) {
    m.DoCommand(new CommandCreateNewNode(m));
  }
  m.show();
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 2);
  m.DoCommand(new CommandCreateNewEdge(m));
  m.show();
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
  m.show();
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeCommandAndCheckZorder() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();

  //Create two nodes
  for (int i=0; i!=2; ++i) {
    m.DoCommand(new CommandCreateNewNode(m));
    m.show();
  }
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 2);

  //Create edge between nodes
  m.DoCommand(new CommandCreateNewEdge(m));
  m.show();
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);

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

void ribi::cmap::QtConceptMapTest::CreateOneEdgeKeyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&q, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(CountQtEdges(q) == 1);
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountSelectedQtEdges(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeKeyboardIncorrectly() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  assert(CountQtEdges(q) == 0);
  QTest::keyClick(&q, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(CountQtEdges(q) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeKeyboardAndUndo() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
  //Undo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 2);
  //Redo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier | Qt::ShiftModifier, 100);
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateOneNodeAndUndoCommand() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
  m.DoCommand(new CommandCreateNewNode(m));
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  m.Undo();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateOneNodeAndUndoKeyboard() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);

  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateOneNodeCommand() const noexcept
{
  const std::string text = __func__;
  const NodeType type{NodeType::normal};
  const double x{314.15};
  const double y{42.69};

  QtConceptMap m;
  m.showFullScreen();
  m.DoCommand(new CommandCreateNewNode(m, text, NodeType::normal, x, y));
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  const auto v = GetSelectedQtNodes(m.GetScene());
  QVERIFY(v.size() == 1);
  const auto n = v[0];
  QVERIFY(n);
  QVERIFY(std::abs(n->pos().x() - x) < 2.0);
  QVERIFY(std::abs(n->pos().y() - y) < 2.0);
}

void ribi::cmap::QtConceptMapTest::CreateOneNodeKeyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  q.show();
  const auto expected_vertices{1};
  const auto measured_vertices{boost::num_vertices(q.ToConceptMap())};
  const auto expected_edges{0};
  const auto measured_edges{boost::num_edges(q.ToConceptMap())};
  QVERIFY(measured_edges == expected_edges);
  QVERIFY(measured_vertices == expected_vertices);
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapTest::CreateOneNodeMouse() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::mouseDClick(q.viewport(), Qt::MouseButton::LeftButton, 0, QPoint(0.0,0.0), 100);
  q.show();
  const int n_nodes_in_scene{static_cast<int>(GetQtNodes(q).size())};
  const int n_nodes_in_conceptmap{static_cast<int>(boost::num_vertices(q.ToConceptMap()))};
  QVERIFY(n_nodes_in_scene == n_nodes_in_conceptmap);
}

void ribi::cmap::QtConceptMapTest::CreateTenNodesAndUndoCommand() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 0);

  const int n{10};
  for (int i=0; i!=n; ++i)
  {
    q.DoCommand(new CommandCreateNewNode(q));
    q.show();
    QVERIFY(CountSelectedQtEdges(q) == 0);
    QVERIFY(CountSelectedQtNodes(q) == i + 1);
  }
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == n);
  for (int i=0; i!=n; ++i) { q.Undo(); }
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateTenNodesAndUndoKeyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  for (int i = 0; i != 10; ++i)
  {
    QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 10);
    q.show();
  }
  assert(CountQtNodes(q) == 10);
  for (int i = 0; i != 10; ++i)
  {
    QTest::keyClick(&q, Qt::Key_Z, Qt::ControlModifier, 10);
    q.show();
  }
  QVERIFY(CountQtNodes(q) == 0);
}

void ribi::cmap::QtConceptMapTest::CreateTwoNodesCommand() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  for (int i=0; i!=2; ++i)
  {
    q.DoCommand(new CommandCreateNewNode(q));
  }
  q.show();
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
}



void ribi::cmap::QtConceptMapTest::CreateTwoNodesKeyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  q.show();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier, 100);
  q.show();
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 2);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 2);
}

void ribi::cmap::QtConceptMapTest::DefaultConstruction() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();

  //No nodes, no edges
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 0);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);

  //No nodes with a Tool icon above it
  QVERIFY(!q.GetQtToolItem().isVisible());

  //No undo info yet
  QVERIFY(q.GetUndo().count() == 0);
}

void ribi::cmap::QtConceptMapTest::DeleteNodeThatIsConnectedToMultipleEdgesKeyboard() const noexcept
{
  QtConceptMap q;
  q.showFullScreen();
  const auto concept_map = ConceptMapFactory().GetStarShaped();
  q.SetConceptMap(concept_map);
  q.show();
  //Select the node at the center of the star
  while (CountSelectedQtEdges(q) != 0
    || CountSelectedQtNodes(q) != 1
    || (CountSelectedQtNodes(q) > 0 && GetNode(*GetSelectedQtNodes(q)[0]).GetConcept().GetName()
      != std::string("X"))
  )
  {
    CheckInvariants(q);
    q.show();
    QTest::keyClick(&q, Qt::Key_Space, Qt::NoModifier, 100);
    CheckInvariants(q);
  }
  q.show();
  QTest::keyClick(&q, Qt::Key_Delete, Qt::NoModifier, 100); //Or here
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
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
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
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::DeleteNodeThatIsHeadOfEdgeAndUndoKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  //Create nodes and edge
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
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
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
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
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
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
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
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
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
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
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 2);
  //Deleting the QtNode should also delete the QtEdge that is connected to it
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, delay);
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::DeleteOneEdgeByNodeCommand() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  q.showFullScreen();
  SetSelectedness(true, *GetFirstQtNode(q));
  assert(CountQtEdges(q) == 1);
  assert(CountQtNodes(q) == 2);
  q.DoCommand(new CommandDeleteSelected(q));
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapTest::DeleteOneEdgeByNodeKeyboard() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdgeNoCenter());
  q.showFullScreen();
  SetSelectedness(true, *GetFirstQtNode(q));
  assert(CountQtEdges(q) == 1);
  assert(CountQtNodes(q) == 2);
  QTest::keyClick(&q, Qt::Key_Delete, Qt::NoModifier, 100);
  QVERIFY(CountQtEdges(q) == 0);
  QVERIFY(CountQtNodes(q) == 1);
}

void ribi::cmap::QtConceptMapTest::DeleteOneEdgeCommand() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  for (int i = 0; i != 2; ++i)
  {
    m.DoCommand(new CommandCreateNewNode(m));
    m.show();
  }
  m.DoCommand(new CommandCreateNewEdge(m));
  m.show();
  assert(CountQtEdges(m) == 1);
  m.DoCommand(new CommandDeleteSelected(m));
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
}

void ribi::cmap::QtConceptMapTest::DeleteOneEdgeKeyboard() const noexcept
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
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::DeleteOneNodeCommand() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  m.DoCommand(new CommandCreateNewNode(m));
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  m.DoCommand(new CommandDeleteSelected(m));
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::DeleteOneNodeCommandAndUndo() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  m.DoCommand(new CommandCreateNewNode(m));
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  m.DoCommand(new CommandDeleteSelected(m));
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  m.Undo();
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
}

void ribi::cmap::QtConceptMapTest::DeleteOneNodeKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 1);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 1);
  QTest::keyClick(&m, Qt::Key_Delete, Qt::NoModifier, 100);
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
}


void ribi::cmap::QtConceptMapTest::DeleteTwoNodesCommand() const noexcept
{
  QtConceptMap m;
  m.show();
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  for (int i{0}; i!=2; ++i)
  {
    m.DoCommand(new CommandCreateNewNode(m));
    m.show();
  }
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 2);

  for (int i{0}; i!=2; ++i) {
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 100);
    m.show();
    const int n_selected_measured{m.scene()->selectedItems().count()};
    const int n_selected_expected{1};
    assert(n_selected_measured == n_selected_expected);
    m.DoCommand(new CommandDeleteSelected(m));
    m.show();
  }
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
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
  QVERIFY(CountQtEdges(m) == 0);
  QVERIFY(CountQtNodes(m) == 0);
  QVERIFY(CountSelectedQtEdges(m) == 0);
  QVERIFY(CountSelectedQtNodes(m) == 0);
}

void ribi::cmap::QtConceptMapTest::DoubleClick() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  m.SetMode(Mode::edit);
  assert(boost::num_vertices(m.ToConceptMap()) == 0);
  QTest::mouseDClick(m.viewport(), Qt::LeftButton);
  QVERIFY(boost::num_vertices(m.ToConceptMap()) == 1);
}

void ribi::cmap::QtConceptMapTest::DoubleClickTwice() const noexcept
{
  QtConceptMap m;
  m.SetMode(Mode::edit);
  m.show();
  assert(boost::num_vertices(m.ToConceptMap()) == 0);
  //Creates a new node
  QTest::mouseDClick(m.viewport(), Qt::LeftButton);
  m.show();
  assert(boost::num_vertices(m.ToConceptMap()) == 1);
  //Does not create a new node, as the double-click took place on an existing node
  QTest::mouseDClick(m.viewport(), Qt::LeftButton);
  m.show();
  QVERIFY(boost::num_vertices(m.ToConceptMap()) == 1);
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
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsSelectable));
      QVERIFY( (qtnode->flags() & QGraphicsItem::ItemIsFocusable));
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
  auto& h = m.GetQtHighlighter();
  //Nothing to highlight, thus not item
  QVERIFY(!h.GetItem());
}

void ribi::cmap::QtConceptMapTest::GetHighlighterConst() const noexcept
{
  const QtConceptMap m;
  const auto& h = m.GetQtHighlighter();
  //Nothing to highlight, thus not item
  QVERIFY(!h.GetItem());
}

void ribi::cmap::QtConceptMapTest::GetItemBelowCursor() const noexcept
{
  QtConceptMap q;
  QVERIFY(!::ribi::cmap::GetItemBelowCursor(q, QPointF(-50.0, -50.0)));
}

void ribi::cmap::QtConceptMapTest::GetQtNodesAlsoOnQtEdge() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  const auto qtnodes = ::ribi::cmap::GetQtNodesAlsoOnQtEdge(q);
  QVERIFY(qtnodes.size() == 3);
}

void ribi::cmap::QtConceptMapTest::HasSelectedItems() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  assert(!::ribi::cmap::HasSelectedItems(q));
  SetSelectedness(true, *GetFirstQtNode(q));
  QVERIFY(::ribi::cmap::HasSelectedItems(q));
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

void ribi::cmap::QtConceptMapTest::Press1TogglesTail() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  SetSelectedness(true, *GetFirstQtEdge(q));
  q.showFullScreen();
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountQtArrowTails(q) == 0);
  QTest::keyClick(&q, Qt::Key_1, Qt::NoModifier);
  QVERIFY(CountQtArrowTails(q) != 0);
}

void ribi::cmap::QtConceptMapTest::Press2TogglesHead() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  SetSelectedness(true, *GetFirstQtEdge(q));
  q.showFullScreen();
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountQtArrowHeads(q) == 1);
  QTest::keyClick(&q, Qt::Key_2, Qt::NoModifier);
  QVERIFY(CountQtArrowHeads(q) != 1);
}

void ribi::cmap::QtConceptMapTest::PressCtrlRightMovesNonCentralNode() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q, "from", NodeType::normal));

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

void ribi::cmap::QtConceptMapTest::PressCtrlHTogglesHead() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  SetSelectedness(true, *GetFirstQtEdge(q));
  q.showFullScreen();
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountQtArrowHeads(q) == 1);
  QTest::keyClick(&q, Qt::Key_H, Qt::ControlModifier);
  QVERIFY(CountQtArrowHeads(q) != 1);
}


void ribi::cmap::QtConceptMapTest::PressCtrlTTogglesTail() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetTwoNodeOneEdge());
  SetSelectedness(true, *GetFirstQtEdge(q));
  q.showFullScreen();
  assert(CountSelectedQtEdges(q) == 1);
  assert(CountQtArrowTails(q) == 0);
  QTest::keyClick(&q, Qt::Key_T, Qt::ControlModifier);
  QVERIFY(CountQtArrowTails(q) != 0);
}

void ribi::cmap::QtConceptMapTest::PressEscapeMustBeIgnored() const noexcept
{
  QtConceptMap m;
  m.show();
  assert(!m.isHidden());
  QTest::keyClick(&m, Qt::Key_Escape);
  QVERIFY(!m.isHidden());
}

void ribi::cmap::QtConceptMapTest::PressF1OnCenterNodeIsRejected() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyCenterNode());
  SetSelectedness(true, *GetFirstQtNode(q));
  q.show();
  assert(CountSelectedQtNodes(q) == 1);
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF1OnEmptyConceptMapIsRejected() const noexcept
{
  QtConceptMap q;
  q.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF1OnSingleNodeEditConceptMapIsRejected() const noexcept
{
  if (OnTravis()) return;

  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyNode());
  q.SetMode(Mode::edit);
  SetSelectedness(true, *GetFirstQtNode(q));
  q.showFullScreen();
  assert(CountSelectedQtNodes(q) == 1);
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  QtRateConceptDialogCloser c;
  QTimer::singleShot(100, &c, SLOT(PressOk()));
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF1OnSingleNodeRateConceptMapIsAccepted() const noexcept
{
  if (OnTravis()) return;

  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyNode());
  q.SetMode(Mode::rate);
  SetSelectedness(true, *GetFirstQtNode(q));
  q.show();
  assert(CountSelectedQtNodes(q) == 1);
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  QtRateConceptDialogCloser c;
  QTimer::singleShot(100, &c, SLOT(PressOk()));
  q.keyPressEvent(&e);
  QVERIFY(e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF1OnSingleNodeUninitializedConceptMapIsRejected() const noexcept
{
  if (OnTravis()) return;

  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyNode());
  q.SetMode(Mode::uninitialized);
  SetSelectedness(true, *GetFirstQtNode(q));
  q.show();
  assert(CountSelectedQtNodes(q) == 0); //Cannot select nodes in uninitialized mode
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  QtRateConceptDialogCloser c;
  QTimer::singleShot(100, &c, SLOT(PressOk()));
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF1OnMultipleSelectedQtNodesIsRejected() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetUnrated());
  SetSelectedness(true, *GetFirstQtNode(q));
  SetSelectedness(true, *GetLastQtNode(q));
  q.show();
  assert(CountSelectedQtNodes(q) == 2);
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF2OnCenterNodeIsRejected() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyCenterNode());
  SetSelectedness(true, *GetFirstQtNode(q));
  q.show();
  assert(CountSelectedQtNodes(q) == 1);
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF2OnEmptyConceptMapIsRejected() const noexcept
{
  QtConceptMap q;
  q.show();
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF2OnMultipleSelectedQtNodesIsRejected() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetUnrated());
  SetSelectedness(true, *GetFirstQtNode(q));
  SetSelectedness(true, *GetLastQtNode(q));
  q.show();
  assert(CountSelectedQtNodes(q) == 2);
  QKeyEvent e(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  q.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::PressF2CannotEditFocalQuestion() const noexcept
{
  QtConceptMap m;
  m.SetMode(Mode::edit);
  m.SetConceptMap(ConceptMapFactory().Get1());
  QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  QTimer::singleShot(100, qApp, SLOT(closeAllWindows()));
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
  QTimer::singleShot(100, qApp, SLOT(closeAllWindows()));
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
  QTimer::singleShot(100, qApp, SLOT(closeAllWindows()));
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
  q.DoCommand(new CommandCreateNewNode(q, "left", NodeType::normal));
  q.DoCommand(new CommandCreateNewNode(q, "right", NodeType::normal));
  q.DoCommand(new CommandCreateNewEdge(q, "between"));
  q.DoCommand(new CommandUnselectAll(q));
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
  q.DoCommand(new CommandCreateNewNode(q, "left", NodeType::normal));
  q.DoCommand(new CommandCreateNewNode(q, "right", NodeType::normal));
  q.DoCommand(new CommandUnselectAll(q));
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
  q.DoCommand(new CommandCreateNewNode(q, "left", NodeType::normal));
  q.DoCommand(new CommandCreateNewNode(q, "right", NodeType::normal));
  q.DoCommand(new CommandCreateNewEdge(q, "between"));
  q.DoCommand(new CommandUnselectAll(q));
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
  q.DoCommand(new CommandCreateNewNode(q, "left", NodeType::normal));
  q.DoCommand(new CommandCreateNewNode(q, "right", NodeType::normal));
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
    ids.push_back(GetNode(*GetSelectedQtNodes(m.GetScene())[0]).GetId());
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
  m.SetConceptMap(ConceptMapFactory().GetThreeNodeTwoEdge());
  m.show();
  m.SetConceptMap(ConceptMap());
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
    QVERIFY(CountQtEdges(m) == n_edges);
    QVERIFY(CountQtNodes(m) == n_nodes);
  }
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithHeadCommand() const noexcept
{
  QtConceptMap m;
  m.show();
  m.DoCommand(new CommandCreateNewNode(m));
  m.DoCommand(new CommandCreateNewNode(m));
  m.DoCommand(new CommandCreateNewEdge(m));

  assert(CountQtEdges(m) == 1);
  assert(CountQtNodes(m) == 2);
  assert(CountSelectedQtEdges(m) == 1);
  assert(CountSelectedQtNodes(m) == 0);

  //Before
  const auto qtedges_before = GetQtEdges(m.GetScene());
  assert(qtedges_before.size() == 1);
  const auto qtedge_before = qtedges_before.back();
  const bool has_arrow_head_before{
    HasHeadArrow(*qtedge_before)
  };

  //Action
  m.DoCommand(new CommandToggleArrowHead(m));

  //After
  const auto qtedges_after = GetQtEdges(m.GetScene());
  assert(qtedges_after.size() == 1);
  const auto qtedge_after = qtedges_after.back();
  const bool has_arrow_head_after{
    HasHeadArrow(*qtedge_after)
  };
  QVERIFY(has_arrow_head_before != has_arrow_head_after);
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithHeadKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  assert(CountQtEdges(m) == 1);
  assert(CountQtNodes(m) == 2);
  assert(CountSelectedQtEdges(m) == 1);
  assert(CountSelectedQtNodes(m) == 0);

  //Before
  const auto qtedges_before = GetQtEdges(m.GetScene());
  assert(qtedges_before.size() == 1);
  const auto qtedge_before = qtedges_before.back();
  const bool has_head_arrow_before{
    HasHeadArrow(*qtedge_before)
  };

  //Action
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);

  //After
  const auto qtedges_after = GetQtEdges(m.GetScene());
  assert(qtedges_after.size() == 1);
  const auto qtedge_after = qtedges_after.back();
  const bool has_head_arrow_after{
    HasHeadArrow(*qtedge_after)
  };
  QVERIFY(has_head_arrow_before != has_head_arrow_after);
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

  assert(CountQtEdges(m) == 1);
  assert(CountQtNodes(m) == 2);
  assert(CountSelectedQtEdges(m) == 1);
  assert(CountSelectedQtNodes(m) == 0);
  for (int i = 0; i != 2; ++i)
  {
    QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
    {
      const auto qtedges = GetQtEdges(m.GetScene());
      assert(qtedges.size() == 1);
      const auto qtedge = qtedges.back();
      QVERIFY(!qtedge->GetEdge().HasHeadArrow());
    }
    QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
    {
      const auto qtedges = GetQtEdges(m.GetScene());
      assert(qtedges.size() == 1);
      const auto qtedge = qtedges.back();
      QVERIFY(qtedge->GetEdge().HasHeadArrow());
    }
  }
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithHeadAndUndoKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);
  //Remove head
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  //Add head
  QTest::keyClick(&m, Qt::Key_H, Qt::ControlModifier, 100);
  QVERIFY(CountQtEdges(m) == 1);
  QVERIFY(CountQtNodes(m) == 2);
  QVERIFY(CountSelectedQtEdges(m) == 1);
  QVERIFY(CountSelectedQtNodes(m) == 0);
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    assert(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(HasHeadArrow(*qtedge));
  }

  //Undo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);
  m.show();

  {
    const auto qtedges = GetQtEdges(m.GetScene());
    assert(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(!HasHeadArrow(*qtedge));
  }
  //Redo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier | Qt::ShiftModifier, 100);
  m.show();

  {
    const auto qtedges = GetQtEdges(m.GetScene());
    assert(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(HasHeadArrow(*qtedge));
  }
  //Undo
  QTest::keyClick(&m, Qt::Key_Z, Qt::ControlModifier, 100);
  m.show();
  {
    const auto qtedges = GetQtEdges(m.GetScene());
    assert(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(!HasHeadArrow(*qtedge));
  }
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithTailCommand() const noexcept
{
  //When there are two selected nodes, an edge can be created
  //After adding the edges, only the edge will be selected
  //The edge its center concept will be between the two nodes
  QtConceptMap m;
  m.show();

  //Create nodes
  {
    const int n{2};
    for (int i=0; i!=n; ++i)
    {
      m.DoCommand(new CommandCreateNewNode(m));
      m.show();
    }
    assert(CountQtEdges(m) == 0);
    assert(CountQtNodes(m) == 2);
    assert(CountSelectedQtEdges(m) == 0);
    assert(CountSelectedQtNodes(m) == 2);
  }

  //Create edge, arrow will be put at head or tail randomly
  {
    m.DoCommand(new CommandCreateNewEdge(m));
    m.show();
    assert(CountQtEdges(m) == 1);
    assert(CountQtNodes(m) == 2);
    assert(CountSelectedQtEdges(m) == 1);
    assert(CountSelectedQtNodes(m) == 0);
  }
  //Preconditions
  const auto qtedges = GetQtEdges(m.GetScene());
  assert(qtedges.size() == 1);
  const auto qtedge = qtedges.back();
  const bool has_tail_arrow_before{
    qtedge->GetEdge().HasTailArrow()
  };

  //Create tail
  {
    m.DoCommand(new CommandToggleArrowTail(m));
    m.show();
    const auto qtedges_now = GetQtEdges(m.GetScene());
    assert(qtedges_now.size() == 1);
    const auto qtedge_now = qtedges_now.back();
    const bool has_tail_arrow_after{
      qtedge_now->GetEdge().HasTailArrow()
    };
    QVERIFY(has_tail_arrow_before != has_tail_arrow_after);
  }
}

void ribi::cmap::QtConceptMapTest::CreateOneEdgeWithTailKeyboard() const noexcept
{
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  QTest::keyClick(&m, Qt::Key_E, Qt::ControlModifier, 100);

  assert(CountQtEdges(m) == 1);
  assert(CountQtNodes(m) == 2);
  assert(CountSelectedQtEdges(m) == 1);
  assert(CountSelectedQtNodes(m) == 0);

  //Before
  const auto qtedges_before = GetQtEdges(m.GetScene());
  assert(qtedges_before.size() == 1);
  const auto qtedge_before = qtedges_before.back();
  assert(HasTailArrow(*qtedge_before) == qtedge_before->GetEdge().HasTailArrow());
  const bool has_tail_arrow_before{
    HasTailArrow(*qtedge_before)
  };

  //Action
  QTest::keyClick(&m, Qt::Key_T, Qt::ControlModifier, 100);

  //After
  const auto qtedges_after = GetQtEdges(m.GetScene());
  assert(qtedges_after.size() == 1);
  const auto qtedge_after = qtedges_after.back();
  assert(HasTailArrow(*qtedge_after) == qtedge_after->GetEdge().HasTailArrow());
  const bool has_tail_arrow_after{
    HasTailArrow(*qtedge_after)
  };

  QVERIFY(has_tail_arrow_before != has_tail_arrow_after);
}

void ribi::cmap::QtConceptMapTest::SetMode() const noexcept
{
  QtConceptMap m;
  m.show();
  m.SetConceptMap(ConceptMapFactory().GetUnrated());
  m.show();
  m.SetMode(Mode::uninitialized);
  m.show();
  m.SetMode(Mode::edit);
  m.show();
  m.SetMode(Mode::rate);
  m.show();
  m.SetMode(Mode::uninitialized);
  m.show();
}

void ribi::cmap::QtConceptMapTest::SingleClickOnEmptyConceptMap() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  assert(boost::num_vertices(m.ToConceptMap()) == 0);
  QTest::mouseClick(m.viewport(), Qt::LeftButton);
  QVERIFY(boost::num_vertices(m.ToConceptMap()) == 0);
}

void ribi::cmap::QtConceptMapTest::SingleClickOnEmptyConceptMapIsNotAccepted() const noexcept
{
  QtConceptMap m;
  m.showFullScreen();
  assert(boost::num_vertices(m.ToConceptMap()) == 0);
  const QPoint nothing;
  QMouseEvent e(QEvent::Type::MouseButtonPress, nothing, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  m.mousePressEvent(&e);
  QVERIFY(!e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::SingleClickOnNodeIsAccepted() const noexcept
{
  QtConceptMap q;
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier);
  q.DoCommand(new CommandUnselectAll(q));
  q.showFullScreen();
  const auto pos = q.mapFromScene(GetFirstQtNode(q)->pos().toPoint());
  QMouseEvent e(QEvent::Type::MouseButtonPress, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  q.mousePressEvent(&e);
  QVERIFY(e.isAccepted());
}

void ribi::cmap::QtConceptMapTest::SingleClickOnNodeSelectsNode() const noexcept
{
  QtConceptMap q;
  q.SetMode(Mode::edit);
  q.show();
  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier);
  QtNode * const qtnode = GetFirstQtNode(q);
  q.DoCommand(new CommandUnselectNode(q, qtnode));
  const auto pos = q.mapFromScene(qtnode->pos().toPoint());
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
  QMouseEvent e(QEvent::Type::MouseButtonPress, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  m.mousePressEvent(&e);

  m.mousePressEvent(&e);
}

void ribi::cmap::QtConceptMapTest::TwoClicksOnNodeSelectsAndUnselectsIt() const noexcept
{
  QtConceptMap q;
  q.SetMode(Mode::edit);
  q.showFullScreen();

  QTest::keyClick(&q, Qt::Key_N, Qt::ControlModifier);
  q.show();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 1);

  QtNode * const qtnode = GetFirstQtNode(q);
  assert(!IsQtNodeOnEdge(qtnode));
  assert(IsSelectable(*qtnode));
  q.DoCommand(new CommandUnselectNode(q, qtnode));
  q.show();

  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);

  const auto first_pos = q.mapFromScene(GetFirstQtNode(q)->pos().toPoint());
  QMouseEvent first_click(QEvent::Type::MouseButtonPress, first_pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
  q.mousePressEvent(&first_click);

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 1);

  const auto second_pos = q.mapFromScene(GetFirstQtNode(q)->pos().toPoint());
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
