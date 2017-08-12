#include "qtconceptmapcommands_test.h"

#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"
#include "conceptmaphelper.h"

#include <QDebug>

void ribi::cmap::QtConceptMapCommandsTest::get_commands() const noexcept
{
  const std::string cmds = "some commands";
  const std::vector<std::string> args = { "--command", cmds };
  const auto s = ::ribi::cmap::get_commands(args);
  QVERIFY(!s.empty());
  QVERIFY(s == cmds);
}

void ribi::cmap::QtConceptMapCommandsTest::nonsense_to_empty() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q, { "--command", "nonsense" } );
  QVERIFY(cmds.empty());
}

void ribi::cmap::QtConceptMapCommandsTest::create_one_new_node_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q, { "--command", "create_new_node(my text, false, 10, 20)" } );
  QVERIFY(cmds.size() == 1);

  QVERIFY(CountQtNodes(q) == 0);
  q.DoCommand(cmds[0]);
  QVERIFY(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 1);
  CheckInvariants(q);
}

void ribi::cmap::QtConceptMapCommandsTest::create_one_new_node_then_unselect() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(to be unselected, false, 10, 20); "
      "unselect(to be unselected)"
    }
  );
  QVERIFY(cmds.size() == 2);

  QVERIFY(CountQtNodes(q) == 0);
  QVERIFY(CountSelectedQtNodes(q) == 0);

  for (const auto cmd: cmds) q.DoCommand(cmd);

  QVERIFY(CountQtNodes(q) == 1);
  QVERIFY(CountSelectedQtNodes(q) == 0);
}


void ribi::cmap::QtConceptMapCommandsTest::create_relation_over_center_node() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(center, true, 0, 0); "
      "unselect(center); "
      "create_new_node(from, false, 0, -50);"
      "create_new_node(to, false, 0, 50);"
      "create_new_edge(relation, from, to);"
    }
  );
  QVERIFY(cmds.size() == 5);

  for (const auto cmd: cmds) q.DoCommand(cmd);

  QVERIFY(CountQtNodes(q) == 3);
}

void ribi::cmap::QtConceptMapCommandsTest::create_two_new_node_commands() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(my first text, false, 10, 20); "
      "create_new_node(my other text, false, 10, 40)"
    }
  );
  QVERIFY(cmds.size() == 2);
}

void ribi::cmap::QtConceptMapCommandsTest::create_new_edge_between_two_selected_nodes_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(my text, false, 10, 20); "
      "create_new_node(my text, false, 10, 40); "
      "create_new_edge(my text)"
    }
  );
  QVERIFY(cmds.size() == 3);
}

void ribi::cmap::QtConceptMapCommandsTest::load_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "load(my_file.cmp)"
    }
  );
  QVERIFY(cmds.size() == 1);
}

void ribi::cmap::QtConceptMapCommandsTest
  ::move_command_fails_on_nonexisting_item() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(from, false, 100, 200); "
      "create_new_node(to, false, 100, 400); "
      "create_new_edge(relation); "
      "move(nonexisting, 50, 75)"
    }
  );
  assert(cmds.size() == 4);
  assert(CountQtNodes(q) == 0);
  q.DoCommand(cmds[0]);
  q.DoCommand(cmds[1]);
  q.DoCommand(cmds[2]);
  try
  {
    q.DoCommand(cmds[3]);
    QVERIFY(!"Should not get here"); //!OCLINT accepted idiom
  }
  catch (std::exception& )
  {
    QVERIFY("Should get here"); //!OCLINT accepted idiom
  }
}


void ribi::cmap::QtConceptMapCommandsTest::move_command_on_edge() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(from, false, 100, 200); "
      "create_new_node(to, false, 100, 400); "
      "create_new_edge(relation); "
      "move(relation, 50, 75)"
    }
  );
  assert(cmds.size() == 4);
  assert(CountQtNodes(q) == 0);
  q.DoCommand(cmds[0]);
  q.DoCommand(cmds[1]);
  q.DoCommand(cmds[2]);
  assert(CountQtEdges(q) == 1);
  {
    const double expected_x{100.0};
    const double measured_x{GetX(*GetQtEdges(q).at(0))};
    const double expected_y{300.0};
    const double measured_y{GetY(*GetQtEdges(q).at(0))};
    assert(std::abs(expected_x - measured_x) < 1.0);
    assert(std::abs(expected_y - measured_y) < 1.0);
  }
  q.DoCommand(cmds[3]);
  {
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
}

void ribi::cmap::QtConceptMapCommandsTest::move_command_on_node() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(move me, false, 50, 75); "
      "move(move me, 100, 200)"
    }
  );
  QVERIFY(cmds.size() == 2);
  assert(CountQtNodes(q) == 0);
  q.DoCommand(cmds[0]);
  assert(CountQtNodes(q) == 1);
  {
    const double expected_x{50.0};
    const double measured_x{GetX(*GetQtNodes(q).at(0))};
    const double expected_y{75.0};
    const double measured_y{GetY(*GetQtNodes(q).at(0))};
    assert(std::abs(expected_x - measured_x) < 1.0);
    assert(std::abs(expected_y - measured_y) < 1.0);
  }
  q.DoCommand(cmds[1]);
  {
    const double expected_x{150.0};
    const double measured_x{GetX(*GetQtNodes(q).at(0))};
    const double expected_y{275.0};
    const double measured_y{GetY(*GetQtNodes(q).at(0))};
    assert(std::abs(expected_x - measured_x) < 1.0);
    assert(std::abs(expected_y - measured_y) < 1.0);
  }
}

