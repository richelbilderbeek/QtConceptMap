#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "container.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandload.h"
#include "qtconceptmapcommandmove.h"
#include "qtconceptmapcommandmoveedge.h"
#include "qtconceptmapcommandmovenode.h"
#include "qtconceptmapcommandsave.h"
#include "qtconceptmapcommandselect.h"
#include "qtconceptmapcommandselectedge.h"
#include "qtconceptmapcommandselectnode.h"
#include "qtconceptmapcommandsetmode.h"
#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapcommandunselect.h"

std::string ribi::cmap::GetCommands(const std::vector<std::string>& args)
{
  //Extract the --command section
  const int sz = args.size();
  if (sz < 2) return {};
  for (int i=0; i!=sz-1; ++i)
  {
    if (args[i] == "--command") return args[i+1];
  }
  return {};
}

/// Works on, for example  'create_node(0, 0, from)', 'create_edge(relation)'
ribi::cmap::Command* ribi::cmap::ParseCommand(QtConceptMap& q, const std::string& s)
{
  using ParsingFunction = std::function<Command*(QtConceptMap&, std::string)>;

  const std::vector<ParsingFunction> parsing_functions =
  {
    ParseCommandCreateNewEdge,
    ParseCommandCreateNewNode,
    ParseCommandLoad,
    ParseCommandMove,
    ParseCommandMoveEdge,
    ParseCommandMoveNode,
    ParseCommandSave,
    ParseCommandSelect,
    ParseCommandSelectEdge,
    ParseCommandSelectNode,
    ParseCommandToggleArrowHead,
    ParseCommandToggleArrowTail,
    ParseCommandSetMode,
    ParseCommandUnselect
  };
  for (const auto parsing_function: parsing_functions)
  {
    if (auto p = parsing_function(q, s)) return p;
  }
  return nullptr;
}

void ribi::cmap::ProcessCommands(
  QtConceptMap& q,
  const std::vector<std::string>& args)
{
  //Get the commands as one string
  const std::string s = GetCommands(args);
  if (s.empty()) return;

  //Convert strings to Commands, nullptr if it string cannot be parsed to any Command
  const std::vector<std::string> text_commands = Container().SeperateString(s, ';');
  for (const std::string& text_command: text_commands)
  {
    Command * const cmd = ParseCommand(q, text_command);
    if (!cmd) continue;
    q.DoCommand(cmd);
  }
}

