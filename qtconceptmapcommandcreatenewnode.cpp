#include "qtconceptmapcommandcreatenewnode.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
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

ribi::cmap::CommandCreateNewNode::CommandCreateNewNode(
  QtConceptMap& qtconceptmap,
  const double x,
  const double y,
  const std::string& text
)
  : Command(qtconceptmap),
    m_added_qtnode{nullptr},
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
    msg << "Create new node with text '" << m_text << "'";
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

ribi::cmap::CommandCreateNewNode * ribi::cmap::parse_command_create_new_node(
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
    const double x = std::stod(v.at(0));
    const double y = std::stod(v.at(1));
    const std::string text = v.at(2);
    return new CommandCreateNewNode(qtconceptmap, x, y, text);
  }
  catch (std::exception&) {} //OK
  return nullptr;
}

void ribi::cmap::CommandCreateNewNode::redo()
{
  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());

  //Add the vertex to the concept map
  VertexDescriptor vd = boost::add_vertex(GetQtConceptMap().GetConceptMap());

  //Create the node
  Node node;
  node.SetX(m_x);
  node.SetY(m_y);
  node.SetConcept(Concept(m_text));

  //Set the node
  {
    const auto pmap = get(boost::vertex_custom_type, GetQtConceptMap().GetConceptMap());
    put(pmap, vd, node);
  }
  //Additively select node
  {
    const auto pmap = get(boost::vertex_is_selected, GetQtConceptMap().GetConceptMap());
    put(pmap, vd, true);
  }

  //Modify the QGraphicsScene
  m_added_qtnode = new QtNode(node);
  assert(m_added_qtnode);
  assert(m_added_qtnode->GetCenterX() == node.GetX());
  assert(m_added_qtnode->GetCenterY() == node.GetY());
  assert(Unwordwrap(m_added_qtnode->GetText()) == m_text);
  assert(!m_added_qtnode->scene());
  GetQtConceptMap().GetScene().addItem(m_added_qtnode);
  assert(m_added_qtnode->scene());
  m_added_qtnode->setSelected(true); //Additively select node
  m_added_qtnode->setFocus();
  m_added_qtnode->SetBrushFunction(GetQtNodeBrushFunction(GetQtConceptMap().GetMode()));

  //QtToolItem gets new buddy
  m_tool_item = &GetQtConceptMap().GetQtToolItem();
  m_tool_item_old_buddy = m_tool_item->GetBuddyItem();
  m_tool_item->SetBuddyItem(m_added_qtnode);

  qApp->processEvents();

  CheckInvariantAsMuchNodesAsQtNodesSelected(GetQtConceptMap());
}

void ribi::cmap::CommandCreateNewNode::undo()
{
  Expects(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));

  //Remove node
  boost::remove_vertex(
    find_first_custom_vertex_with_my_vertex(m_added_qtnode->GetNode(), GetQtConceptMap().GetConceptMap()),
    GetQtConceptMap().GetConceptMap()
  );

  //Remove QtNode
  assert(m_added_qtnode->scene());
  GetQtConceptMap().GetScene().removeItem(m_added_qtnode);
  assert(!m_added_qtnode->scene());
  m_tool_item->SetBuddyItem(m_tool_item_old_buddy);

  Ensures(CountSelectedQtNodes(GetQtConceptMap())
    == count_vertices_with_selectedness(true, GetQtConceptMap().GetConceptMap()));
}
