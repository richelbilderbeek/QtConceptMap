#ifndef QTCONCEPTMAP_COMMANDSETCONCEPT_TEST_H
#define QTCONCEPTMAP_COMMANDSETCONCEPT_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandSetConceptTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  ///Cannot set the concept of a center QtNode
  void FailsOnCenterNode() const noexcept;

  ///Cannot set a concept if there is no QtNode (also on QtEdge) selected
  void FailsWithoutSelected() const noexcept;

  ///Parse from a string
  void Parse() const noexcept;

  ///Parsing a nonsense string should fails
  void ParseNonsenseFails() const noexcept;

  ///Give an empty QtEdge Concept with examples
  void SetConceptWithExamplesAtQtEdge() const noexcept;

  ///Give an empty QtEdge Concept with examples and undo
  void SetConceptWithExamplesAtQtEdgeAndUndo() const noexcept;

  ///Give an empty QtNode a Concept with examples
  void SetConceptWithExamplesAtQtNode() const noexcept;

  ///Give an empty QtNode a Concept with examples and undo
  void SetConceptWithExamplesAtQtNodeAndUndo() const noexcept;

  ///Give an QtNode with examples a Concept without examples
  void SetConceptWithoutExamplesAtQtNodeWithExamples() const noexcept;

  ///Give an QtNode with examples a Concept without examples and undo
  void SetConceptWithoutExamplesAtQtNodeWithExamplesAndUndo() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSETCONCEPT_TEST_H

