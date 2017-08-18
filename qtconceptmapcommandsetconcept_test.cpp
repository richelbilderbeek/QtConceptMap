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

void ribi::cmap::QtConceptMapCommandSetConceptTest::SetConceptWithExamplesAtQtNode() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  QVERIFY(!GetQtExamplesItemBuddy(q));

  const Concept concept("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept));

  const Concept concept_again = GetConcept(*GetFirstQtNode(q));
  QVERIFY(concept == concept_again);

  QVERIFY(GetQtExamplesItemBuddy(q));
}

void ribi::cmap::QtConceptMapCommandSetConceptTest
  ::SetConceptWithExamplesAtQtNodeAndUndo() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  QVERIFY(!GetQtExamplesItemBuddy(q));

  const Concept concept("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept));

  assert(GetQtExamplesItemBuddy(q));

  q.Undo();

  QVERIFY(!GetQtExamplesItemBuddy(q));
}




void ribi::cmap::QtConceptMapCommandSetConceptTest
  ::SetConceptWithoutExamplesAtQtNodeWithExamples() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  const Concept concept_with_examples("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept_with_examples));

  QVERIFY(GetQtExamplesItemBuddy(q));

  const Concept concept_without_examples("concept without examples");
  q.DoCommand(new CommandSetConcept(q, concept_without_examples));

  QVERIFY(!GetQtExamplesItemBuddy(q));
}

void ribi::cmap::QtConceptMapCommandSetConceptTest
  ::SetConceptWithoutExamplesAtQtNodeWithExamplesAndUndo() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  const Concept concept_with_examples("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept_with_examples));

  QVERIFY(GetQtExamplesItemBuddy(q));

  const Concept concept_without_examples("concept without examples");
  q.DoCommand(new CommandSetConcept(q, concept_without_examples));
  assert(!GetQtExamplesItemBuddy(q));
  q.Undo();

  QVERIFY(GetQtExamplesItemBuddy(q));
}
