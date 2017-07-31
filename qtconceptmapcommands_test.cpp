#include "qtconceptmapcommands_test.h"

#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"

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
  const auto cmds = parse_commands(q, { "--command", "create_new_node(10, 20, my text)" } );
  QVERIFY(cmds.size() == 1);
}

void ribi::cmap::qtconceptmapcommands_test::create_two_new_node_commands() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(10, 20, my first text); "
      "create_new_node(10, 40, my other text)"
    }
  );
  QVERIFY(cmds.size() == 2);
}

void ribi::cmap::qtconceptmapcommands_test::create_new_edge_command() const noexcept
{
  QtConceptMap q;
  const auto cmds = parse_commands(q,
    {
      "--command",
      "create_new_node(10, 20, my text); "
      "create_new_node(10, 40, my text); "
      "create_new_edge(my text)"
    }
  );
  QVERIFY(cmds.size() == 3);
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
