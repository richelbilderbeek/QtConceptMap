#include "qtconceptmapcommandsave.h"

#include <cassert>
//#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>


#include <gsl/gsl_assert>
//#include <QApplication>
#include <QFile>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"

ribi::cmap::CommandSave::CommandSave(
  QtConceptMap& qtconceptmap,
  const std::string& filename
) : Command(qtconceptmap),
    m_filename{filename}
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Save file as '"
      << m_filename << "'";
    this->setText(msg.str().c_str());
  }
}


ribi::cmap::CommandSave * ribi::cmap::ParseCommandSave(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"save(my_file.cmp)"
  boost::algorithm::trim(s);
  const std::string str_begin = "save(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my_file.cmp"
  const std::string filename = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(filename[0] != '(');
  assert(filename.back() != ')');
  return new CommandSave(qtconceptmap, filename);
}

void ribi::cmap::CommandSave::Redo()
{
  if (QFile::exists(m_filename.c_str()))
  {
    throw std::runtime_error("Will not save and overwrite existing file");
  }
  
  Expects(!QFile::exists(m_filename.c_str()));
  SaveToFile(GetQtConceptMap(), m_filename);
  Ensures(QFile::exists(m_filename.c_str()));
  
}

void ribi::cmap::CommandSave::Undo()
{
  Expects(QFile::exists(m_filename.c_str()));
  QFile::remove(m_filename.c_str());
  Ensures(!QFile::exists(m_filename.c_str()));
}
