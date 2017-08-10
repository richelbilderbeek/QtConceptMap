#ifndef qtconceptmap_test_TEST_H
#define qtconceptmap_test_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmap_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:

  //Todo/review
  ///Set an empty ConceptMap in a QtConcepytMap
  ///when one QtEdge is selected
  void set_empty_concept_map_one_selected_edge() const noexcept;


  //Done
  void cannot_delete_center_node() const noexcept; //#114
  void cannot_edit_center_node() const noexcept; //#104
  void cannot_move_center_node() const noexcept; //#115
  void change_modes() const noexcept;
  void click_on_nothing_should_be_ignored() const noexcept;
  void concept_map_must_fit_window() const noexcept; //#135
  void concept_map_must_fit_window_after_setting() const noexcept;
  void create_one_edge_command() const noexcept;
  void create_one_edge_command_and_check_z_order() const noexcept;
  void create_one_edge_keyboard() const noexcept;
  void create_one_edge_keyboard_and_undo() const noexcept;
  void create_one_edge_with_head_command() const noexcept;
  void create_one_edge_with_head_keyboard() const noexcept;
  void create_one_edge_with_head_and_undo_keyboard() const noexcept;
  void create_one_edge_with_head_and_toggle_keyboard() const noexcept;
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
  void press_escape() const noexcept;
  void press_f1_on_empty_concept_map_is_rejected() const noexcept;
  void press_f2_on_empty_concept_map_is_rejected() const noexcept;
  void press_f2_can_edit_non_focal_question() const noexcept;
  void press_f2_cannot_edit_focal_question() const noexcept;
  void press_f4_is_rejected() const noexcept;
  void press_h() const noexcept;
  void press_n() const noexcept;
  void press_question_mark() const noexcept;
  void press_space_on_empty_concept_map_is_rejected() const noexcept;
  void press_t() const noexcept;
  void press_z() const noexcept;
  void qtcenter_node_must_be_gold() const noexcept; //#111
  void qtnodes_must_show_example_when_focused() const noexcept; //#96
  void rate_concept_map_has_less_focusable_items() const noexcept;
  void rate_mode_flags() const noexcept;
  void select_left_node_keyboard() const noexcept; // Skip on Travis
  void select_random_node_keyboard_edit() const noexcept; //#138, skip on Travis

  void set_concept_map_4() const noexcept;

  void set_concept_maps() const noexcept;


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
