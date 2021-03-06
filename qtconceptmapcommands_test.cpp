#include "qtconceptmapcommands_test.h"

#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "conceptmaphelper.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"

#include <QDebug>

void ribi::cmap::QtCommandsTest::GetCommandsEmptyWhenOneWord() const noexcept
{
  const std::string cmds = "single-word";
  const std::vector<std::string> args = { "--command", cmds };
  const auto s = ::ribi::cmap::GetCommands(args);
  QVERIFY(!s.empty());
  QVERIFY(s == cmds);
}

void ribi::cmap::QtCommandsTest::GetCommandsEmptyWhenTwoWords() const noexcept
{
  const std::string cmds = "two words";
  const std::vector<std::string> args = { "--command", cmds };
  const auto s = ::ribi::cmap::GetCommands(args);
  QVERIFY(!s.empty());
  QVERIFY(s == cmds);
}

void ribi::cmap::QtCommandsTest::GetCommandsEmptyWithManyNonsenseWords() const noexcept
{
  const std::string cmds = "many and many nonsense words without minus minus command";
  const std::vector<std::string> args = { "--command", cmds };
  const auto s = ::ribi::cmap::GetCommands(args);
  QVERIFY(!s.empty());
  QVERIFY(s == cmds);
}

void ribi::cmap::QtCommandsTest::NonsenseToEmpty() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q, { "--command", "nonsense" } );
  QVERIFY(q.GetUndo().count() == 0);
}

void ribi::cmap::QtCommandsTest::CreateOneNewNodeCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q, { "--command", "create_new_node(my text, 10, 20, normal)" } );
  QVERIFY(q.GetUndo().count() == 1);
  QVERIFY(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 1);
  CheckInvariants(q);
}

void ribi::cmap::QtCommandsTest::CreateOneNewNodeThenUnselect() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(to be unselected, 10, 20, normal); "
      "unselect(to be unselected)"
    }
  );
  QVERIFY(q.GetUndo().count() == 2);
  QVERIFY(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}


void ribi::cmap::QtCommandsTest::CreateRelationOverCenterNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(focal question, 0, 0, center); "
      "unselect(focal question); "
      "create_new_node(from, 0, -50, normal);"
      "create_new_node(to, 0, 50, normal);"
      "create_new_edge(relation, from, to);"
    }
  );
  QVERIFY(q.GetUndo().count() == 5);
  QVERIFY(CountQtNodes(q) == 3);
}

void ribi::cmap::QtCommandsTest::CreateTwoNewNodeCommands() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(my first text, 10, 20, normal); "
      "create_new_node(my other text, 10, 40, normal)"
    }
  );
  QVERIFY(q.GetUndo().count() == 2);
}

void ribi::cmap::QtCommandsTest::CreateNewEdgeBetweenTwoSelectedNodesCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, 10, 20, normal); "
      "create_new_node(to, 10, 40, normal); "
      "create_new_edge(between)"
    }
  );
  QVERIFY(q.GetUndo().count() == 3);
}

void ribi::cmap::QtCommandsTest
  ::MoveCommandFailsOnNonexistingItem() const noexcept
{
  QtConceptMap q;
  try
  {
    ProcessCommands(q,
      {
        "--command",
        "create_new_node(from, 100, 200, normal); "
        "create_new_node(to, 100, 400, normal); "
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


void ribi::cmap::QtCommandsTest::MoveCommandOnEdge() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, 100, 200, normal); "
      "create_new_node(to, 100, 400, normal); "
      "create_new_edge(relation); "
      "move(relation, 50, 75)"
    }
  );
  assert(q.GetUndo().count() == 4);
  const double expected_x{150.0};
  const double measured_x1{GetX(*GetQtEdges(q).at(0))};
  const double measured_x2{GetX(*GetQtEdges(q).at(0)->GetQtNode())};
  const double expected_y{375.0};
  const double measured_y1{GetY(*GetQtEdges(q).at(0))};
  const double measured_y2{GetY(*GetQtEdges(q).at(0)->GetQtNode())};
  QVERIFY(std::abs(expected_x - measured_x1) < 1.0);
  QVERIFY(std::abs(expected_y - measured_y1) < 1.0);
  QVERIFY(std::abs(expected_x - measured_x2) < 1.0);
  QVERIFY(std::abs(expected_y - measured_y2) < 1.0);
}

void ribi::cmap::QtCommandsTest::MoveCommandOnNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(move me, 50, 75, normal); "
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

void ribi::cmap::QtCommandsTest::MoveNodeCommandOnNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, 50, 75, normal); "
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

void ribi::cmap::QtCommandsTest::SelectCommandIsIgnoredOnAbsentItem() const noexcept
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

void ribi::cmap::QtCommandsTest::SelectAndUnselectAllLonelyCenterNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit); create_new_node(focal question, 0, 0, center); unselect_all()"
    }
  );
  assert(q.GetUndo().count() == 3);
  assert(CountSelectedQtEdges(q) == 0);
  assert(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandsTest::SelectAndUnselectLonelyCenterNode() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit); create_new_node(focal question, 0, 0, center); unselect(focal question)"
    }
  );
  q.show();
  assert(CountQtEdges(q) == 0);
  assert(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

void ribi::cmap::QtCommandsTest::SetModeCommand() const noexcept
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

void ribi::cmap::QtCommandsTest::ToggleArrowHeadCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, 10, 20, normal); "
      "create_new_node(to, 10, 40, normal); "
      "create_new_edge(between); "
      "toggle_arrow_head()"
    }
  );
  QVERIFY(q.GetUndo().count() == 4);
}

void ribi::cmap::QtCommandsTest::ToggleArrowTailCommand() const noexcept
{
  QtConceptMap q;
  ProcessCommands(q,
    {
      "--command",
      "create_new_node(from, 10, 20, normal); "
      "create_new_node(to, 10, 40, normal); "
      "create_new_edge(between); "
      "toggle_arrow_tail()"
    }
  );
  QVERIFY(q.GetUndo().count() == 4);
}


void ribi::cmap::QtCommandsTest::UnselectCommandIsIgnoredOnAbsentItem() const noexcept
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

void ribi::cmap::QtCommandsTest::UnselectLonelyCenterNode() const noexcept
{
  QtConceptMap q;
  q.show();
  ProcessCommands(q,
    {
      "--command",
      "set_mode(edit); create_new_node(my name, 0, 0, center); unselect(my name)"
    }
  );
  assert(q.GetUndo().count() == 3);
  assert(CountQtCenterNodes(q) == 1);
  assert(CountSelectedQtEdges(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}

