#include "qtconceptmapcommandcreatenewnode.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/lexical_cast.hpp>
#include <gsl/gsl_assert>
#include <QApplication>
#include "count_vertices_with_selectedness.h"
#include "container.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaphelper.h"
#include "set_vertex_selectedness.h"
#include "set_my_custom_vertex.h"

bool str_to_bool(std::string s)
{
  boost::algorithm::trim(s);
  if (s == "0") return false;
  if (s == "1") return true;
  if (s == "false") return false;
  if (s == "true") return true;
  throw std::invalid_argument("string cannot be converted to bool");
}

ribi::cmap::CommandCreateNewNode::CommandCreateNewNode(
  QtConceptMap& qtconceptmap,
  const std::string& text,
  const bool is_center_node,
  const double x,
  const double y
)
  : Command(qtconceptmap),
    m_added_qtnode{nullptr},
    m_is_center_node{is_center_node},
    m_text{text},
    m_tool_item{nullptr},
    m_tool_item_old_buddy{nullptr},
    m_x{x},
    m_y{y}
{
  Expects(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));

  //QCommands have a text
  {
    std::stringstream msg;
    msg << "Create new "
      << (m_is_center_node ? std::string("center") : std::string("regular"))
      << " << node with text '" << m_text << "' at ("
      << m_x << ", " << m_y << ")"
    ;
    this->setText(msg.str().c_str());
  }

  Ensures(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));
}

double ribi::cmap::CommandCreateNewNode::GetX() const noexcept
{
  assert(m_added_qtnode);
  return m_added_qtnode->GetCenterX();
}

double ribi::cmap::CommandCreateNewNode::GetY() const noexcept
{
  return m_added_qtnode->GetCenterY();
}

std::string ribi::cmap::CommandCreateNewNode::GetText() const noexcept
{
  return Unwordwrap(m_added_qtnode->GetText());
}

ribi::cmap::CommandCreateNewNode * ribi::cmap::ParseCommandCreateNewNode(
  QtConceptMap& qtconceptmap, std::string s)
{
  //"create_new_node(10, 20, my text)"
  boost::algorithm::trim(s);
  const std::string str_begin = "create_new_node(";
  if (s.substr(0, str_begin.size()) != str_begin) return nullptr;
  if (s.back() != ')') return nullptr;
  //"10, 20, my text"
  const std::string t = s.substr(str_begin.size(), s.size() - str_begin.size() - 1);
  assert(t[0] != '(');
  assert(t.back() != ')');
  // "10", "20, "my text"
  std::vector<std::string> v = Container().SeperateString(t, ',');
  for (std::string& u: v) boost::algorithm::trim(u);
  try
  {
    const std::string text = v.at(0);
    const bool is_center_node = str_to_bool(v.at(1));
    const double x = std::stod(v.at(2));
    const double y = std::stod(v.at(3));
    return new CommandCreateNewNode(qtconceptmap, text, is_center_node, x, y);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

void ribi::cmap::CommandCreateNewNode::Redo()
{
  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());
  

  //Add the vertex to the concept map
  VertexDescriptor vd = boost::add_vertex(GetQtConceptMap().GetConceptMap());

  //Create the node
  const Node node(Concept(m_text), m_is_center_node, m_x, m_y) ;

  //Set the node
  set_my_custom_vertex(node, vd, GetQtConceptMap().GetConceptMap());

  //Modify the QGraphicsScene
  m_added_qtnode = new QtNode(node);
  assert(m_added_qtnode);
  assert(m_added_qtnode->GetCenterX() == node.GetX());
  assert(m_added_qtnode->GetCenterY() == node.GetY());
  assert(Unwordwrap(m_added_qtnode->GetText()) == m_text);
  assert(!m_added_qtnode->scene());
  GetQtConceptMap().GetScene().addItem(m_added_qtnode);
  assert(m_added_qtnode->scene());

  //QtToolItem gets new buddy
  m_tool_item = &GetQtConceptMap().GetQtToolItem();
  m_tool_item_old_buddy = m_tool_item->GetBuddyItem();
  m_tool_item->SetBuddyItem(m_added_qtnode);

  //Additively select node
  set_vertex_selectedness(true, vd, GetQtConceptMap().GetConceptMap());

  //Additively select node,
  // must be done after setting m_added_qtnode as the buddy of m_tool_item,
  // as QtConceptMap::onSelectionChanged will be triggered
  m_added_qtnode->setSelected(true);
  m_added_qtnode->setFocus();
  m_added_qtnode->SetBrushFunction(GetQtNodeBrushFunction(GetQtConceptMap().GetMode()));

  qApp->processEvents();

  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());
  
}

void ribi::cmap::CommandCreateNewNode::Undo()
{
  
  Expects(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));

  m_added_qtnode->clearFocus();
  SetSelectedness(false, *m_added_qtnode, GetQtConceptMap());

  //Remove node
  boost::remove_vertex(
    find_first_custom_vertex_with_my_vertex(m_added_qtnode->GetNode(), GetQtConceptMap().GetConceptMap()),
    GetQtConceptMap().GetConceptMap()
  );

  //Remove QtNode
  assert(m_added_qtnode->scene());
  //
  GetQtConceptMap().GetScene().removeItem(m_added_qtnode);
  assert(!m_added_qtnode->scene());
  m_tool_item->SetBuddyItem(m_tool_item_old_buddy);

  Ensures(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));
  CheckInvariantSingleSelectedQtNodeMustHaveQtTool(GetQtConceptMap());
  
}
