#ifndef QTCONCEPTMAP_COMMANDS_TEST_H
#define QTCONCEPTMAP_COMMANDS_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandsTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //TODO
  void ToggleArrowHeadCommand() const noexcept;
  void ToggleArrowTailCommand() const noexcept;

  //DONE
  void CreateOneNewNodeCommand() const noexcept;
  void CreateOneNewNodeThenUnselect() const noexcept;
  void CreateRelationOverCenterNode() const noexcept;
  void CreateTwoNewNodeCommands() const noexcept;
  void CreateNewEdgeBetweenTwoSelectedNodesCommand() const noexcept;
  void GetCommands() const noexcept;
  void LoadCommand() const noexcept;
  void MoveCommandFailsOnNonexistingItem() const noexcept;
  void MoveCommandOnEdge() const noexcept;
  void MoveCommandOnNode() const noexcept;
  void NonsenseToEmpty() const noexcept;
  void SaveAndLoad() const noexcept; //#137
  void SaveAndLoadMustResultInSameTopology() const noexcept;
  void SaveCommandEmptyConceptMap() const noexcept;
  void SaveCommandTwoNodesOneEdge() const noexcept;
  void SelectCommand() const noexcept;
  void SetModeCommand() const noexcept;
  void UnselectCommand() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDS_TEST_H

