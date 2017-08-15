#include "qtconceptmapcommands.h"
#include "qtconceptmap.h"
#include "container.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandload.h"
#include "qtconceptmapcommandmove.h"
#include "qtconceptmapcommandsave.h"
#include "qtconceptmapcommandselect.h"
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
  if (auto p = parse_command_create_new_node(q, s)) { return p; }
  if (auto p = parse_command_create_new_edge(q, s)) { return p; }
  if (auto p = ParseCommandLoad(q, s)) { return p; }
  if (auto p = ParseCommandMove(q, s)) { return p; }
  if (auto p = ParseCommandSave(q, s)) { return p; }
  if (auto p = ParseCommandSelect(q, s)) { return p; }
  if (auto p = ParseCommandToggleArrowHead(q, s)) { return p; }
  if (auto p = ParseCommandToggleArrowTail(q, s)) { return p; }
  if (auto p = parse_command_set_mode(q, s)) { return p; }
  if (auto p = parse_command_unselect(q, s)) { return p; }
  return nullptr;
}

std::vector<ribi::cmap::Command*> ribi::cmap::ParseCommands(
  QtConceptMap& q,
  const std::vector<std::string>& args)
{
  //Get the commands as one string
  const std::string s = GetCommands(args);
  if (s.empty()) return {};

  //Convert strings to Commands, nullptr if it string cannot be parsed to any Command
  const std::vector<std::string> v = Container().SeperateString(s, ';');
  std::vector<Command*> commands;
  commands.reserve(v.size());
  std::transform(
    std::begin(v), std::end(v), std::back_inserter(commands),
    [&q](const std::string& t)
    {
      return ParseCommand(q, t);
    }
  );

  //Keep only the non-nullptr commands
  const auto new_end = std::remove(std::begin(commands), std::end(commands), nullptr);
  commands.erase(new_end, std::end(commands));

  return commands;
}

