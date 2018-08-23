#include "qtconceptmapcommandsetconcept_test.h"

#include <iostream>
#include <QDebug>
#include "conceptmapconcept.h"

#include "qtconceptmapcommandsetconcept.h"
#include "qtconceptmap.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "conceptmapfactory.h"

void ribi::cmap::QtCommandSetConceptTest::FailsOnCenterNode() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyQtCenterNode());
  q.show();
  assert(CountSelectedQtNodes(q) == 0);
  try
  {
    q.DoCommand(new CommandSetConcept(q, Concept()));
    assert(!"Should not get here");
  }
  catch (const std::exception&)
  {
    QVERIFY("Should get here");
  }
}

void ribi::cmap::QtCommandSetConceptTest::FailsWithoutSelected() const noexcept
{
  QtConceptMap q;
  q.SetConceptMap(ConceptMapFactory().GetLonelyQtCenterNode());
  SetSelectedness(true, *GetFirstQtNode(q));
  q.show();
  assert(CountSelectedQtNodes(q) == 1);
  try
  {
    q.DoCommand(new CommandSetConcept(q, Concept()));
    assert(!"Should not get here");
  }
  catch (const std::exception&)
  {
    QVERIFY("Should get here");
  }
}

void ribi::cmap::QtCommandSetConceptTest::Parse() const noexcept
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

void ribi::cmap::QtCommandSetConceptTest::ParseNonsenseFails() const noexcept
{
  QtConceptMap q;
  QVERIFY(ParseCommandSetConcept(q, "nonsense") == nullptr);
}

void ribi::cmap::QtCommandSetConceptTest
  ::SetConceptWithExamplesAtQtEdge() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewEdge(q));
  assert(IsSelected(*GetFirstQtEdge(q)));

  const Concept concept("any name", Examples( { Example("John"), Example("Jane")} ));

  q.DoCommand(new CommandSetConcept(q, concept));

  const Concept concept_again = GetConcept(*GetFirstQtEdge(q));

  QVERIFY(concept == concept_again);
}

void ribi::cmap::QtCommandSetConceptTest
  ::SetConceptWithExamplesAtQtEdgeAndUndo() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewNode(q));
  q.DoCommand(new CommandCreateNewEdge(q));
  assert(IsSelected(*GetFirstQtEdge(q)));

  const Concept concept("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept));
  assert(concept == GetConcept(*GetFirstQtEdge(q)));
  q.Undo();
  const Concept concept_again = GetConcept(*GetFirstQtEdge(q));

  QVERIFY(concept != concept_again);
}

void ribi::cmap::QtCommandSetConceptTest
  ::SetConceptWithExamplesAtQtNode() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  const Concept concept("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept));

  const Concept concept_again = GetConcept(*GetFirstQtNode(q));
  QVERIFY(concept == concept_again);
}

void ribi::cmap::QtCommandSetConceptTest
  ::SetConceptWithExamplesAtQtNodeAndUndo() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  const Concept concept("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept));

  q.Undo();
}




void ribi::cmap::QtCommandSetConceptTest
  ::SetConceptWithoutExamplesAtQtNodeWithExamples() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  const Concept concept_with_examples("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept_with_examples));

  const Concept concept_without_examples("concept without examples");
  q.DoCommand(new CommandSetConcept(q, concept_without_examples));
}

void ribi::cmap::QtCommandSetConceptTest
  ::SetConceptWithoutExamplesAtQtNodeWithExamplesAndUndo() const noexcept
{
  QtConceptMap q;
  q.DoCommand(new CommandCreateNewNode(q));
  assert(IsSelected(*GetFirstQtNode(q)));

  const Concept concept_with_examples("any name", Examples( { Example("John"), Example("Jane")} ));
  q.DoCommand(new CommandSetConcept(q, concept_with_examples));

  const Concept concept_without_examples("concept without examples");
  q.DoCommand(new CommandSetConcept(q, concept_without_examples));
  q.Undo();
}
