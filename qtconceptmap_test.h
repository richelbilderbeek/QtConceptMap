#ifndef qtconceptmap_test_TEST_H
#define qtconceptmap_test_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //To do/review


  //Done
  void CannotDeleteCenterNode() const; //#114
  void CannotEditCenterNode() const; //#104
  void CannotMoveCenterNode() const; //#115
  void ChangeModes() const;
  void ClickOnNothingShouldUnselectAll() const;
  void CreateOneEdgeCommandAndCheckZorder() const;
  void CreateOneEdgeCommand() const;
  void CreateOneEdgeKeyboard() const;
  void CreateOneEdgeKeyboardAndUndo() const;
  void CreateOneEdgeWithHeadAndToggleKeyboard() const;
  void CreateOneEdgeWithHeadAndUndoKeyboard() const;
  void CreateOneEdgeWithHeadCommand() const;
  void CreateOneEdgeWithHeadKeyboard() const;
  void CreateOneEdgeWithTailCommand() const;
  void CreateOneEdgeWithTailKeyboard() const;
  //void create_one_edge_with_tail_and_undo_keyboard() const;
  //void create_one_edge_with_tail_and_toggle_keyboard() const;
  void create_one_node_and_undo_command() const;
  void create_one_node_and_undo_keyboard() const;
  void create_one_node_command() const;
  void create_one_node_keyboard() const;
  void create_one_node_mouse() const;
  void create_ten_nodes_and_undo_command() const;
  void create_ten_nodes_and_undo_keyboard() const;
  void create_two_nodes_command() const;
  void create_two_nodes_keyboard() const;
  void DefaultConstruction() const;
  void DeleteNodeThatIsConnectedToMultipleEdgesKeyboard() const;
  void DeleteNodeThatIsHeadOfEdgeKeyboard() const;
  void DeleteNodeThatIsHeadOfEdgeAndUndoKeyboard() const;
  void DeleteNodeThatIsTailOfEdgeKeyboard() const;
  void DeleteNodesThatAreHeadAndTailOfEdgeKeyboard() const;
  void delete_one_edge_by_node_command() const;
  void delete_one_edge_by_node_keyboard() const;
  void delete_one_edge_command() const;
  void delete_one_edge_keyboard() const;
  void delete_one_node_command() const;
  void delete_one_node_command_and_undo() const;
  void delete_one_node_keyboard() const;
  void delete_two_nodes_command() const;
  void DeleteTwoNodesKeyboard() const;
  void DoubleClick() const;
  void DoubleClickTwice() const;
  void EditModeFlags() const;
  void GetHighlighter() const;
  void IsCommandPutOnUndoStack() const;
  void MouseWheel() const;
  void NcenterNodesAndQtCenterNodesMustMatchEdit() const; //#113
  void NcenterNodesAndQtCenterNodesMustMatchRate() const; //#113
  void NcenterNodesAndQtCenterNodesMustMatchUninitialized() const; //#113
  void PressCtrlRightMovesNonCentralNode() const;
  void PressEscape() const;
  void PressF2CanEditNonFocalQuestion() const;
  void PressF2CannotEditFocalQuestion() const;
  void PressF1OnEmptyConceptMapIsRejected() const;
  void PressF2OnEmptyConceptMapIsRejected() const;
  void PressF4IsRejected() const;
  void PressH() const;
  void PressN() const;
  void PressQuestionMark() const;
  void PressRightSelectsEdgeExclusively() const;
  void PressRightSelectsNodeExclusively() const;
  void PressShiftRightSelectsEdgeAdditively() const;
  void PressShiftRightSelectsNodeAdditively() const;
  void PressSpaceOnEmptyConceptMapIsRejected() const;
  void PressT() const;
  void PressZ() const;
  void QtNodeInCenterMustBeGold() const; //#111
  void RateModeFlags() const;
  void SelectRandomNodeKeyboardEdit() const; //#138, skip on Travis

  void SetConceptMap4() const;
  void SetConceptMaps() const;

  ///Set an empty ConceptMap in a QtConcepytMap
  ///when one QtEdge is selected
  void SetEmptyConceptMapOneSelectedEdge() const;

  ///Set an empty ConceptMap in a QtConcepytMap
  ///when one QtNode is selected
  void SetEmptyConceptMapOneSelectedNode() const;

  ///Set an empty ConceptMap in a QtConcepytMap
  ///when two QtNodes are selected, one of which on an edge
  void SetEmptyConceptMapTwoSelectedNodes() const;

  ///When setting a ConceptMap, the number of Edges and QtEdges,
  /// and Nodes and QtNodes must match
  void SettingConceptMapsEdgesQtEdgesNodesQtNodesMustMatch() const;

  void SingleClickOnEmptyConceptMap() const;
  void SingleClickOnEmptyConceptMapIsNotAccepted() const;
  void SingleClickOnNodeIsAccepted() const;
  void SingleClickOnNodeSelectsNode() const;
  void TwoClicksOnEdgeSelectsAndUnselectsIt() const;
  void TwoClicksOnNodeSelectsAndUnselectsIt() const;
  void UninitializedModeFlags() const;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmap_test_TEST_H
