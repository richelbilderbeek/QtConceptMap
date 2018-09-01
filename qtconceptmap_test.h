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
  void ClickLmbOnEmptyConceptMapIsNotAcceptedInEditMode() const noexcept;
  void ClickLmbOnEmptyConceptMapIsNotAcceptedInRateMode() const noexcept;
  void ClickLmbOnEmptyConceptMapIsNotAcceptedInUninitializedMode() const noexcept;

  //Done
  void CannotDeleteCenterNode() const noexcept; //#114
  void CannotEditCenterNode() const noexcept; //#104
  void CannotMoveCenterNode() const noexcept; //#115
  void ChangeModes() const noexcept;
  void ClickLmbSelectsNormalNodeInEditMode() const noexcept;
  void ClickLmbSelectsNormalNodeInRateMode() const noexcept;
  void ClickLmbSelectsNothingInUninitializedMode() const noexcept;
  void ClickLmbSelectsSecondNode() const noexcept;
  void ClickOnNothingShouldUnselectAll() const noexcept;
  void ClickShiftLmbSelectsAdditivelyOnFirstNode() const noexcept;
  void ClickShiftLmbSelectsAdditivelyOnSecondNode() const noexcept;
  void CreateOneEdgeCommandAndCheckZorder() const noexcept;
  void CreateOneEdgeCommand() const noexcept;
  void CreateOneEdgeKeyboardAndUndo() const noexcept;
  void CreateOneEdgeKeyboard() const noexcept;
  void CreateOneEdgeKeyboardIncorrectly() const noexcept;
  void CreateOneEdgeWithHeadAndToggleKeyboard() const noexcept;
  void CreateOneEdgeWithHeadAndUndoKeyboard() const noexcept;
  void CreateOneEdgeWithHeadCommand() const noexcept;
  void CreateOneEdgeWithHeadKeyboard() const noexcept;
  void CreateOneEdgeWithTailCommand() const noexcept;
  void CreateOneEdgeWithTailKeyboard() const noexcept;
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
  void DeleteNodesThatAreHeadAndTailOfEdgeKeyboard() const noexcept;
  void DeleteNodeThatIsConnectedToMultipleEdgesKeyboard() const noexcept;
  void DeleteNodeThatIsHeadOfEdgeAndUndoKeyboard() const noexcept;
  void DeleteNodeThatIsHeadOfEdgeKeyboard() const noexcept;
  void DeleteNodeThatIsTailOfEdgeKeyboard() const noexcept;
  void DeleteOneEdgeByNodeCommand() const noexcept;
  void DeleteOneEdgeByNodeKeyboard() const noexcept;
  void DeleteOneEdgeCommand() const noexcept;
  void DeleteOneEdgeKeyboard() const noexcept;
  void DeleteOneNodeCommandAndUndo() const noexcept;
  void DeleteOneNodeCommand() const noexcept;
  void DeleteTwoNodesCommand() const noexcept;
  void DeleteTwoNodesKeyboard() const noexcept;
  void DoubleClick() const noexcept;
  void DoubleClickTwice() const noexcept;
  void EditModeFlags() const noexcept;
  void GetHighlighterConst() const noexcept;
  void GetHighlighter() const noexcept;
  void GetItemBelowCursor() const noexcept;
  void GetQtNodesAlsoOnQtEdge() const noexcept;
  void HasSelectedItems() const noexcept;
  void IsCommandPutOnUndoStack() const noexcept;
  void MouseWheel() const noexcept;
  void NcenterNodesAndQtCenterNodesMustMatchEdit() const noexcept; //#113
  void NcenterNodesAndQtCenterNodesMustMatchRate() const noexcept; //#113
  void NcenterNodesAndQtCenterNodesMustMatchUninitialized() const noexcept; //#113
  void Press1DoesNothingWithoutSelectedEdge() const noexcept;
  void Press1TogglesTail() const noexcept;
  void Press2DoesNothingWithoutSelectedEdge() const noexcept;
  void Press2TogglesHead() const noexcept;
  void PressAltF1IsRejected() const noexcept;
  void PressAltF2IsRejected() const noexcept;
  void PressCtrlDeleteIsIgnored() const noexcept; // https://github.com/richelbilderbeek/BrainWeaver/issues/289
  void PressCtrlDownMovesSelectedQtNodeDown() const noexcept;
  void PressCtrlF1IsRejected() const noexcept;
  void PressCtrlF2IsRejected() const noexcept;
  void PressCtrlHTogglesHead() const noexcept;
  void PressCtrlLeftMovesSelectedQtNodeLeft() const noexcept;
  void PressCtrlNCreatesNewNodeInEditMode() const noexcept;
  void PressCtrlNIsIgnoredInRateMode() const noexcept;
  void PressCtrlNIsIgnoredInUninitializedMode() const noexcept;
  void PressCtrlRightMovesNonCentralNode() const noexcept;
  void PressCtrlRightMovesSelectedQtNodeRight() const noexcept;
  void PressCtrlTTogglesTail() const noexcept;
  void PressCtrlUpMovesSelectedQtNodeUp() const noexcept;
  void PressDelDeletesNodeInEditMode() const noexcept;
  void PressDelOnNormalNodeIsIgnoredInRateMode() const noexcept;
  void PressDelOnNormalNodeIsIgnoredInUninitializedMode() const noexcept;
  void PressEscapeMustBeIgnored() const noexcept;
  void PressF1OnCenterNodeIsRejected() const noexcept;
  void PressF1OnEmptyConceptMapIsRejected() const noexcept;
  void PressF1OnMultipleSelectedQtNodesIsRejected() const noexcept;
  void PressF1OnSingleNodeEditConceptMapIsRejected() const noexcept;
  void PressF1OnSingleNodeRateConceptMapIsAccepted() const noexcept;
  void PressF1OnSingleNodeUninitializedConceptMapIsRejected() const noexcept;
  void PressF1RateNormalQtNodeAndCancelDoesNotChangeConcept() const noexcept;
  void PressF1RateNormalQtNodeAndOkChangesConcept() const noexcept;
  void PressF2CannotEditFocalQuestion() const noexcept;
  void PressF2EditNormalQtNodeAndCancelDoesNotChangeConcept() const noexcept;
  void PressF2EditNormalQtNodeAndOkChangesConcept() const noexcept;
  void PressF2OnCenterNodeIsRejected() const noexcept;
  void PressF2OnEmptyConceptMapIsRejected() const noexcept;
  void PressF2OnMultipleSelectedQtNodesIsRejected() const noexcept;
  void PressF2RateNormalQtNodeAndCancelDoesNotChangeConcept() const noexcept;
  void PressF2RateNormalQtNodeAndOkChangesConcept() const noexcept;
  void PressF2ToEditNormalQtNodeIsAccepted() const noexcept;
  void PressF4IsRejected() const noexcept;
  void PressQuestionMark() const noexcept;
  void PressRightSelectsEdgeExclusively() const noexcept;
  void PressRightSelectsNodeExclusively() const noexcept;
  void PressShiftRightSelectsEdgeAdditively() const noexcept;
  void PressShiftRightSelectsNodeAdditivelyInEditMode() const noexcept;
  void PressShiftRightSelectsNodeExclusivelyInRateMode() const noexcept;
  void PressSpaceOnEmptyConceptMapIsRejected() const noexcept;
  void PressZ() const noexcept;
  void QtNodeInCenterMustBeGold() const noexcept; //#111
  void RateModeFlags() const noexcept;
  void SelectRandomNodeKeyboardEdit() const noexcept; //#138, skip on Travis
  void SetConceptMap4() const noexcept;
  void SetConceptMaps() const noexcept;
  void SetEmptyConceptMapOneSelectedEdge() const noexcept;
  void SetEmptyConceptMapOneSelectedNode() const noexcept;
  void SetEmptyConceptMapTwoSelectedNodes() const noexcept;
  void SetMode() const noexcept;
  void SettingConceptMapsEdgesQtEdgesNodesQtNodesMustMatch() const noexcept;
  void SingleClickOnEmptyConceptMap() const noexcept;
  void SingleClickOnNodeSelectsNode() const noexcept;
  void TwoClicksOnEdgeSelectsAndUnselectsIt() const noexcept;
  void TwoClicksOnNodeSelectsAndUnselectsIt() const noexcept;
  void UninitializedModeFlags() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // RIBI_CMAP_QTCONCEPTMAP_TEST_H
