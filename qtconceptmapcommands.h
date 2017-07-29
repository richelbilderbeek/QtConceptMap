#ifndef QTCONCEPTMAPCOMMANDS_H
#define QTCONCEPTMAPCOMMANDS_H

///Functions to work on Commands

#include <string>
#include <vector>

namespace ribi {
namespace cmap {

class Command;

/// Works on, for example  'create_node(0, 0, from)', 'create_edge(relation)'
Command* parse_command(const std::string& s);

///Parse Commands from command line arguments
///For example, QtConceptMapDemo --width 640 --height 400
///  --command "create_node(0, 0, from), create_node(0, 100, to), create_edge(relation)"
std::vector<Command*> parse_commands(const std::vector<std::string>& args);


///From, for example,
///
///  QtConceptMapDemo --width 640 --height 400
///  --command "create_node(0, 0, from), create_node(0, 100, to), create_edge(relation)"
///
/// extracts
///
///   "create_node(0, 0, from), create_node(0, 100, to), create_edge(relation)"
std::string get_commands(const std::vector<std::string>& args);

} //~namespace cmap {
} //~namespace ribi {


#endif // QTCONCEPTMAPCOMMANDS_H
