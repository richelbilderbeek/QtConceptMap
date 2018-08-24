#ifndef QTCONCEPTMAP_COMMANDS_TEST_H
#define QTCONCEPTMAP_COMMANDS_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandsTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void CreateNewEdgeBetweenTwoSelectedNodesCommand() const noexcept;
  void CreateOneNewNodeCommand() const noexcept;
  void CreateOneNewNodeThenUnselect() const noexcept;
  void CreateRelationOverCenterNode() const noexcept;
  void CreateTwoNewNodeCommands() const noexcept;
  void GetCommandsEmptyWhenOneWord() const noexcept;
  void GetCommandsEmptyWhenTwoWords() const noexcept;
  void GetCommandsEmptyWithManyNonsenseWords() const noexcept;
  void MoveCommandFailsOnNonexistingItem() const noexcept;
  void MoveCommandOnEdge() const noexcept;
  void MoveCommandOnNode() const noexcept;
  void MoveNodeCommandOnNode() const noexcept;
  void NonsenseToEmpty() const noexcept;
  void SelectAndUnselectAllLonelyCenterNode() const noexcept;
  void SelectAndUnselectLonelyCenterNode() const noexcept;
  void SelectCommandIsIgnoredOnAbsentItem() const noexcept;
  void SetModeCommand() const noexcept;
  void ToggleArrowHeadCommand() const noexcept;
  void ToggleArrowTailCommand() const noexcept;
  void UnselectCommandIsIgnoredOnAbsentItem() const noexcept;
  void UnselectLonelyCenterNode() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDS_TEST_H

