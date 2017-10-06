#include "qtconceptmapcommands_test.h"

#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "conceptmaphelper.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

#include <QDebug>

void ribi::cmap::QtConceptMapCommandsTest::GetCommands() const noexcept
{
  const std::string cmds = "some commands";
  const std::vector<std::string> args = { "--command", cmds };
  const auto s = ::ribi::cmap::GetCommands(args);
  QVERIFY(!s.empty());
  QVERIFY(s == cmds);
}

void ribi::cmap::QtConceptMapCommandsTest::NonsenseToEmpty() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q, { "--command", "nonsense" } );
  QVERIFY(q.GetUndo().count() == 0);
}

void ribi::cmap::QtConceptMapCommandsTest::CreateOneNewNodeCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q, { "--command", "create_new_node(my text, false, 10, 20)" } );
  QVERIFY(q.GetUndo().count() == 1);
  QVERIFY(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 1);
  CheckInvariants(q);
}

void ribi::cmap::QtConceptMapCommandsTest::CreateOneNewNodeThenUnselect() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(to be unselected, false, 10, 20); "
      "unselect(to be unselected)"
    }
  );
  QVERIFY(q.GetUndo().count() == 2);
  QVERIFY(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}


void ribi::cmap::QtConceptMapCommandsTest::CreateRelationOverCenterNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(center, true, 0, 0); "
      "unselect(center); "
      "create_new_node(from, false, 0, -50);"
      "create_new_node(to, false, 0, 50);"
      "create_new_edge(relation, from, to);"
    }
  );
  QVERIFY(q.GetUndo().count() == 5);
  QVERIFY(CountQtNodes(q) == 3);
}

void ribi::cmap::QtConceptMapCommandsTest::CreateTwoNewNodeCommands() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(my first text, false, 10, 20); "
      "create_new_node(my other text, false, 10, 40)"
    }
  );
  QVERIFY(q.GetUndo().count() == 2);
}

void ribi::cmap::QtConceptMapCommandsTest::CreateNewEdgeBetweenTwoSelectedNodesCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, false, 10, 20); "
      "create_new_node(to, false, 10, 40); "
      "create_new_edge(between)"
    }
  );
  QVERIFY(q.GetUndo().count() == 3);
}

void ribi::cmap::QtConceptMapCommandsTest::LoadCommand() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) { QFile::remove(filename.c_str()); }

  QtConceptMap q;
  SaveToFile(q, filename);

  assert(QFile::exists(filename.c_str()));

  ProcessCommands(q,
    {
      "--command",
      "load(" + filename + ")"
    }
  );
  QVERIFY(q.GetUndo().count() == 1);

  if (QFile::exists(filename.c_str())) { QFile::remove(filename.c_str()); }
}

void ribi::cmap::QtConceptMapCommandsTest
  ::MoveCommandFailsOnNonexistingItem() const noexcept
{
  QtConceptMap q;
  try
  {
    ProcessCommands(q,
      {
        "--command",
        "create_new_node(from, false, 100, 200); "
        "create_new_node(to, false, 100, 400); "
        "create_new_edge(relation); "
        "move(nonexisting, 50, 75)"
      }
    );
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception&)
  {
    QVERIFY("Should get here"); //!OCLINT accepted idiom
  }
}


void ribi::cmap::QtConceptMapCommandsTest::MoveCommandOnEdge() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, false, 100, 200); "
      "create_new_node(to, false, 100, 400); "
      "create_new_edge(relation); "
      "move(relation, 50, 75)"
    }
  );
  assert(q.GetUndo().count() == 4);
  const double expected_x{150.0};
  const double measured_x1{GetX(*GetQtEdges(q).at(0))};
  const double measured_x2{GetX(*GetQtEdges(q).at(0)->GetQtNode())};
  const double measured_x3{GetX(GetQtEdges(q).at(0)->GetEdge())};
  const double expected_y{375.0};
  const double measured_y1{GetY(*GetQtEdges(q).at(0))};
  const double measured_y2{GetY(*GetQtEdges(q).at(0)->GetQtNode())};
  const double measured_y3{GetY(GetQtEdges(q).at(0)->GetEdge())};
  assert(std::abs(expected_x - measured_x1) < 1.0);
  assert(std::abs(expected_y - measured_y1) < 1.0);
  assert(std::abs(expected_x - measured_x2) < 1.0);
  assert(std::abs(expected_y - measured_y2) < 1.0);
  assert(std::abs(expected_x - measured_x3) < 1.0);
  assert(std::abs(expected_y - measured_y3) < 1.0);
}

