#include "qtconceptmapcommandsetconcept_test.h"

#include "qtconceptmapcommandsetconcept.h"
#include "qtconceptmap.h"
#include "qtconceptmapcommandcreatenewnode.h"

void ribi::cmap::QtConceptMapCommandSetConceptTest::Parse() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));
  const Concept concept("my name");
  const std::string xml = ToXml(concept);
  const std::string cmd = "set_concept(" + xml + ")";
  const auto c = ParseCommandSetConcept(q, cmd);
  assert(c);
  QVERIFY(c != nullptr);
}

void ribi::cmap::QtConceptMapCommandSetConceptTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSetConcept(q, "nonsense") == nullptr);
}
