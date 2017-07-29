#include "qtconceptmapcommandcreatenewnode.h"

#include <cassert>
#include <boost/graph/isomorphism.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include "container.h"
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
    m_conceptmap(qtconceptmap.GetConceptMap()),
    m_conceptmap_after{qtconceptmap.GetConceptMap()},
    m_conceptmap_before{qtconceptmap.GetConceptMap()},
    m_mode{qtconceptmap.GetMode()},
    m_qtnode{nullptr},
    m_scene(qtconceptmap.GetScene()),
    m_tool_item{qtconceptmap.GetQtToolItem()},
    m_tool_item_old_buddy{qtconceptmap.GetQtToolItem().GetBuddyItem()},
    m_x{x},
    m_y{y}
{


  this->setText("Create new node with text ''");

  //Add the vertex to the concept map
  VertexDescriptor vd = boost::add_vertex(m_conceptmap_after);

  //Create the node
  Node node;
  node.SetX(m_x);
  node.SetY(m_y);
  node.SetConcept(Concept(text));

  //Set the node
  {
    const auto pmap = get(boost::vertex_custom_type, m_conceptmap_after);
    put(pmap, vd, node);
  }
  //Additively select node
  {
    const auto pmap = get(boost::vertex_is_selected, m_conceptmap_after);
    put(pmap, vd, true);
  }

  //Modify the QGraphicsScene
  m_qtnode = new QtNode(node);
  assert(m_qtnode);
  assert(m_qtnode->GetCenterX() == node.GetX());
  assert(m_qtnode->GetCenterY() == node.GetY());
  assert(Unwordwrap(m_qtnode->GetText()) == text);
}

double ribi::cmap::CommandCreateNewNode::GetX() const noexcept
{
  return m_qtnode->GetCenterX();
}

double ribi::cmap::CommandCreateNewNode::GetY() const noexcept
{
  return m_qtnode->GetCenterY();
}

std::string ribi::cmap::CommandCreateNewNode::GetText() const noexcept
{
  return Unwordwrap(m_qtnode->GetText());
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
  m_conceptmap = m_conceptmap_after;
  assert(!m_qtnode->scene());
  m_scene.addItem(m_qtnode);
  assert(m_qtnode->scene());
  m_qtnode->setSelected(true); //Additively select node
  m_qtnode->setFocus();
  m_qtnode->SetBrushFunction(GetQtNodeBrushFunction(m_mode));
  m_tool_item.SetBuddyItem(m_qtnode);
}

void ribi::cmap::CommandCreateNewNode::undo()
{
  m_conceptmap = m_conceptmap_before;
  assert(m_qtnode->scene());
  m_scene.removeItem(m_qtnode);
  assert(!m_qtnode->scene());
  m_tool_item.SetBuddyItem(m_tool_item_old_buddy);
}
