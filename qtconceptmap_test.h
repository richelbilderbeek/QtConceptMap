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
  void PressCtrlRightMovesNonCentralNode() const noexcept;
  void CannotMoveCenterNode() const noexcept; //#115

  //Done
  void CannotDeleteCenterNode() const noexcept; //#114
  void CannotEditCenterNode() const noexcept; //#104
  void ChangeModes() const noexcept;
  void ClickOnNothingShouldBeIgnored() const noexcept;
  void concept_map_must_fit_window() const noexcept; //#135
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
  void delete_node_that_is_head_of_edge_keyboard() const noexcept;
  void delete_node_that_is_head_of_edge_and_undo_keyboard() const noexcept;
  void delete_node_that_is_tail_of_edge_keyboard() const noexcept;
  void delete_nodes_that_are_head_and_tail_of_edge_keyboard() const noexcept;
  void delete_one_edge_by_node_command() const noexcept;
  void delete_one_edge_by_node_keyboard() const noexcept;
  void delete_one_edge_command() const noexcept;
  void delete_one_edge_keyboard() const noexcept;
  void delete_one_node_command() const noexcept;
  void delete_one_node_command_and_undo() const noexcept;
  void delete_one_node_keyboard() const noexcept;
  void delete_two_nodes_command() const noexcept;
  void delete_two_nodes_keyboard() const noexcept;
  void double_click() const noexcept;
  void double_click_twice() const noexcept;
  void edit_mode_flags() const noexcept;
  void get_highlighter() const noexcept;
  void hide_examples() const noexcept;
  void is_command_put_on_undo_stack() const noexcept;
  void mouse_wheel() const noexcept;
  void n_center_nodes_and_qt_center_nodes_must_match_edit() const noexcept; //#113
  void n_center_nodes_and_qt_center_nodes_must_match_rate() const noexcept; //#113
  void n_center_nodes_and_qt_center_nodes_must_match_uninitialized() const noexcept; //#113
  void PressEscape() const noexcept;
  void PressF1OnEmptyConceptMapIsRejected() const noexcept;
  void PressF2OnEmptyConceptMapIsRejected() const noexcept;
  void PressF2CanEditNonFocalQuestion() const noexcept;
  void PressF2CannotEditFocalQuestion() const noexcept;
  void PressF4IsRejected() const noexcept;
  void PressH() const noexcept;
  void PressN() const noexcept;
  void PressQuestionMark() const noexcept;
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
  void set_empty_concept_map_one_selected_node() const noexcept;

  ///Set an empty ConceptMap in a QtConcepytMap
  ///when two QtNodes are selected, one of which on an edge
  void set_empty_concept_map_two_selected_nodes() const noexcept;


  void setting_concept_maps_edges_qtedges_nodes_qtnodes_must_match() const noexcept;
  void uninitialized_mode_flags() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmap_test_TEST_H
