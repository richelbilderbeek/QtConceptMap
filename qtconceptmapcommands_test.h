#ifndef QTCONCEPTMAP_COMMANDS_TEST_H
#define QTCONCEPTMAP_COMMANDS_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapcommands_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:


  //DONE
  void get_commands() const noexcept;
  void nonsense_to_empty() const noexcept;
  void create_one_new_node_command() const noexcept;
  void create_one_new_node_then_unselect() const noexcept;
  void create_relation_over_center_node() const noexcept;
  void create_two_new_node_commands() const noexcept;
  void create_new_edge_between_two_nodes_command() const noexcept;
  void create_new_edge_between_two_selected_nodes_command() const noexcept;
  void load_command() const noexcept;
  void move_command() const noexcept;
  void save_and_load() const noexcept; //#137
  void save_command_empty_concept_map() const noexcept;
  void save_command_two_nodes_one_edge() const noexcept;
  void select_command() const noexcept;
  void set_mode_command() const noexcept;
  void unselect_command() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDS_TEST_H