void ribi::cmap::QtConceptMapCommandsTest::MoveCommandOnNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(move me, false, 50, 75); "
      "move(move me, 100, 200)"
    }
  );
  QVERIFY(q.GetUndo().count() == 2);
  const double expected_x{150.0};
  const double measured_x{GetX(*GetQtNodes(q).at(0))};
  const double expected_y{275.0};
  const double measured_y{GetY(*GetQtNodes(q).at(0))};
  QVERIFY(std::abs(expected_x - measured_x) < 1.0);
  QVERIFY(std::abs(expected_y - measured_y) < 1.0);
}

void ribi::cmap::QtConceptMapCommandsTest::MoveNodeCommandOnNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, false, 50, 75); "
      "move_node(from, 100, 200)"
    }
  );
  QVERIFY(q.GetUndo().count() == 2);
  const double expected_x{150.0};
  const double measured_x{GetX(*GetQtNodes(q).at(0))};
  const double expected_y{275.0};
  const double measured_y{GetY(*GetQtNodes(q).at(0))};
  QVERIFY(std::abs(expected_x - measured_x) < 1.0);
  QVERIFY(std::abs(expected_y - measured_y) < 1.0);
}

void ribi::cmap::QtConceptMapCommandsTest::SaveCommandEmptyConceptMap() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());

  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "save(" + filename + ")"
    }
  );
  QVERIFY(q.GetUndo().count() == 1);
  QVERIFY(QFile::exists(filename.c_str()));

  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());
}

void ribi::cmap::QtConceptMapCommandsTest::SaveCommandTwoNodesOneEdge() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());

  QtConceptMap q;

  //Create file
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(center, true, 0, 0); "
      "unselect(center); "
      "create_new_node(from, false, 0, -50); "
      "create_new_node(to, false, 0, 50); "
      "create_new_edge(relation, from, to); "
      "save(" + filename + ")"
    }
  );
  QVERIFY(q.GetUndo().count() == 6);
  QVERIFY(QFile::exists(filename.c_str()));

  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());
}

void ribi::cmap::QtConceptMapCommandsTest::SaveAndLoad() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());

  QtConceptMap q;

  //Create file
  {
    ProcessCommands(q,
      {
        "--command",
        "create_new_node(center, true, 0, 0); "
        "unselect(center); "
        "create_new_node(from, false, 0, -50); "
        "create_new_node(to, false, 0, 50); "
        "create_new_edge(relation, from, to); "
        "save(" + filename + ")"
      }
    );
    QVERIFY(q.GetUndo().count() == 6);
    assert(QFile::exists(filename.c_str()));
    assert(CountQtNodes(q) == 3);
  }
  q.SetConceptMap(ConceptMap());
  CheckInvariants(q);
  //Load file
  {
    ProcessCommands(q,
      {
        "--command",
        "load(" + filename + ")"
      }
    );
    QVERIFY(CountQtNodes(q) == 3);
  }
}


