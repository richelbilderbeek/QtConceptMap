#include "qtconceptmapcommands_test.h"

#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "qtconceptmaphelper.h"

#include <QDebug>

void ribi::cmap::qtconceptmapcommands_test::get_commands() const noexcept
{
  const std::string cmds = "some commands";
  const std::vector<std::string> args = { "--command", cmds };
  const auto s = ::ribi::cmap::get_commands(args);
  QVERIFY(!s.empty());
  QVERIFY(s == cmds);

}

void ribi::cmap::qtconceptmapcommands_test::nonsense_to_empty() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q, { "--command", "nonsense" } );
  QVERIFY(cmds.empty());
}

void ribi::cmap::qtconceptmapcommands_test::create_one_new_node_command() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::create_one_new_node_then_unselect() const noexcept
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


void ribi::cmap::qtconceptmapcommands_test::create_relation_over_center_node() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::create_two_new_node_commands() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::create_new_edge_between_two_selected_nodes_command() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::create_new_edge_between_two_nodes_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(A, false, 10, 20); "
      "create_new_node(B, false, 10, 20); "
      "create_new_node(C, false, 10, 40); "
      "create_new_edge(my text, A, C)"
    }
  );
  QVERIFY(cmds.size() == 4);
}

void ribi::cmap::qtconceptmapcommands_test::load_command() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::save_command_empty_concept_map() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::save_command_two_nodes_one_edge() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::save_and_load() const noexcept
{
  //#define FIX_ISSUE_137
  #ifdef FIX_ISSUE_137
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
  #endif // FIX_ISSUE_137
}

void ribi::cmap::qtconceptmapcommands_test::select_command() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::set_mode_command() const noexcept
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

void ribi::cmap::qtconceptmapcommands_test::unselect_command() const noexcept
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
