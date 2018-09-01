#ifndef QTCONCEPTMAP_COMMANDCREATENEWEDGE_TEST_H
#define QTCONCEPTMAP_COMMANDCREATENEWEDGE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandCreateNewEdgeTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To review
  void CannotCreateNewEdgeWithSelectedEdge() const noexcept;

  //Done
  void CannotCreateNewEdgeBetweenTwoRelations() const noexcept;
  void CannotCreateNewEdgeFromOneRelation() const noexcept;
  void CannotCreateNewEdgeToOneRelation() const noexcept;
  void CannotCreateNewEdgeWithoutSelectedNodes() const noexcept;
  void CreateNewEdge() const noexcept;
  void CreateNewEdgeFromCenterNode() const noexcept;
  void CreateNewEdgeUndo() const noexcept;
  void Parse() const noexcept;
  void ParseNonsenseFails() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDCREATENEWEDGE_TEST_H

