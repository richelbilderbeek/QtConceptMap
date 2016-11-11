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
#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapexamplesitem.h"
#include "qtconceptmap.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapbrushfactory.h"

//#define FIX_ISSUE_83
//#define FIX_ISSUE_104

void ribi::cmap::qtconceptmap_test::cannot_click_on_focal_question()
{
  #ifdef FIX_ISSUE_104
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get1());
  m.SetMode(Mode::edit);
  m.show();
  QtNode * const qtnode = GetFirstQtNode(m.GetScene());
  qtnode->setSelected(true);
  qtnode->setFocus();
  QKeyEvent e(QEvent::Type::KeyPress, Qt::Key_F2, Qt::NoModifier);
  m.keyPressEvent(&e);
  QVERIFY(!e.isAccepted());
  QTest::qWait(100000);
  #endif // FIX_ISSUE_104
}

void ribi::cmap::qtconceptmap_test::aaa_click_on_nothing()
{
  #ifdef FIX_ISSUE_83
  QtConceptMap m;
  QMouseEvent(QMouseEvent::MouseButtonPress, QPoint(1.0,2.0),Qt::LeftButton,Qt::NoButton,Qt::NoModifier);
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.SetMode(Mode::edit);
  m.show();
  const auto qtnode = GetFirstQtNode(m.GetScene());
  const QPoint nothing{
    qtnode->mapToScene(qtnode->boundingRect().bottomRight()).toPoint()
  };
  QTest::mouseClick(&m ,Qt::LeftButton, 0, nothing);
  #endif // FIX_ISSUE_83
}

void ribi::cmap::qtconceptmap_test::aaa_click_on_qtnode()
{
  #ifdef FIX_ISSUE_83
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.SetMode(Mode::edit);
  m.show();
  const auto qtnode = GetFirstQtNode(m.GetScene());
  const QPoint here{qtnode->mapToScene(qtnode->boundingRect().center()).toPoint()};
  QTest::mouseClick(&m ,Qt::LeftButton, 0, here);
  QTest::qWait(10000000);
  #endif // FIX_ISSUE_83
}

void ribi::cmap::qtconceptmap_test::qttoolitem_should_remain_when_moving_out_and_in_of_screen()
{
  #ifdef FIX_ISSUE_83
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get2());
  m.SetMode(Mode::edit);
  m.showFullScreen();

  //Select a QtNode
  while (GetSelectedQtNodes(m.GetScene()).size() != 1)
  {
    QTest::keyPress(&m, Qt::Key_Space, 0, 100);
  }
  QVERIFY(m.GetQtToolItem().isVisible());
  //Move it out of the screen
  for (int i=0; i!=80; ++i)
  {
    qDebug() << i;
    const auto pos_before = m.GetQtToolItem().pos();
    QTest::keyPress(&m, Qt::Key_Left, Qt::ControlModifier);
    QTest::qWait(10);
    const auto pos_after = m.GetQtToolItem().pos();
    QVERIFY(pos_before != pos_after);
  }
  //Move it back in the screen
  for (int i=0; i!=80; ++i)
  {
    qDebug() << i;
    const auto pos_before = m.GetQtToolItem().pos();
    QTest::keyPress(&m, Qt::Key_Right, Qt::ControlModifier);
    QTest::qWait(10);
    const auto pos_after = m.GetQtToolItem().pos();
    QVERIFY(pos_before != pos_after);
  }
  //Weird, it always stays visible, even though me, a mere human, cannot see it on my screen
  QVERIFY(m.GetQtToolItem().isVisible());

  qDebug() << "SOLVED ISSUE #83!";
  QTest::qWait(100000);
  #endif // FIX_ISSUE_83
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

