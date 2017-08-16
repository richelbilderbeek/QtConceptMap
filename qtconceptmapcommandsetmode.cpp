#include "qtconceptmapcommandsetmode.h"

#include <cassert>
#include <sstream>

#include <gsl/gsl_assert>
#include <boost/algorithm/string/trim.hpp>

#include "conceptmap.h"
#include "qtconceptmap.h"

ribi::cmap::CommandSetMode::CommandSetMode(
  QtConceptMap& qtconceptmap,
  const Mode mode
) : Command(qtconceptmap),
    m_mode{mode},
    m_mode_prev{Mode::uninitialized}
{
  //QUndoText
  std::stringstream msg;
  msg << "Set new mode to " << to_str(m_mode);
  this->setText(msg.str().c_str());
}

ribi::cmap::CommandSetMode::~CommandSetMode()
{
  //Empty indeed
}

ribi::cmap::CommandSetMode * ribi::cmap::ParseCommandSetMode(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"set_mode(my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "set_mode(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  // "my text"
  try
  {
    return new CommandSetMode(qtconceptmap, str_to_mode(t));
  }
  catch (std::exception&) //No problem
  {
    return nullptr;
  }
}

void ribi::cmap::CommandSetMode::redo()
{
  m_mode_prev = GetQtConceptMap().GetMode();
  GetQtConceptMap().SetMode(m_mode);
  GetQtConceptMap().update();
}

void ribi::cmap::CommandSetMode::undo()
{
  GetQtConceptMap().SetMode(m_mode_prev);
  GetQtConceptMap().update();
}
