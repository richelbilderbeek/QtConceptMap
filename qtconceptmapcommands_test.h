#ifndef QTCONCEPTMAP_COMMANDS_TEST_H
#define QTCONCEPTMAP_COMMANDS_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandsTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //TODO

  //https://github.com/richelbilderbeek/QtConceptMap/issues/5
  void UnselectLonelyCenterNode() const noexcept;

  //https://github.com/richelbilderbeek/Brainweaver/issues/139
  //https://github.com/richelbilderbeek/QtConceptMap/issues/2
  void SelectAndUnselectAllLonelyCenterNode() const noexcept;

  //https://github.com/richelbilderbeek/Brainweaver/issues/139
  void SelectAndUnselectLonelyCenterNode() const noexcept;

  //DONE
  void CreateOneNewNodeCommand() const noexcept;
  void CreateOneNewNodeThenUnselect() const noexcept;
  void CreateRelationOverCenterNode() const noexcept;
  void CreateTwoNewNodeCommands() const noexcept;
  void CreateNewEdgeBetweenTwoSelectedNodesCommand() const noexcept;
  void GetCommandsEmptyWhenOneWord() const noexcept;
  void GetCommandsEmptyWhenTwoWords() const noexcept;
  void GetCommandsEmptyWithManyNonsenseWords() const noexcept;
  void LoadCommand() const noexcept;
  void MoveCommandFailsOnNonexistingItem() const noexcept;
  void MoveCommandOnEdge() const noexcept;
  void MoveCommandOnNode() const noexcept;
  void MoveNodeCommandOnNode() const noexcept;
  void NonsenseToEmpty() const noexcept;
  void SaveAndLoad() const noexcept; //#137
  void SaveAndLoadMustResultInSameTopology() const noexcept;
  void SaveCommandEmptyConceptMap() const noexcept;
  void SaveCommandTwoNodesOneEdge() const noexcept;
  void SelectCommandIsIgnoredOnAbsentItem() const noexcept;

  void SetModeCommand() const noexcept;
  void ToggleArrowHeadCommand() const noexcept;
  void ToggleArrowTailCommand() const noexcept;
  void UnselectCommandIsIgnoredOnAbsentItem() const noexcept;

};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDS_TEST_H