void ribi::cmap::qtconceptmap_test::concept_map_must_fit_window()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.show();
  QTest::qWait(100);
  QVERIFY(!m.verticalScrollBar()->isVisible());
  QVERIFY(!m.horizontalScrollBar()->isVisible());
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
    m.DoCommand(
      new CommandCreateNewNode(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem(),
        0.0,
        0.0
      )
    );
  }
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  m.DoCommand(
    new CommandCreateNewEdgeBetweenTwoSelectedNodes(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem()
    )
  );
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
    m.DoCommand(
      new CommandCreateNewNode(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem(),
        0.0,
        0.0
      )
    );
    m.show();
  }
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));

  //Create edge between nodes
  m.DoCommand(
    new CommandCreateNewEdgeBetweenTwoSelectedNodes(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem()
    )
  );
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
  m.DoCommand(
    new CommandCreateNewNode(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem(),
      0.0,
      0.0
    )
  );
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
  const double x{314.15};
  const double y{42.69};
  QtConceptMap m;
  m.show();
  m.DoCommand(
    new CommandCreateNewNode(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem(),
      x,
      y
    )
  );
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
  for (int i=0; i!=n; ++i) {
    m.DoCommand(
      new CommandCreateNewNode(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem(),
        0.0,
        0.0
      )
    );
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
  for (int i=0; i!=2; ++i) {
    m.DoCommand(
      new CommandCreateNewNode(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem(),
        0.0,
        0.0
      )
    );
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
  for (int i=0; i!=n; ++i) {
    m.DoCommand(
      new CommandCreateNewNode(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem(),
        0.0,
        0.0
      )
    );
    m.show();
  }
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,2));
  m.DoCommand(
    new CommandCreateNewEdgeBetweenTwoSelectedNodes(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem()
    )
  );
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
  m.DoCommand(
    new CommandDeleteSelected(
      m.GetConceptMap(),
      m.GetScene(),
      m.GetQtToolItem()
    )
  );
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
  m.DoCommand(
    new CommandCreateNewNode(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem(),
      0.0,
      0.0
    )
  );
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  m.DoCommand(
    new CommandDeleteSelected(
      m.GetConceptMap(),
      m.GetScene(),
      m.GetQtToolItem()
    )
  );
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
}

