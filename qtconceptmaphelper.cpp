#include "qtconceptmaphelper.h"
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "conceptmap.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmap.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapcollect.h"
#include "count_vertices_with_selectedness.h"
#include "get_my_custom_edge.h"
#include "count_edges_with_selectedness.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "has_custom_edge.h"


int ribi::cmap::CountQtArrowHeads(const QGraphicsScene& scene) noexcept
{
  const auto qtedges = GetQtEdges(scene);
  return std::count_if(std::begin(qtedges), std::end(qtedges),
    [](const QtEdge* const qtedge)
    {
      return HasHeadArrow(*qtedge);
    }
  );
}

int ribi::cmap::CountQtArrowTails(const QGraphicsScene& scene) noexcept
{
  const auto qtedges = GetQtEdges(scene);
  return std::count_if(std::begin(qtedges), std::end(qtedges),
    [](const QtEdge* const qtedge)
    {
      return HasTailArrow(*qtedge);
    }
  );
}

int ribi::cmap::CountQtCenterNodes(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (const auto item: scene.items()) {
    if (IsQtCenterNode(item)) ++cnt;
  }
  return cnt;
}

int ribi::cmap::CountQtEdges(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (auto item: scene.items()) {
    if (dynamic_cast<QtEdge*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountQtNodes(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (const auto item: scene.items()) {
    if (IsQtNodeNotOnEdge(item, scene)) ++cnt;
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtEdges(const QGraphicsScene& scene) noexcept
{
  //Impl1 always works, as it relies on the QtEdges
  //Impl2 relies on QtNodes, which are hidden when connected to a center node
  assert(
       (CountSelectedQtEdgesImpl1(scene) == CountSelectedQtEdgesImpl2(scene)
           && CountQtCenterNodes(scene) == 0)
    || (CountSelectedQtEdgesImpl1(scene) >= CountSelectedQtEdgesImpl2(scene)
           && CountQtCenterNodes(scene)  > 0)
  );
  return CountSelectedQtEdgesImpl1(scene);
}

int ribi::cmap::CountSelectedQtEdgesImpl1(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (auto item: scene.items())
  {
    if (dynamic_cast<QtEdge*>(item)
      && dynamic_cast<QtEdge*>(item)->IsSelected()
    )
    {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtEdgesImpl2(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (auto item: scene.items()) {
    if (dynamic_cast<QtNode*>(item)
      && dynamic_cast<QtNode*>(item)->isSelected()
      && IsOnEdge(dynamic_cast<QtNode*>(item), scene)
    ) {
      ++cnt;
    }
  }
  return cnt;

}

int ribi::cmap::CountSelectedQtNodes(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (auto item: scene.items()) {
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
  const auto g = qtconceptmap.ToConceptMap();
  const auto n_nodes = static_cast<int>(boost::num_vertices(g));
  const auto n_edges = static_cast<int>(boost::num_edges(g));
  const auto n_qtnodes = CountQtNodes(*qtconceptmap.scene());
  const auto n_qtedges = CountQtEdges(*qtconceptmap.scene());
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
  const int n_nodes_desired,
  bool verbose
)
{
  const auto g = qtconceptmap.ToConceptMap();
  const auto n_selected_qtnodes = CountSelectedQtNodes(qtconceptmap);
  const auto n_selected_qtedges = CountSelectedQtEdges(qtconceptmap);

  if (n_selected_qtnodes != n_nodes_desired)
  {
    if (verbose)
    {
      std::clog << "n_selected_qtnodes (" << n_selected_qtnodes
        << ") != n_nodes_desired (" << n_nodes_desired << ")";
    }
    return false;
  }
  if (n_selected_qtedges != n_edges_desired)
  {
    if (verbose)
    {
      std::clog << "n_selected_qtedges (" << n_selected_qtedges
        << ") != n_edges_desired (" << n_edges_desired << ")";
    }
    return false;
  }
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
    has_custom_edge(
      [id = qtedge->GetEdge().GetId()](const Edge& edge) { return edge.GetId() == id; },
      conceptmap
    )
  );
  const auto ed = ::find_first_custom_edge(
    [id = qtedge->GetEdge().GetId()](const Edge& edge) { return edge.GetId() == id; },
    conceptmap
  );
  const Edge edge = get_my_custom_edge(ed, conceptmap);
  assert(edge.GetId() == qtedge->GetEdge().GetId());
  return edge;
}

ribi::cmap::QtEdge *
ribi::cmap::ExtractTheOneSelectedQtEdge(const QGraphicsScene& scene)
{
  if (CountSelectedQtEdges(scene) != 1)
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
    QtEdge * const qtedge_behind_node = FindQtEdge(qtnode, scene);
    if (qtedge_behind_node) return qtedge_behind_node;
  }

  //Nope, it cannot be found
  std::stringstream msg;
  msg << __func__ << ": "
    << "The selected item must be a QtEdge, "
    << "or a QtNode on a QtEdge"
  ;
  throw std::invalid_argument(msg.str());
}

ribi::cmap::QtEdge * ribi::cmap::FindFirstQtEdge(
  const QGraphicsScene& scene, const std::function<bool(QtEdge*)> predicate) noexcept
{
  for (auto item: scene.items())
  {
    QtEdge * const qtedge = dynamic_cast<QtEdge*>(item);
    if (qtedge && predicate(qtedge))
    {
      return qtedge;
    }
  }
  return nullptr;
}

ribi::cmap::QtEdge * ribi::cmap::FindFirstQtEdgeWithName(
  const QGraphicsScene& scene,
  const std::string& name) noexcept
{
  return FindFirstQtEdge(
    scene,
    [name](const QtEdge* const qtedge)
    {
      return GetText(*qtedge) == name;
    }
  );
}

ribi::cmap::QtNode * ribi::cmap::FindFirstQtNode(
  const QGraphicsScene& scene, const std::function<bool(QtNode*)> predicate) noexcept
{
  for (auto item: scene.items())
  {
    QtNode * const qtnode = dynamic_cast<QtNode*>(item);
    if (qtnode && predicate(qtnode))
    {
      return qtnode;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode * ribi::cmap::FindFirstQtNodeWithName(
  const QGraphicsScene& scene,
  const std::string& name) noexcept
{
  return FindFirstQtNode(scene, QtNodeHasName(name));
}

ribi::cmap::QtNode * ribi::cmap::FindQtCenterNode(const QGraphicsScene& scene) noexcept
{
  if (!CountQtCenterNodes(scene)) return nullptr;
  assert(CountQtCenterNodes(scene) == 1);
  for (QtNode * const qtnode: GetQtNodes(scene))
  {
    if (IsQtCenterNode(qtnode)) return qtnode;
  }
  assert(!"Should not get here"); //!OCLINT accepted idiom
  return nullptr;
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const int edge_id,
  const QGraphicsScene& scene
) noexcept
{
  for (auto item: scene.items())
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
  const QtNode * const qtnode,
  const QGraphicsScene& scene
) noexcept
{
  for (const auto item: scene.items())
  {
    QtEdge * const qtedge = dynamic_cast<QtEdge*>(item);
    if (qtedge && qtedge->GetQtNode() == qtnode)
    {
      return qtedge;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode *
ribi::cmap::GetQtCenterNode(const QGraphicsScene& scene) noexcept
{
  const auto qtnodes = GetQtCenterNodes(scene);
  if (qtnodes.empty()) return nullptr;
  return qtnodes.front();
}

ribi::cmap::QtEdge * ribi::cmap::GetFirstQtEdge(const QGraphicsScene& scene) noexcept
{
  const auto qtedges = GetQtEdges(scene);
  if (qtedges.empty()) return nullptr;
  return qtedges.front();
}

ribi::cmap::QtNode * ribi::cmap::GetFirstQtNode(const QGraphicsScene& scene) noexcept
{
  const auto qtnodes = GetQtNodes(scene);
  if (qtnodes.empty()) return nullptr;
  return qtnodes.front();
}

ribi::cmap::QtEdge * ribi::cmap::GetLastQtEdge(const QGraphicsScene& scene) noexcept
{
  const auto qtedges = GetQtEdges(scene);
  if (qtedges.empty()) return nullptr;
  return qtedges.back();
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetQtCenterNodes(const QGraphicsScene& scene) noexcept
{
  const auto v = GetQtNodes(scene);
  std::vector<QtNode *> w;
  std::copy_if(
    std::begin(v), std::end(v),
    std::back_inserter(w),
    [](const auto qtnode) { return IsCenterNode(*qtnode); }
  );
  return w;
}

std::vector<ribi::cmap::QtEdge*> ribi::cmap::GetQtEdges(
  const QtNode* const from,
  const QGraphicsScene& scene
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
  const QGraphicsScene& scene
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
  }
  throw std::logic_error(
    "ribi::cmap::QtConceptMap::GetNodeBrushFunction: unimplemented mode"
  );
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunctionEdit() noexcept
{
  return [](const QtNode& qtnode)
  {
    //Gold if center node
    //Gray if solitary node
    //Blue if relation node
    if (qtnode.IsCenterNode())
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
    if (qtnode.IsCenterNode())
    {
      return QtBrushFactory().CreateGoldGradientBrush();
    }
    const int n_rated
      = (qtnode.GetConcept().GetRatingComplexity()   == -1 ? 0 : 1)
      + (qtnode.GetConcept().GetRatingConcreteness() == -1 ? 0 : 1)
      + (qtnode.GetConcept().GetRatingSpecificity()  == -1 ? 0 : 1);
    switch (n_rated)
    {
      case 0: return QtBrushFactory().CreateRedGradientBrush();
      case 1:
      case 2:
        return QtBrushFactory().CreateYellowGradientBrush();
      case 3:
        return QtBrushFactory().CreateGreenGradientBrush();
      default:
        throw std::logic_error("GetNodeBrushFunction: unimplemented rating");
    }
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

/*
std::vector<ribi::cmap::QtNode *> ribi::cmap::GetQtNodes(
  const QGraphicsScene& scene
) noexcept
{
  return GetQtNodes(scene);
}
*/

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetQtNodes(const QGraphicsScene& scene) noexcept
{
  const auto qtnodes_all = Collect<QtNode>(scene);
  std::vector<QtNode*> qtnodes;
  std::copy_if(std::begin(qtnodes_all), std::end(qtnodes_all),
    std::back_inserter(qtnodes),
    [&scene](const QtNode* const qtnode)
    {
      return !IsOnEdge(qtnode, scene);
    }
  );
  return qtnodes;
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetQtNodesAlsoOnQtEdge(const QGraphicsScene& scene) noexcept
{
  return Collect<QtNode>(scene);
}


std::vector<ribi::cmap::QtEdge *>
ribi::cmap::GetSelectedQtEdges(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtEdge *> selected;
  const auto qtedges = GetQtEdges(scene);
  std::copy_if(
    std::begin(qtedges),
    std::end(qtedges),
    std::back_inserter(selected),
    [](QtEdge* const qtedge)
    {
      return qtedge->IsSelected() || qtedge->GetQtNode()->isSelected();
    }
  );
  return selected;
}

/*
std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetSelectedQtNodes(const QGraphicsScene& scene) noexcept
{

  return GetSelectedQtNodes(scene);
}
*/

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetSelectedQtNodes(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtNode *> selected;
  const auto qtnodes = GetQtNodes(scene);
  std::copy_if(
    std::begin(qtnodes),
    std::end(qtnodes),
    std::back_inserter(selected),
    [&scene](QtNode* const qtnode)
    {
      return qtnode->isSelected()
        && !IsOnEdge(qtnode, scene)
      ;
    }
  );
  return selected;
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(const QGraphicsScene& scene) noexcept
{
  //All QtNodes, also those on QtEdge
  const auto qtnodes = Collect<QtNode>(scene);
  std::vector<ribi::cmap::QtNode *> selected;
  std::copy_if(
    std::begin(qtnodes),
    std::end(qtnodes),
    std::back_inserter(selected),
    [&scene](QtNode* const qtnode)
    {
      return qtnode->isSelected();
    }
  );
  return selected;
}


bool ribi::cmap::IsDashed(const QPen& pen) noexcept
{
  return pen.style() == Qt::DashLine;
}

bool ribi::cmap::IsQtCenterNode(const QGraphicsItem* const item)
{
  const QtNode * const qtnode = dynamic_cast<const QtNode*>(item);
  if (!qtnode) return false;
  return IsQtCenterNode(*qtnode);
}

bool ribi::cmap::IsQtCenterNode(const QtNode& qtnode)
{
  return IsCenterNode(qtnode);
}

bool ribi::cmap::IsOnEdge(
  const QtNode * const qtnode,
  const QGraphicsScene& scene
) noexcept
{
  return FindQtEdge(qtnode, scene);
}

bool ribi::cmap::IsQtCenterNodeSelected(const QGraphicsScene& scene)
{
  for (const auto item: scene.selectedItems())
  {
    if (IsQtCenterNode(item))
    {
      return true;
    }
  }
  return false;
}

bool ribi::cmap::IsQtNodeNotOnEdge(
  const QGraphicsItem * const item,
  const QGraphicsScene& scene
) noexcept
{
  const QtNode* const qtnode{dynamic_cast<const QtNode*>(item)};
  return qtnode && !IsOnEdge(qtnode, scene);
}

bool ribi::cmap::IsQtNodeOnEdge(
  const QGraphicsItem * const item,
  const QGraphicsScene& scene
) noexcept
{
  const QtNode* const qtnode{dynamic_cast<const QtNode*>(item)};
  return qtnode && IsOnEdge(qtnode, scene);
}
