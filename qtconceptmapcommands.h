#ifndef QTCONCEPTMAPCOMMANDS_H
#define QTCONCEPTMAPCOMMANDS_H

///Functions to work on Commands

#include <string>
#include <vector>
#include "qtconceptmapfwd.h"
namespace ribi {
namespace cmap {

/// Works on, for example  'create_node(0, 0, from)', 'create_edge(relation)'
Command* ParseCommand(QtConceptMap& q, const std::string& s);

///Parse Commands from command line arguments
///For example, QtConceptMapDemo --width 640 --height 400
///  --command "create_node(0, 0, from), create_node(0, 100, to), create_edge(relation)"
std::vector<Command*> ParseCommands(
  QtConceptMap& q,
  const std::vector<std::string>& args);


///From, for example,
///
///  QtConceptMapDemo --width 640 --height 400
///  --command "create_node(0, 0, from), create_node(0, 100, to), create_edge(relation)"
///
/// extracts
///
///   "create_node(0, 0, from), create_node(0, 100, to), create_edge(relation)"
std::string GetCommands(const std::vector<std::string>& args);

} //~namespace cmap {
} //~namespace ribi {


#endif // QTCONCEPTMAPCOMMANDS_H
