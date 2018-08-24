#ifndef RIBI_CMAP_QTCONCEPTMAP_TEST_H
#define RIBI_CMAP_QTCONCEPTMAP_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review
  void PressF1OnSingleNodeEditConceptMapIsRejected() const noexcept;
  void PressF1OnSingleNodeRateConceptMapIsAccepted() const noexcept;
  void PressF1OnSingleNodeUninitializedConceptMapIsRejected() const noexcept;

  //Done
  void CannotDeleteCenterNode() const noexcept; //#114
  void CannotEditCenterNode() const noexcept; //#104
  void CannotMoveCenterNode() const noexcept; //#115
  void ChangeModes() const noexcept;
  void ClickOnNothingShouldUnselectAll() const noexcept;
  void CreateOneEdgeCommandAndCheckZorder() const noexcept;
  void CreateOneEdgeCommand() const noexcept;
  void CreateOneEdgeKeyboard() const noexcept;
  void CreateOneEdgeKeyboardIncorrectly() const noexcept;
  void CreateOneEdgeKeyboardAndUndo() const noexcept;
  void CreateOneEdgeWithHeadAndToggleKeyboard() const noexcept;
  void CreateOneEdgeWithHeadAndUndoKeyboard() const noexcept;
  void CreateOneEdgeWithHeadCommand() const noexcept;
  void CreateOneEdgeWithHeadKeyboard() const noexcept;
  void CreateOneEdgeWithTailCommand() const noexcept;
  void CreateOneEdgeWithTailKeyboard() const noexcept;
  //void create_one_edge_with_tail_and_undo_keyboard() const;
  //void create_one_edge_with_tail_and_toggle_keyboard() const;
  void CreateOneNodeAndUndoCommand() const noexcept;
  void CreateOneNodeAndUndoKeyboard() const noexcept;
  void CreateOneNodeCommand() const noexcept;
  void CreateOneNodeKeyboard() const noexcept;
  void CreateOneNodeMouse() const noexcept;
  void CreateTenNodesAndUndoCommand() const noexcept;
  void CreateTenNodesAndUndoKeyboard() const noexcept;
  void CreateTwoNodesCommand() const noexcept;
  void CreateTwoNodesKeyboard() const noexcept;
  void DefaultConstruction() const noexcept;
  void DeleteNodeThatIsConnectedToMultipleEdgesKeyboard() const noexcept;
  void DeleteNodeThatIsHeadOfEdgeKeyboard() const noexcept;
  void DeleteNodeThatIsHeadOfEdgeAndUndoKeyboard() const noexcept;
  void DeleteNodeThatIsTailOfEdgeKeyboard() const noexcept;
  void DeleteNodesThatAreHeadAndTailOfEdgeKeyboard() const noexcept;
  void DeleteOneEdgeByNodeCommand() const noexcept;
  void DeleteOneEdgeByNodeKeyboard() const noexcept;
  void DeleteOneEdgeCommand() const noexcept;
  void DeleteOneEdgeKeyboard() const noexcept;
  void DeleteOneNodeCommand() const noexcept;
  void DeleteOneNodeCommandAndUndo() const noexcept;
  void DeleteOneNodeKeyboard() const noexcept;
  void DeleteTwoNodesCommand() const noexcept;
  void DeleteTwoNodesKeyboard() const noexcept;
  void DoubleClick() const noexcept;
  void DoubleClickTwice() const noexcept;
  void EditModeFlags() const noexcept;
  void GetHighlighter() const noexcept;
  void GetHighlighterConst() const noexcept;
  void GetItemBelowCursor() const noexcept;
  void GetQtNodesAlsoOnQtEdge() const noexcept;
  void HasSelectedItems() const noexcept;
  void IsCommandPutOnUndoStack() const noexcept;
  void MouseWheel() const noexcept;
  void NcenterNodesAndQtCenterNodesMustMatchEdit() const noexcept; //#113
  void NcenterNodesAndQtCenterNodesMustMatchRate() const noexcept; //#113
  void NcenterNodesAndQtCenterNodesMustMatchUninitialized() const noexcept; //#113
  void Press1TogglesTail() const noexcept;
  void Press2TogglesHead() const noexcept;
  void PressCtrlHTogglesHead() const noexcept;
  void PressCtrlRightMovesNonCentralNode() const noexcept;
  void PressCtrlTTogglesTail() const noexcept;
  void PressEscapeMustBeIgnored() const noexcept;
  void PressF1OnCenterNodeIsRejected() const noexcept;
  void PressF1OnEmptyConceptMapIsRejected() const noexcept;
  void PressF1OnMultipleSelectedQtNodesIsRejected() const noexcept;
  void PressF2CanEditNonFocalQuestion() const noexcept;
  void PressF2CannotEditFocalQuestion() const noexcept;
  void PressF2OnCenterNodeIsRejected() const noexcept;
  void PressF2OnEmptyConceptMapIsRejected() const noexcept;
  void PressF2OnMultipleSelectedQtNodesIsRejected() const noexcept;
  void PressF4IsRejected() const noexcept;
  void PressN() const noexcept;
  void PressQuestionMark() const noexcept;
  void PressRightSelectsEdgeExclusively() const noexcept;
  void PressRightSelectsNodeExclusively() const noexcept;
  void PressShiftRightSelectsEdgeAdditively() const noexcept;
  void PressShiftRightSelectsNodeAdditively() const noexcept;
  void PressSpaceOnEmptyConceptMapIsRejected() const noexcept;
  void PressZ() const noexcept;
  void QtNodeInCenterMustBeGold() const noexcept; //#111
  void RateModeFlags() const noexcept;
  void SelectRandomNodeKeyboardEdit() const noexcept; //#138, skip on Travis
  void SetConceptMap4() const noexcept;
  void SetConceptMaps() const noexcept;

  ///Set an empty ConceptMap in a QtConcepytMap
  ///when one QtEdge is selected
  void SetEmptyConceptMapOneSelectedEdge() const noexcept;

  ///Set an empty ConceptMap in a QtConcepytMap
  ///when one QtNode is selected
  void SetEmptyConceptMapOneSelectedNode() const noexcept;

  ///Set an empty ConceptMap in a QtConcepytMap
  ///when two QtNodes are selected, one of which on an edge
  void SetEmptyConceptMapTwoSelectedNodes() const noexcept;

  ///When setting a ConceptMap, the number of Edges and QtEdges,
  /// and Nodes and QtNodes must match
  void SettingConceptMapsEdgesQtEdgesNodesQtNodesMustMatch() const noexcept;

  void SetMode() const noexcept;

  void SingleClickOnEmptyConceptMap() const noexcept;
  void SingleClickOnEmptyConceptMapIsNotAccepted() const noexcept;
  void SingleClickOnNodeIsAccepted() const noexcept;
  void SingleClickOnNodeSelectsNode() const noexcept;
  void TwoClicksOnEdgeSelectsAndUnselectsIt() const noexcept;
  void TwoClicksOnNodeSelectsAndUnselectsIt() const noexcept;
  void UninitializedModeFlags() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // RIBI_CMAP_QTCONCEPTMAP_TEST_H
