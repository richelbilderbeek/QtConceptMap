#include "qtconceptmapcommandsetmode_test.h"

#include "qtconceptmapcommandsetmode.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmap.h"

#include <QDebug>

void ribi::cmap::QtConceptMapCommandSetModeTest::Parse() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSetMode(q, "nonsense") == nullptr);
  QVERIFY(ParseCommandSetMode(q, "set_mode(nonsense)") == nullptr);
  //Set edit mode
  {
    assert(q.GetMode() != Mode::edit);
    const auto cmd = ParseCommandSetMode(q, "set_mode(edit)");
    q.DoCommand(cmd);
    QVERIFY(q.GetMode() == Mode::edit);
  }
  //Set rate mode
  {
    assert(q.GetMode() != Mode::rate);
    const auto cmd = ParseCommandSetMode(q, "set_mode(rate)");
    q.DoCommand(cmd);
    QVERIFY(q.GetMode() == Mode::rate);
  }
  //Set rate mode
  {
    assert(q.GetMode() != Mode::uninitialized);
    const auto cmd = ParseCommandSetMode(q, "set_mode(uninitialized)");
    q.DoCommand(cmd);
    QVERIFY(q.GetMode() == Mode::uninitialized);
  }
}
