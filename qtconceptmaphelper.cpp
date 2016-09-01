#include "qtconceptmaphelper.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "conceptmap.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapcenternode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapcollect.h"
#include "count_vertices_with_selectedness.h"
#include "get_my_custom_edge.h"
#include "count_edges_with_selectedness.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "has_custom_edge_with_my_edge.h"

int ribi::cmap::CountQtEdges(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtEdge*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountQtNodes(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    const QtNode* const qtnode = dynamic_cast<QtNode*>(item);
    if (qtnode && !IsOnEdge(qtnode, scene)) ++cnt;
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtEdges(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtEdge*>(item)
      && dynamic_cast<QtEdge*>(item)->isSelected()
    ) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtNodes(const QGraphicsScene * const scene) noexcept
{
  int cnt{0};
  for (auto item: scene->items()) {
    if (dynamic_cast<QtNode*>(item)
      && dynamic_cast<QtNode*>(item)->isSelected()
      && !IsOnEdge(dynamic_cast<QtNode*>(item), scene)
    ) {
      ++cnt;
    }
  }
  return cnt;
}

bool ribi::cmap::DoubleCheckEdgesAndNodes(
  const QtConceptMap& qtconceptmap,
  const int n_edges_desired,
  const int n_nodes_desired
)
{
  const auto g = qtconceptmap.GetConceptMap();
  const auto n_nodes = static_cast<int>(boost::num_vertices(g));
  const auto n_edges = static_cast<int>(boost::num_edges(g));
  const auto n_qtnodes = CountQtNodes(qtconceptmap.GetScene());
  const auto n_qtedges = CountQtEdges(qtconceptmap.GetScene());
  if (n_nodes != n_qtnodes)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_nodes (" << n_nodes << ") != n_qtedges (" << n_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_edges != n_qtedges)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_edges (" << n_edges << ") != n_qtedges (" << n_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_nodes != n_nodes_desired) return false;
  if (n_edges != n_edges_desired) return false;
  return true;
}

bool ribi::cmap::DoubleCheckSelectedEdgesAndNodes(
  const QtConceptMap& qtconceptmap,
  const int n_edges_desired,
  const int n_nodes_desired
)
{
  const auto g = qtconceptmap.GetConceptMap();
  const auto n_selected_nodes = count_vertices_with_selectedness(true,g);
  const auto n_selected_edges = count_edges_with_selectedness(true,g);
  const auto n_selected_qtnodes = CountSelectedQtNodes(qtconceptmap.GetScene());
  const auto n_selected_qtedges = CountSelectedQtEdges(qtconceptmap.GetScene());

  if (n_selected_nodes != n_selected_qtnodes)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_selected_nodes (" << n_selected_nodes << ") != n_selected_qtnodes ("
      << n_selected_qtnodes << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_selected_edges != n_selected_qtedges)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Internal inconsistency, "
      << "n_selected_edges (" << n_selected_edges << ") != n_selected_qtedges ("
      << n_selected_qtedges << ")"
    ;
    throw std::logic_error(msg.str());
  }
  if (n_selected_nodes != n_nodes_desired) return false;
  if (n_selected_edges != n_edges_desired) return false;
  return true;
}

ribi::cmap::Edge ribi::cmap::ExtractTheOneSelectedEdge(
  const ConceptMap& conceptmap, const QGraphicsScene& scene
)
{
  //Must check on ID here, as QtEdge and its Edge may mismatch,
  //due to the positions at the endpoint
  const auto qtedge = ExtractTheOneSelectedQtEdge(scene);
  assert(
    has_custom_edge_with_my_edge(
      qtedge->GetEdge(), conceptmap,
      [](const Edge& lhs, const Edge& rhs) { return lhs.GetId() == rhs.GetId(); }
    )
  );
  const auto ed = ::find_first_custom_edge_with_my_edge(
    qtedge->GetEdge(), conceptmap,
    [](const Edge& lhs, const Edge& rhs) { return lhs.GetId() == rhs.GetId(); }
  );
  const Edge edge = get_my_custom_edge(ed, conceptmap);
  assert(edge.GetId() == qtedge->GetEdge().GetId());
  return edge;
}

ribi::cmap::QtEdge *
ribi::cmap::ExtractTheOneSelectedQtEdge(const QGraphicsScene& scene)
{
  if (scene.selectedItems().size() != 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "Must have one selected item, instead of "
      << scene.selectedItems().size() << " items"
    ;
    throw std::invalid_argument(msg.str());
  }
  auto item = scene.selectedItems().front();

  //Is it an edge?
  if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item))
  {
    return qtedge;
  }
  else if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
  {
    //Or is it the node on an edge?
    QtEdge * const qtedge = FindQtEdge(qtnode, &scene);
    if (qtedge) return qtedge;
  }

  //Nope, it cannot be found
  std::stringstream msg;
  msg << __func__ << ": "
    << "The selected item must be a QtEdge, "
    << "or a QtNode on a QtEdge"
  ;
  throw std::invalid_argument(msg.str());
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const int edge_id,
  const QGraphicsScene * const scene
) noexcept
{
  for (auto item: scene->items())
  {
    QtEdge * qtedge = dynamic_cast<QtEdge*>(item);
    if (qtedge && qtedge->GetEdge().GetId() == edge_id)
    {
      return qtedge;
    }
  }
  return nullptr;
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const QtNode* const from,
  const QtNode* const to,
  const QGraphicsScene * const scene
) noexcept
{

  assert(from);
  assert(to);
  assert(from != to);
  const std::vector<QtEdge*> edge_concepts = Collect<QtEdge>(scene);
  const auto iter = std::find_if(edge_concepts.begin(),edge_concepts.end(),
    [from,to](const QtEdge* const qtedge)
    {
      return
        (*qtedge->GetFrom() == *from && *qtedge->GetTo() == *to)
     || (*qtedge->GetFrom() == *to && *qtedge->GetTo() == *from);
    }
  );
  if (iter == edge_concepts.end()) return nullptr;
  return * iter;
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept
{
  for (const auto item: scene->items())
  {
    QtEdge * const qtedge = dynamic_cast<QtEdge*>(item);
    if (qtedge && qtedge->GetQtNode() == qtnode)
    {
      return qtedge;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode * ribi::cmap::FindQtNode(
  const int node_id, const QGraphicsScene * const scene) noexcept
{
  for (auto item: scene->items())
  {
    QtNode * const qtnode = dynamic_cast<QtNode*>(item);
    if (qtnode && qtnode->GetNode().GetId() == node_id)
    {
      return qtnode;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode *
ribi::cmap::GetCenterNode(const QGraphicsScene& scene) noexcept
{
  if (scene.items().isEmpty()) return nullptr;
  assert(scene.items()[0]);
  QList<QGraphicsItem *> v = scene.items();
  const int n_centernodes{
    static_cast<int>(
      std::count_if(v.begin(),v.end(),
        [](const QGraphicsItem * const item) { return IsQtCenterNode(item); }
      )
    )
  };
  assert(n_centernodes == 0 || n_centernodes == 1);
  if (n_centernodes == 0) return nullptr;
  assert(n_centernodes == 1);
  const auto iter = std::find_if(v.begin(),v.end(),
    [](const QGraphicsItem * const item) { return IsQtCenterNode(item); } );
  assert(iter != v.end());
  QtNode * const center_node = dynamic_cast<QtNode*>(*iter);
  assert(center_node);
  assert(IsQtCenterNode(center_node));
  return center_node;
}

std::vector<ribi::cmap::QtEdge*> ribi::cmap::GetQtEdges(
  const QtNode* const from,
  const QGraphicsScene * const scene
) noexcept
{
  assert(from);
  const std::vector<QtEdge*> v = GetQtEdges(scene);
  std::vector<QtEdge*> w;
  std::copy_if(v.begin(),v.end(),std::back_inserter(w),
    [from](const QtEdge* const qtedge)
    {
      return *qtedge->GetFrom() == *from || *qtedge->GetTo() == *from;
    }
  );
  return w;
}


std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetQtEdges(
  const QGraphicsScene * const scene
) noexcept
{
  return Collect<QtEdge>(scene);
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunction(const Mode mode)
{
  switch (mode)
  {
    case Mode::edit: return GetQtNodeBrushFunctionEdit();
    case Mode::rate: return GetQtNodeBrushFunctionRate();
    case Mode::uninitialized: return GetQtNodeBrushFunctionUninitialized();
    default:
      throw std::logic_error(
        "ribi::cmap::QtConceptMap::GetNodeBrushFunction: unimplemented mode"
      );
  }
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunctionEdit() noexcept
{
  return [](const QtNode& qtnode)
  {
    //Gold if center node
    //Gray if solitary node
    //Blue if relation node
    const auto node = qtnode.GetNode();
    if (node.IsCenterNode())
    {
      return QtBrushFactory().CreateGoldGradientBrush();
    }
    if (!qtnode.parentItem())
    {
      return QtBrushFactory().CreateGrayGradientBrush();
    }
    assert(qtnode.parentItem());
    return QtBrushFactory().CreateBlueGradientBrush();
  };
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunctionRate() noexcept
{
  return [](const QtNode& qtnode)
  {
    const auto node = qtnode.GetNode();
    if (node.IsCenterNode())
    {
      return QtBrushFactory().CreateGoldGradientBrush();
    }
    const int n_rated
      = (node.GetConcept().GetRatingComplexity()   != -1 ? 1 : 0)
      + (node.GetConcept().GetRatingConcreteness() != -1 ? 1 : 0)
      + (node.GetConcept().GetRatingSpecificity()  != -1 ? 1 : 0);
    switch (n_rated)
    {
      case 0: return QtBrushFactory().CreateRedGradientBrush();
      case 1:
      case 2:
        return QtBrushFactory().CreateYellowGradientBrush();
      case 3:
        return QtBrushFactory().CreateGreenGradientBrush();
      default: assert(!"Should not get here");
    }
    throw std::logic_error("GetNodeBrushFunction: unimplemented rating");
  };
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunctionUninitialized() noexcept
{
  return [](const QtNode&)
  {
    return QtBrushFactory().CreateWhiteGradientBrush();
  };
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetQtNodes(
  const QGraphicsScene * const scene
) noexcept
{
  return Collect<QtNode>(scene);
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetQtNodesNotOnEdge(const QGraphicsScene * const scene) noexcept
{
  const auto qtnodes_all = GetQtNodes(scene);
  std::vector<QtNode*> qtnodes;
  std::copy_if(std::begin(qtnodes_all), std::end(qtnodes_all),
    std::back_inserter(qtnodes),
    [](const QtNode* const qtnode)
    {
      //Solitary QtNodes have no parent
      return !qtnode->parentItem();
    }
  );
  return qtnodes;
}


std::vector<ribi::cmap::QtEdge *>
ribi::cmap::GetSelectedQtEdges(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtEdge *> selected;
  const auto qtedges = GetQtEdges(&scene);
  std::copy_if(
    std::begin(qtedges),
    std::end(qtedges),
    std::back_inserter(selected),
    [](QtEdge* const qtedge)
    {
      return qtedge->isSelected() || qtedge->GetQtNode()->isSelected();
    }
  );
  return selected;
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetSelectedQtNodes(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtNode *> selected;
  const auto qtnodes = GetQtNodes(&scene);
  std::copy_if(
    std::begin(qtnodes),
    std::end(qtnodes),
    std::back_inserter(selected),
    [](QtNode* const qtnode) { return qtnode->isSelected(); }
  );
  return selected;
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetSelectedQtNodesNotOnEdge(const QGraphicsScene& scene) noexcept
{
  const auto qtnodes_all = GetSelectedQtNodes(scene);
  std::vector<QtNode*> qtnodes;
  std::copy_if(std::begin(qtnodes_all), std::end(qtnodes_all),
    std::back_inserter(qtnodes),
    [](const QtNode* const qtnode)
    {
      //Solitary QtNodes have no parent
      return !qtnode->parentItem();
    }
  );
  return qtnodes;

}

bool ribi::cmap::IsQtCenterNode(const QGraphicsItem* const item)
{
  const QtCenterNode * const qtnode = dynamic_cast<const QtCenterNode*>(item);
  assert(!qtnode || IsCenterNode(qtnode->GetNode()));
  return qtnode;
}

bool ribi::cmap::IsOnEdge(
  const QtNode * const qtnode,
  const QGraphicsScene * const scene
) noexcept
{
  return FindQtEdge(qtnode, scene);
}

void ribi::cmap::MessUp(QGraphicsScene& scene)
{
  for (QGraphicsItem * const item: scene.items())
  {
    const int dx{(std::rand() % 20) - 10};
    const int dy{(std::rand() % 20) - 10};
    item->moveBy(dx, dy);
  }

}
