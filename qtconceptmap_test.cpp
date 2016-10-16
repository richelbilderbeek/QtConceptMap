#include "qtconceptmap_test.h"
#include "qtconceptmap.h"
#include "qtconceptmap.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <QApplication>
#include <QMouseEvent>
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
#include "qtconceptmaphelper.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtnode.h"
//#include "ribi_system.h"

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
  QTest::mouseDClick(&m, Qt::MouseButton::LeftButton, 0, QPoint(0.0,0.0), 100);
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

void ribi::cmap::qtconceptmap_test::get_focusable_items()
{
  //In rate mode, the center node cannot be focused on
  QtConceptMap m;
  m.SetConceptMap(ConceptMapFactory().Get11());
  m.SetMode(Mode::edit);
  assert(CountCenterNodes(m.GetConceptMap()) > 0);
  const auto n_edit = GetFocusableItems(m).size();
  m.SetMode(Mode::rate);
  const auto n_rate = GetFocusableItems(m).size();
  QVERIFY(n_rate < n_edit);
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

void ribi::cmap::qtconceptmap_test::issue_96()
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

void ribi::cmap::qtconceptmap_test::select_random_node_keyboard()
{
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
    QVERIFY(GetSelectedQtNodes(m.GetScene()).size() == 1);
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
