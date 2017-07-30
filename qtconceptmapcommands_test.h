#ifndef QTCONCEPTMAP_COMMANDS_TEST_H
#define QTCONCEPTMAP_COMMANDS_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class qtconceptmapcommands_test : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void get_commands() const noexcept;

  void nonsense_to_empty() const noexcept;
  void create_one_new_node_command() const noexcept;
  void create_two_new_node_commands() const noexcept;

  void create_new_edge_command() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDS_TEST_H

