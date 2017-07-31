#include "qtconceptmapmode.h"

#include <cassert>
#include <sstream>
#include <stdexcept>

std::vector<ribi::cmap::Mode> ribi::cmap::collect_all_modes() noexcept
{
  return
  {
    Mode::edit,
    Mode::rate,
    Mode::uninitialized
  };
}


ribi::cmap::Mode ribi::cmap::str_to_mode(const std::string& s)
{
  if (s == "edit") return Mode::edit;
  if (s == "rate") return Mode::rate;
  if (s == "uninitialized") return Mode::uninitialized;

  std::stringstream msg;
  msg << "Invalid string '" << s << "': cannot be converted to Mode";
  throw std::invalid_argument(msg.str());

}

std::string ribi::cmap::to_str(const Mode m) noexcept
{
  switch (m)
  {
    case Mode::edit: return "edit";
    case Mode::rate: return "rate";
    case Mode::uninitialized : return "uninitialized";
    default: break;
  }
  assert(!"Should not get here");
  return "";
}