void ribi::cmap::qtconceptmap_test::delete_one_node_command_and_undo()
{
  QtConceptMap m;
  m.show();
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,0));
  m.DoCommand(
    new CommandCreateNewNode(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem(),
      0.0,
      0.0
    )
  );
  m.show();
  QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,0,1));
  QVERIFY(DoubleCheckEdgesAndNodes(m,0,1));
  m.DoCommand(
    new CommandDeleteSelected(
      m.GetConceptMap(),
      m.GetScene(),
      m.GetQtToolItem()
    )
  );
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
  for (int i{0}; i!=2; ++i) {
    m.DoCommand(
      new CommandCreateNewNode(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem(),
        0.0,
        0.0
      )
    );
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
    m.DoCommand(
      new CommandDeleteSelected(
        m.GetConceptMap(),
        m.GetScene(),
        m.GetQtToolItem()
      )
    );
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
  if (n_rate >= n_edit)
  {
    qCritical() << "n_rate: " << n_rate;
    qCritical() << "n_edit: " << n_edit;
    //QTest::qWait(10000);
  }
  QVERIFY(n_rate < n_edit);

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
  CommandCreateNewNode * const command {
    new CommandCreateNewNode(
      m.GetConceptMap(),
      Mode::uninitialized,
      m.GetScene(),
      m.GetQtToolItem(),
      0.0,
      0.0
    )
  };
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

void ribi::cmap::qtconceptmap_test::press_f2_cannot_edit_focal_question_on_fuller_conceptmap()
{
  #ifdef FIX_ISSUE_113
  QtConceptMap m;
  m.SetMode(Mode::edit);
  m.SetConceptMap(ConceptMapFactory().Get11());
  assert(CountCenterNodes(GetNodes(m.GetConceptMap())) == 1);
  assert(ribi::cmap::CountQtCenterNodes(m.GetScene()) == 1);

  m.SetPopupMode(PopupMode::normal); //
  //Press space until center QtNode is selected again
  m.show();
  QTest::keyClick(&m, Qt::Key_Space);
  while (1)
  {
    qApp->processEvents();
    QTest::qWait(1);
    m.show();
    QTest::keyClick(&m, Qt::Key_Space);
    const auto qtnodes = GetSelectedQtNodes(m.GetScene());
    assert(qtnodes.size() <= 1);
    if (qtnodes.empty()) continue;
    if (IsQtCenterNode(qtnodes[0])) break;
  }
  QKeyEvent event(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
  m.keyPressEvent(&event);
  QVERIFY(!event.isAccepted());
  #endif // FIX_ISSUE_113
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

void ribi::cmap::qtconceptmap_test::relations_should_be_blue_in_edit_mode()
{
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.SetMode(Mode::edit);
  m.show();
  #ifdef CAN_TEST_112
  for (const QtNode * const qtnode: GetQtNodesAlsoOnQtEdge(m.GetScene()))
  {
    if (IsOnEdge(qtnode, m.GetScene()))
    {
      //This test does not work :-(
      QVERIFY(qtnode->brush() == QtBrushFactory().CreateBlueGradientBrush());
    }
  }
  qCritical() << "Tested #112 to be solved";
  QTest::qWait(10000);
  #endif //~ CAN_TEST_112
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

void ribi::cmap::qtconceptmap_test::select_random_node_keyboard()
{
  #ifdef FIX_ISSUE_108
  QtConceptMap m;
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();
  QTest::keyClick(&m, Qt::Key_N, Qt::ControlModifier, 100);
  m.show();

  QVERIFY(GetSelectedQtNodes(m.GetScene()).size() == 2);

  std::vector<int> ids;
  for (int i=0; i!=100; ++i)
  {
    QTest::keyClick(&m, Qt::Key_Space, Qt::NoModifier, 10);
    m.show();
    const auto n_selected = GetSelectedQtNodes(m.GetScene()).size();
    if (n_selected != 1) { qCritical() << n_selected; }
    QVERIFY(n_selected == 1);
    assert(GetSelectedQtNodes(m.GetScene()).size() == 1);
    ids.push_back(GetSelectedQtNodes(m.GetScene())[0]->GetNode().GetId());
  }
  //Test if there are two IDs, both occuring multiple times
  QVERIFY(std::count(std::begin(ids),std::end(ids),ids[0])
    != static_cast<int>(ids.size())
  ); //Good enough?
  #endif //FIX_ISSUE_108
}

void ribi::cmap::qtconceptmap_test::set_concept_maps()
{
  for (const auto concept_map: ConceptMapFactory().GetAllTests())
  {
    QtConceptMap m;
    m.show();
    m.SetConceptMap(concept_map);
    m.show();
    m.SetConceptMap(concept_map);
    m.show();
  }
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
    for (int i=0; i!=n; ++i) {
      m.DoCommand(
        new CommandCreateNewNode(
          m.GetConceptMap(),
          Mode::uninitialized,
          m.GetScene(),
          m.GetQtToolItem(),
          0.0,
          0.0
        )
      );
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
    m.DoCommand(
      new CommandCreateNewEdgeBetweenTwoSelectedNodes(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem()
      )
    );
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
    m.DoCommand(
      new CommandToggleArrowHead(
        m.GetConceptMap(),
        m.GetScene()
      )
    );

    //Postconditions
    QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(qtedge->GetEdge().HasHeadArrow());

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
    for (int i=0; i!=n; ++i) {
      m.DoCommand(
        new CommandCreateNewNode(
          m.GetConceptMap(),
          Mode::uninitialized,
          m.GetScene(),
          m.GetQtToolItem(),
          0.0,
          0.0
        )
      );
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
    m.DoCommand(
      new CommandCreateNewEdgeBetweenTwoSelectedNodes(
        m.GetConceptMap(),
        Mode::uninitialized,
        m.GetScene(),
        m.GetQtToolItem()
      )
    );
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
    m.DoCommand(
      new CommandToggleArrowTail(
        m.GetConceptMap(),
        m.GetScene()
      )
    );
    m.show();
    //Postconditions
    QVERIFY(DoubleCheckEdgesAndNodes(m,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(m,1,0));
    const auto qtedges = GetQtEdges(m.GetScene());
    QVERIFY(qtedges.size() == 1);
    const auto qtedge = qtedges.back();
    QVERIFY(qtedge->GetEdge().HasTailArrow());

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


void ribi::cmap::qtconceptmap_test::all_tests()
{
  using namespace ribi::cmap;
  bool verbose{false};
  //#define FOR_LUCAS_20160130
  #ifdef FOR_LUCAS_20160130
  if (verbose) { TRACE("Delete Edge and Node-that-is-head-of-Edge: delete from QtConceptMap"); }
  {
    QtConceptMap qtconceptmap;
    QTest::keyClick(&d, Qt::Key_N, Qt::ControlModifier, 100);
    QTest::keyClick(&d, Qt::Key_N, Qt::ControlModifier, 100);
    auto ctrl_e = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrl_e);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,1,0));
    std::srand(42);
    //Select only one single QtNode
    //PROBLEM: This is an infinite loop. Why?
    while (CountSelectedQtNodes(qtconceptmap.GetScene()) != 1
      || CountSelectedQtEdges(qtconceptmap.GetScene()) != 0
    ) {
      auto ctrl_space = CreateControlSpace();
      qtconceptmap.keyPressEvent(&ctrl_space);
      std::cerr
        << CountSelectedQtNodes(qtconceptmap.GetScene())
        << ","
        << CountSelectedQtEdges(qtconceptmap.GetScene())
      ;
    }
    QVERIFY(!"You've fixed the problem halfways");
    QVERIFY(CountSelectedQtEdges(qtconceptmap.GetScene()) == 0);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,1,2));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,1));
    //Deleting the QtNode should also delete the QtEdge that is connected to it
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);
    QVERIFY(DoubleCheckEdgesAndNodes(qtconceptmap,0,1));
    QVERIFY(DoubleCheckSelectedEdgesAndNodes(qtconceptmap,0,0));
  }
  QVERIFY(!"You've fixed it!");
  #endif // FOR_LUCAS_20160130
  //#define FIX_ISSUE_1
  #ifdef  FIX_ISSUE_1
  verbose = true;
  if (verbose) { TRACE("FIX_ISSUE_1"); }
  if (verbose) { TRACE("CTRL-N, CTRL-N, CTRL-E, Left: should select one Node"); }
  {
    QtConceptMap qtconceptmap;
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    qtconceptmap.SetConceptMap(conceptmap);
    conceptmap.SetVerbosity(true);
    qtconceptmap.SetVerbosity(true);

    if (verbose) { TRACE("Create two nodes"); }
    auto ctrln = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrln);
    qtconceptmap.keyPressEvent(&ctrln);

    if (verbose) { TRACE("Create an edge, edge is selected"); }
    auto ctrle = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrle);

    QVERIFY(qtconceptmap.GetQtNodes().size() == 2);
    qtconceptmap.GetQtNodes()[0]->SetCenterPos(-100.0,0.0);
    qtconceptmap.GetQtNodes()[0]->setToolTip("QtNodes[0]");
    qtconceptmap.GetQtNodes()[1]->SetCenterPos( 100.0,0.0);
    qtconceptmap.GetQtNodes()[1]->setToolTip("QtNodes[1]");
    qtconceptmap.GetQtEdges()[0]->setToolTip("QtEdges[0]");
    qtconceptmap.GetQtEdges()[0]->GetQtNode()->setToolTip("QtEdges[0] its center QtNode");

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 0);
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodesNotOnEdge().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == 1);
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

    //Fails if item is invisible
    qtconceptmap.showFullScreen();
    for (int i=0; i!=100; ++i) qApp->processEvents();
    QVERIFY(qtconceptmap.scene()->selectedItems().count() > 0);


    if (verbose) { TRACE("Unselect the edge, select the node by pressing an arrow key"); }
    auto up = CreateRight(); //Selects
    TRACE("START");
    conceptmap.SetVerbosity(true);
    qtconceptmap.SetVerbosity(true);
    qtconceptmap.keyPressEvent(&up);

    TRACE(conceptmap.GetSelectedNodes().size());
    TRACE(qtconceptmap.GetSelectedQtNodesNotOnEdge().size());
    TRACE(conceptmap.GetSelectedEdges().size());
    TRACE(qtconceptmap.GetSelectedQtEdges().size());


    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodesNotOnEdge().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 1);
    QVERIFY(conceptmap.GetSelectedEdges().size() == 0);
  }
  if (verbose) { TRACE("Delete Node-that-is-head-of-Edge, then Undo"); }
  {
    QtConceptMap qtconceptmap;
    ConceptMap conceptmap = ribi::cmap::ConceptMapFactory().GetEmptyConceptMap();
    qtconceptmap.SetConceptMap(conceptmap);

    //Create two nodes
    auto ctrln = CreateControlN();
    qtconceptmap.keyPressEvent(&ctrln);
    qtconceptmap.keyPressEvent(&ctrln);

    //Create an edge
    auto ctrle = CreateControlE();
    qtconceptmap.keyPressEvent(&ctrle);

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodesNotOnEdge().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 0);
    QVERIFY(conceptmap.GetSelectedEdges().size() == 1);


    //Select a node
    auto left = CreateLeft();
    qtconceptmap.keyPressEvent(&left);

    TRACE(conceptmap.GetSelectedNodes().size());
    TRACE(qtconceptmap.GetSelectedQtNodesNotOnEdge().size());
    TRACE(conceptmap.GetSelectedEdges().size());
    TRACE(qtconceptmap.GetSelectedQtEdges().size());

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodesNotOnEdge().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 1);
    QVERIFY(conceptmap.GetSelectedEdges().size() == 0);

    //Delete the node, edge will be deleted as well
    auto del = CreateDel();
    qtconceptmap.keyPressEvent(&del);

    //Undo the deletion, should bring back node and edge
    auto cntrlz = CreateControlZ();
    qtconceptmap.keyPressEvent(&cntrlz);

    QVERIFY(boost::num_vertices(conceptmap) == 2);
    QVERIFY(boost::num_vertices(conceptmap) == qtconceptmap.GetQtNodes().size());
    QVERIFY(boost::num_edges(conceptmap) == 1);
    QVERIFY(boost::num_edges(conceptmap) == qtconceptmap.GetQtEdges().size());
    QVERIFY(conceptmap.GetSelectedNodes().size() == 1);
    QVERIFY(conceptmap.GetSelectedNodes().size() == qtconceptmap.GetSelectedQtNodesNotOnEdge().size());
    QVERIFY(conceptmap.GetSelectedEdges().size() == 0);
    QVERIFY(conceptmap.GetSelectedEdges().size() == qtconceptmap.GetSelectedQtEdges().size());

  }
  QVERIFY(!"Fixed issue #1");
  #endif // FIX_ISSUE_1
}
