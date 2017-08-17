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
  void CannotDeleteCenterNode() const noexcept; //#114
  void CannotEditCenterNode() const noexcept; //#104
  void CannotMoveCenterNode() const noexcept; //#115
  void ChangeModes() const noexcept;
  void ClickOnNothingShouldBeIgnored() const noexcept;
  void ConceptMapMustFitWindow() const noexcept; //#135
  void concept_map_must_fit_window_after_setting() const noexcept;
  void create_one_edge_command_and_check_z_order() const noexcept;
  void create_one_edge_command() const noexcept;
  void create_one_edge_keyboard() const noexcept;
  void create_one_edge_keyboard_and_undo() const noexcept;
  void create_one_edge_with_head_and_toggle_keyboard() const noexcept;
  void create_one_edge_with_head_and_undo_keyboard() const noexcept;
  void create_one_edge_with_head_command() const noexcept;
  void create_one_edge_with_head_keyboard() const noexcept;
  void create_one_edge_with_tail_command() const noexcept;
  void create_one_edge_with_tail_keyboard() const noexcept;
  //void create_one_edge_with_tail_and_undo_keyboard() const noexcept;
  //void create_one_edge_with_tail_and_toggle_keyboard() const noexcept;
  void create_one_node_and_undo_command() const noexcept;
  void create_one_node_and_undo_keyboard() const noexcept;
  void create_one_node_command() const noexcept;
  void create_one_node_keyboard() const noexcept;
  void create_one_node_mouse() const noexcept;
  void create_ten_nodes_and_undo_command() const noexcept;
  void create_ten_nodes_and_undo_keyboard() const noexcept;
  void create_two_nodes_command() const noexcept;
  void create_two_nodes_keyboard() const noexcept;
  void default_construction() const noexcept;
  void delete_node_that_is_connected_to_multiple_edges_keyboard() const noexcept;
  void DeleteNodeThatIsHeadOfEdgeKeyboard() const noexcept;
  void DeleteNodeThatIsHeadOfEdgeAndUndoKeyboard() const noexcept;
  void DeleteNodeThatIsTailOfEdgeKeyboard() const noexcept;
  void DeleteNodesThatAreHeadAndTailOfEdgeKeyboard() const noexcept;
  void delete_one_edge_by_node_command() const noexcept;
  void delete_one_edge_by_node_keyboard() const noexcept;
  void delete_one_edge_command() const noexcept;
  void delete_one_edge_keyboard() const noexcept;
  void delete_one_node_command() const noexcept;
  void delete_one_node_command_and_undo() const noexcept;
  void delete_one_node_keyboard() const noexcept;
  void delete_two_nodes_command() const noexcept;
  void DeleteTwoNodesKeyboard() const noexcept;
  void DoubleClick() const noexcept;
  void DoubleClickTwice() const noexcept;
  void EditModeFlags() const noexcept;
  void GetHighlighter() const noexcept;
  void HideExamples() const noexcept;
  void IsCommandPutOnUndoStack() const noexcept;
  void MouseWheel() const noexcept;
  void NcenterNodesAndQtCenterNodesMustMatchEdit() const noexcept; //#113
  void NcenterNodesAndQtCenterNodesMustMatchRate() const noexcept; //#113
  void NcenterNodesAndQtCenterNodesMustMatchUninitialized() const noexcept; //#113
  void PressCtrlRightMovesNonCentralNode() const noexcept;
  void PressEscape() const noexcept;
  void PressF1OnEmptyConceptMapIsRejected() const noexcept;
  void PressF2CanEditNonFocalQuestion() const noexcept;
  void PressF2CannotEditFocalQuestion() const noexcept;
  void PressF2OnEmptyConceptMapIsRejected() const noexcept;
  void PressF4IsRejected() const noexcept;
  void PressH() const noexcept;
  void PressN() const noexcept;
  void PressQuestionMark() const noexcept;
  void PressRightSelectsEdgeExclusively() const noexcept;
  void PressRightSelectsNodeExclusively() const noexcept;
  void PressShiftRightSelectsEdgeAdditively() const noexcept;
  void PressShiftRightSelectsNodeAdditively() const noexcept;
  void PressSpaceOnEmptyConceptMapIsRejected() const noexcept;
  void PressT() const noexcept;
  void PressZ() const noexcept;
  void QtNodeInCenterMustBeGold() const noexcept; //#111
  void QtNodesMustShowExampleWhenFocused() const noexcept; //#96
  void rate_concept_map_has_less_focusable_items() const noexcept;
  void rate_mode_flags() const noexcept;
  void select_random_node_keyboard_edit() const noexcept; //#138, skip on Travis

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

  void SingleClickOnEmptyConceptMap() const noexcept;
  void SingleClickOnEmptyConceptMapIsNotAccepted() const noexcept;
  void SingleClickOnNodeIsAccepted() const noexcept;
  void SingleClickOnNodeSelectsNode() const noexcept;
  void TwoClicksOnNodeSelectsAndUnselectsIt() const noexcept;
  void UninitializedModeFlags() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmap_test_TEST_H