void ribi::cmap::QtConceptMapCommandsTest::save_command_empty_concept_map() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "save(my_file.cmp)"
    }
  );
  QVERIFY(cmds.size() == 1);
}

void ribi::cmap::QtConceptMapCommandsTest::save_command_two_nodes_one_edge() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());

  QtConceptMap q;

  //Create file
  const auto cmds = parse_commands(q,
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
  QVERIFY(cmds.size() == 6);
  assert(!QFile::exists(filename.c_str()));
  for (const auto cmd: cmds) q.DoCommand(cmd);
  assert(QFile::exists(filename.c_str()));
  assert(CountQtNodes(q) == 3);
}

void ribi::cmap::QtConceptMapCommandsTest::save_and_load() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());

  QtConceptMap q;

  //Create file
  {
    const auto cmds = parse_commands(q,
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
    QVERIFY(cmds.size() == 6);
    assert(!QFile::exists(filename.c_str()));
    for (const auto cmd: cmds) q.DoCommand(cmd);
    assert(QFile::exists(filename.c_str()));
    assert(CountQtNodes(q) == 3);
  }
  q.SetConceptMap(ConceptMap());
  CheckInvariants(q);
  //Load file
  {
    const auto cmds = parse_commands(q,
      {
        "--command",
        "load(" + filename + ")"
      }
    );
    QVERIFY(cmds.size() == 1);
    assert(CountQtNodes(q) == 0);
    for (const auto cmd: cmds) q.DoCommand(cmd);
    QVERIFY(CountQtNodes(q) == 3);
  }
}


void ribi::cmap::QtConceptMapCommandsTest::save_and_load_must_result_in_same_topology() const noexcept
{
  const std::string filename{std::string(__func__) + ".cmp"};
  if (QFile::exists(filename.c_str())) QFile::remove(filename.c_str());

  QtConceptMap q;

  //Create file
  {
    const auto cmds = parse_commands(q,
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
    assert(cmds.size() == 7);
    assert(!QFile::exists(filename.c_str()));
    //Creagte concept map with QtEdge
    q.DoCommand(cmds[0]);
    q.DoCommand(cmds[1]);
    q.DoCommand(cmds[2]);
    q.DoCommand(cmds[3]);
    q.DoCommand(cmds[4]);
    {
      //for (const auto cmd: cmds) q.DoCommand(cmd);
      CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(q);
      CheckInvariantQtNodesAndNodesHaveSameCoordinats(q);
      assert(CountQtNodes(q) == 3);
      assert(CountQtEdges(q) == 1);
      const auto qtedges = GetQtEdges(q);
      assert(qtedges.size() == 1);
      const QtEdge * const qtedge = qtedges[0];
      assert(qtedge);
      const double expected_x{140.0};
      const double expected_y{300.0};
      const double measured_x1{GetX(*qtedge)};
      const double measured_y1{GetY(*qtedge)};
      const double measured_x2{GetX(qtedge->GetEdge())};
      const double measured_y2{GetY(qtedge->GetEdge())};
      const double measured_x3{GetX(*qtedge->GetQtNode())};
      const double measured_y3{GetY(*qtedge->GetQtNode())};
      qDebug()
        << "\nGetX(qtedge->GetEdge()): " << GetX(qtedge->GetEdge())
        << "\nGetX(qtedge->GetQtNode()): " << GetX(*qtedge->GetQtNode())
        << "\nGetX(*qtedge): " << GetX(*qtedge)
        << "\nGetText(*qtedge): " << GetText(*qtedge).c_str()
      ;
      assert(std::abs(expected_x - measured_x1) < 2.0);
      assert(std::abs(expected_y - measured_y1) < 2.0);
      assert(std::abs(expected_x - measured_x2) < 2.0);
      assert(std::abs(expected_y - measured_y2) < 2.0);
      assert(std::abs(expected_x - measured_x3) < 2.0);
      assert(std::abs(expected_y - measured_y3) < 2.0);
      assert(GetText(*qtedge) == "relation");
    }
    //Move QtEdge
    q.DoCommand(cmds[5]);
    {
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
    }
    //Save
    assert(!QFile::exists(filename.c_str()));
    assert(GetX(GetFirstEdge(q.GetConceptMap())) > 189.0);
    assert(GetY(GetFirstEdge(q.GetConceptMap())) > 374.0);
    q.DoCommand(cmds[6]);
    assert(QFile::exists(filename.c_str()));
    CoutFile(filename);
  }
  q.SetConceptMap(ConceptMap());
  CheckInvariants(q);
  //Load file
  {
    const auto cmds = parse_commands(q,
      {
        "--command",
        "load(" + filename + ")"
      }
    );
    assert(cmds.size() == 1);
    assert(CountQtNodes(q) == 0);
    for (const auto cmd: cmds) q.DoCommand(cmd);
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
}
void ribi::cmap::QtConceptMapCommandsTest::select_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "select(absent node)"
    }
  );
  QVERIFY(cmds.size() == 1);
}

void ribi::cmap::QtConceptMapCommandsTest::set_mode_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "set_mode(edit)"
    }
  );
  QVERIFY(cmds.size() == 1);
}

void ribi::cmap::QtConceptMapCommandsTest::unselect_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "unselect(absent node)"
    }
  );
  QVERIFY(cmds.size() == 1);
}
