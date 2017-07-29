#include "qtconceptmapcommands.h"
#include "container.h"
#include "qtconceptmapcommandcreatenewnode.h"
std::string ribi::cmap::get_commands(const std::vector<std::string>& args)
{
  //Extract the --command section
  const int sz = args.size();
  if (sz < 2) return {};
  for (int i=0; i!=sz-1; ++i)
  {
    if (args[i] == "--command") return args[i+i];
  }
  return {};
}

/// Works on, for example  'create_node(0, 0, from)', 'create_edge(relation)'
ribi::cmap::Command* ribi::cmap::parse_command(const std::string& s)
{
  if (parse_command_create_new_node(s)) { return parse_command_create_new_node(s); }
  return nullptr;
}

std::vector<ribi::cmap::Command*> ribi::cmap::parse_commands(const std::vector<std::string>& args)
{
  //Get the commands as one string
  const std::string s = get_commands(args);
  if (s.empty()) return {};

  const std::vector<std::string> v = Container().SeperateString(s, ',');
  std::vector<Command*> commands;
  commands.reserve(v.size());
  std::transform(
    std::begin(v), std::end(v), std::back_inserter(commands),
    [](const std::string& command)
    {
      return parse_command(command);
    }
  );
  return commands;
}