void ribi::cmap::QtConceptMapCommandsTest::SaveAndLoadMustResultInSameTopology() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());

  QtConceptMap q;

  //Create file
  {
    ProcessCommands(q,
      {
        "--command",
        "create_new_node(center, true, 0, 0); "
        "unselect(center); "
        "create_new_node(from, false, 100, 200); "
        "create_new_node(to, false, 180, 400); "
        "create_new_edge(relation); "
        "move(relation, 50, 75); "
        "save(" + filename + ")"
      }
    );
    assert(q.GetUndo().count() == 7);
    CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(q);
    CheckInvariantQtNodesAndNodesHaveSameCoordinats(q);
    assert(CountQtNodes(q) == 3);
    assert(CountQtEdges(q) == 1);
    const auto qtedges = GetQtEdges(q);
    assert(qtedges.size() == 1);
    const QtEdge * const qtedge = qtedges[0];
    assert(qtedge);
    const double expected_x{140.0 + 50.0};
    const double expected_y{300.0 + 75.0};
    const double measured_x1{GetX(*qtedge)};
    const double measured_y1{GetY(*qtedge)};
    const double measured_x2{GetX(qtedge->GetEdge())};
    const double measured_y2{GetY(qtedge->GetEdge())};
    const double measured_x3{GetX(*qtedge->GetQtNode())};
    const double measured_y3{GetY(*qtedge->GetQtNode())};
    const double measured_x4{GetX(GetFirstEdge(q.GetConceptMap()))};
    const double measured_y4{GetY(GetFirstEdge(q.GetConceptMap()))};
    assert(std::abs(expected_x - measured_x1) < 2.0);
    assert(std::abs(expected_y - measured_y1) < 2.0);
    assert(std::abs(expected_x - measured_x2) < 2.0);
    assert(std::abs(expected_y - measured_y2) < 2.0);
    assert(std::abs(expected_x - measured_x3) < 2.0);
    assert(std::abs(expected_y - measured_y3) < 2.0);
    assert(std::abs(expected_x - measured_x4) < 2.0);
    assert(std::abs(expected_y - measured_y4) < 2.0);
    assert(QFile::exists(filename.c_str()));
  }
  q.SetConceptMap(ConceptMap());
  assert(CountQtNodes(q) == 0);
  CheckInvariants(q);
  //Load file
  {
    ProcessCommands(q,
      {
        "--command",
        "load(" + filename + ")"
      }
    );
    CheckInvariants(q);
    assert(CountQtNodes(q) == 3);
    assert(CountQtEdges(q) == 1);
    const auto qtedges = GetQtEdges(q);
    assert(qtedges.size() == 1);
    const QtEdge * const qtedge = qtedges[0];
    assert(qtedge);
    const double expected_x{140.0 + 50.0};
    const double expected_y{300.0 + 75.0};
    const double measured_x{GetX(*qtedge)};
    const double measured_y{GetY(*qtedge)};
    QVERIFY(std::abs(expected_x - measured_x) < 2.0);
    QVERIFY(std::abs(expected_y - measured_y) < 2.0);
  }
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());
}

void ribi::cmap::QtConceptMapCommandsTest::SelectCommandIsIgnoredOnAbsentItem() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "select(absent node)"
    }
  );
  assert(q.GetUndo().count() == 0);
}

void ribi::cmap::QtConceptMapCommandsTest::SelectAndUnselectAllLonelyCenterNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit); create_new_node(center, true, 0, 0); unselect_all()"
    }
  );
  assert(q.GetUndo().count() == 3);
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
  assert(DoubleCheckSelectedEdgesAndNodes(q, 0, 0));
}

void ribi::cmap::QtConceptMapCommandsTest::SelectAndUnselectLonelyCenterNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit); create_new_node(center, true, 0, 0); unselect(center)"
    }
  );

  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
  assert(DoubleCheckSelectedEdgesAndNodes(q, 0, 0));
  q.showFullScreen();
  QtNode * const qtnode = GetFirstQtNode(q);
  assert(qtnode->GetContourPen().style() == Qt::SolidLine );
  assert(qtnode->GetFocusPen().style() == Qt::DashLine);
  assert(qtnode->pen().style() == Qt::SolidLine);
  /* WIP
  q.update();
  while (1)
  {
    qApp->processEvents();
  }
  assert(!"FIXED");
  */
}



void ribi::cmap::QtConceptMapCommandsTest::SetModeCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit)"
    }
  );
  QVERIFY(q.GetUndo().count() == 1);
}

void ribi::cmap::QtConceptMapCommandsTest::ToggleArrowHeadCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, false, 10, 20); "
      "create_new_node(to, false, 10, 40); "
      "create_new_edge(between); "
      "toggle_arrow_head()"
    }
  );
  QVERIFY(q.GetUndo().count() == 4);
}

void ribi::cmap::QtConceptMapCommandsTest::ToggleArrowTailCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, false, 10, 20); "
      "create_new_node(to, false, 10, 40); "
      "create_new_edge(between); "
      "toggle_arrow_tail()"
    }
  );
  QVERIFY(q.GetUndo().count() == 4);
}


void ribi::cmap::QtConceptMapCommandsTest::UnselectCommandIsIgnoredOnAbsentItem() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "unselect(absent node)"
    }
  );
  assert(q.GetUndo().count() == 0);
}
