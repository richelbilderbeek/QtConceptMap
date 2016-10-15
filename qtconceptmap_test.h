#ifndef qtconceptmap_test_TEST_H
#define qtconceptmap_test_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmap_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void change_modes();
  void create_one_edge_command();
  void create_one_edge_command_and_check_z_order();
  void create_one_edge_keyboard();
  void create_one_edge_keyboard_and_undo();
  void create_one_edge_with_head_command();
  void create_one_edge_with_head_keyboard();
  void create_one_edge_with_head_and_undo_keyboard();
  void create_one_edge_with_head_and_toggle_keyboard();
  void create_one_edge_with_tail_command();
  void create_one_edge_with_tail_keyboard();
  //void create_one_edge_with_tail_and_undo_keyboard();
  //void create_one_edge_with_tail_and_toggle_keyboard();
  void create_one_node_and_undo_command();
  void create_one_node_and_undo_keyboard();
  void create_one_node_command();
  void create_one_node_keyboard();
  void create_one_node_mouse();
  void create_ten_nodes_and_undo_command();
  void create_ten_nodes_and_undo_keyboard();
  void create_two_nodes_command();
  void create_two_nodes_keyboard();
  void default_construction();
  void delete_one_edge_by_node_command();
  void delete_one_edge_by_node_keyboard();
  void delete_one_edge_command();
  void delete_one_edge_keyboard();
  void delete_one_node_command();
  void delete_one_node_keyboard();
  void delete_two_nodes_command();
  void delete_two_nodes_keyboard();
  void get_focusable_items();
  void is_command_put_on_undo_stack();
  void issue_96();
  void select_left_node_keyboard();
  void select_random_node_keyboard();
  void set_concept_maps();
  void setting_concept_maps_edges_qtedges_nodes_qtnodes_must_match();
  void all_tests();
};

} //~namespace cmap
} //~namespace ribi

#endif // qtconceptmap_test_TEST_H
