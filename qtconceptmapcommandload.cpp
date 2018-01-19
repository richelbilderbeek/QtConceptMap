#include "qtconceptmapcommandload.h"

#include <cassert>
#include <boost/algorithm/string/trim.hpp>
#include <gsl/gsl_assert>
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

ribi::cmap::CommandLoad::CommandLoad(
  QtConceptMap& qtconceptmap,
  const std::string& filename
) : Command(qtconceptmap),
    m_filename{filename}
{
  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Load from file with filename '"
      << m_filename << "'";
    this->setText(msg.str().c_str());
  }
}


ribi::cmap::CommandLoad * ribi::cmap::ParseCommandLoad(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"load(my_file.cmp)"
  boost::algorithm::trim(s);
  const std::string str_begin = "load(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"my_file.cmp"
  const std::string filename = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(filename[0] != '(');
  assert(filename.back() != ')');
  return new CommandLoad(qtconceptmap, filename);
}

void ribi::cmap::CommandLoad::Redo()
{
  Expects(QFile::exists(m_filename.c_str()));

  const auto concept_map = LoadFromFile(m_filename);
  GetQtConceptMap().SetConceptMap(concept_map);

  Ensures(CountQtNodes(GetQtConceptMap()) ==
    static_cast<int>(boost::num_vertices(concept_map)));
}

void ribi::cmap::CommandLoad::Undo()
{
  GetQtConceptMap().SetConceptMap(m_before);

  Ensures(CountQtNodes(GetQtConceptMap()) ==
    static_cast<int>(boost::num_vertices(m_before)));
}
