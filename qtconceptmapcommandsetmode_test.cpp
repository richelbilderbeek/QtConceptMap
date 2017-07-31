#include "qtconceptmapcommandsetmode_test.h"

#include "qtconceptmapcommandsetmode.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

#include <QDebug>

void ribi::cmap::qtconceptmapcommandsetmode_test::parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(parse_command_set_mode(q, "nonsense") == nullptr);
  QVERIFY(parse_command_set_mode(q, "set_mode(nonsense)") == nullptr);
  //Set edit mode
  {
    assert(q.GetMode() != Mode::edit);
    const auto cmd = parse_command_set_mode(q, "set_mode(edit)");
    q.DoCommand(cmd);
    QVERIFY(q.GetMode() == Mode::edit);
  }
  //Set rate mode
  {
    assert(q.GetMode() != Mode::rate);
    const auto cmd = parse_command_set_mode(q, "set_mode(rate)");
    q.DoCommand(cmd);
    QVERIFY(q.GetMode() == Mode::rate);
  }
  //Set rate mode
  {
    assert(q.GetMode() != Mode::uninitialized);
    const auto cmd = parse_command_set_mode(q, "set_mode(uninitialized)");
    q.DoCommand(cmd);
    QVERIFY(q.GetMode() == Mode::uninitialized);
  }
}
