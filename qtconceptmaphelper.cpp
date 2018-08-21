#include "qtconceptmaphelper.h"

#include <QDebug>
#include <QGraphicsScene>

#include "qtconceptmap.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapcollect.h"
#include "qtconceptmapqtedge.h"

int ribi::cmap::CountQtArrowHeads(const QGraphicsScene& scene) noexcept
{
  #ifndef TRUST_GETQTEDGES_20180821
  int cnt{0};
  for (QGraphicsItem * const item: scene.items())
  {
    if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      if (HasHeadArrow(*qtedge)) ++cnt;
    }
  }
  return cnt;
  #else
  const auto qtedges = GetQtEdges(scene);
  return std::count_if(std::begin(qtedges), std::end(qtedges),
    [](const QtEdge* const qtedge)
    {
      return HasHeadArrow(*qtedge);
    }
  );
  #endif
}

int ribi::cmap::CountQtArrowTails(const QGraphicsScene& scene) noexcept
{
  #ifndef TRUST_GETQTEDGES_20180821
  int cnt{0};
  for (QGraphicsItem * const item: scene.items())
  {
    if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      if (HasTailArrow(*qtedge)) ++cnt;
    }
  }
  return cnt;
  #else
  const auto qtedges = GetQtEdges(scene);
  return std::count_if(std::begin(qtedges), std::end(qtedges),
    [](const QtEdge* const qtedge)
    {
      return HasTailArrow(*qtedge);
    }
  );
  #endif
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
    if (qgraphicsitem_cast<QtEdge*>(item)) {
      ++cnt;
    }
  }
  return cnt;
}

int ribi::cmap::CountQtNodes(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (const auto item: scene.items()) {
    if (IsQtNodeNotOnEdge(item)) ++cnt;
  }
  return cnt;
}

int ribi::cmap::CountSelectedQtEdges(const QGraphicsScene& scene) noexcept
{
  int cnt{0};
  for (auto item: scene.selectedItems()) {
    if (qgraphicsitem_cast<QtEdge*>(item)
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
    if (qgraphicsitem_cast<QtNode*>(item)
      && item->isSelected()
      && !IsQtNodeOnEdge(item)
    ) {
      ++cnt;
    }
  }
  return cnt;
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
  if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
  {
    return qtedge;
  }
  else if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
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
    QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item);
    if (qtedge && predicate(qtedge))
    {
      return qtedge;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode * ribi::cmap::FindFirstQtNode(
  const QGraphicsScene& scene, const std::function<bool(QtNode*)> predicate) noexcept
{
  for (auto item: scene.items())
  {
    QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item);
    if (qtnode && predicate(qtnode))
    {
      return qtnode;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode * ribi::cmap::FindQtCenterNode(const QGraphicsScene& scene) noexcept
{
  if (CountQtCenterNodes(scene) > 1) return nullptr;
  for (QtNode * const qtnode: GetQtNodes(scene))
  {
    if (IsQtCenterNode(qtnode)) return qtnode;
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
    QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item);
    if (qtedge && qtedge->GetQtNode() == qtnode)
    {
      return qtedge;
    }
  }
  return nullptr;
}

ribi::cmap::QtNode *
ribi::cmap::GetQtCenterNode(const QGraphicsScene& scene)
{
  const auto qtnodes = GetQtCenterNodes(scene);
  if (qtnodes.empty()) return nullptr;
  assert(qtnodes.size() == 1);
  return qtnodes.front();
}

ribi::cmap::QtEdge * ribi::cmap::GetFirstQtEdge(const QGraphicsScene& scene) noexcept
{
  #ifndef TRUST_GETQTEDGES_20180821
  for (QGraphicsItem * const item: scene.items())
  {
    if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      return qtedge;
    }
  }
  return nullptr;
  #else
  const auto qtedges = GetQtEdges(scene);
  if (qtedges.empty()) return nullptr;
  return qtedges.front();
  #endif
}

ribi::cmap::QtNode * ribi::cmap::GetFirstQtNode(const QGraphicsScene& scene) noexcept
{
  const auto qtnodes = GetQtNodes(scene);
  if (qtnodes.empty()) return nullptr;
  return qtnodes.front();
}

ribi::cmap::QtEdge * ribi::cmap::GetLastQtEdge(const QGraphicsScene& scene) noexcept
{
  const std::vector<QtEdge*> qtedges = GetQtEdges(scene);
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
  std::vector<QtEdge*> w;
  #ifndef TRUST_GETQTEDGES_20180821
  for (QGraphicsItem * const item: scene.items())
  {
    if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      if (qtedge->GetFrom() == from || qtedge->GetTo() == from)
      {
        w.push_back(qtedge);
      }
    }
  }
  #else
  const std::vector<QtEdge*> v = GetQtEdges(scene);
  std::copy_if(v.begin(),v.end(), std::back_inserter(w),
    [from](const QtEdge* const qtedge)
    {
      return qtedge->GetFrom() == from || qtedge->GetTo() == from;
    }
  );
  #endif
  return w;
}


std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetQtEdges(
  const QGraphicsScene& scene
) noexcept
{
  #ifndef TRUST_COLLECT_QTEDGES_20180821
  std::vector<QtEdge *> qtedges;
  for (QGraphicsItem * const item: scene.items())
  {
    if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      qtedges.push_back(qtedge);
    }
  }
  return qtedges;
  #else
  //Unsure if this works
  return Collect<QtEdge>(scene);
  #endif // TRUST_COLLECT_QTEDGES_20180821
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunction(const Mode mode) noexcept
{
  switch (mode)
  {
    case Mode::edit: return GetQtNodeBrushFunctionEdit();
    case Mode::rate: return GetQtNodeBrushFunctionRate();
    default:
      assert(!"Should not get here"); //!OCLINT accepted idiom
    case Mode::uninitialized: return GetQtNodeBrushFunctionUninitialized();
  }
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunctionEdit() noexcept
{
  return [](const QtNode& qtnode)
  {
    //Gold if center node
    //Grey if no examples
    //Red if solitary node (that is, a concept) with examples
    //Blue if relation node with examples
    if (IsCenterNode(qtnode))
    {
      return QtBrushFactory().CreateGoldGradientBrush();
    }
    if (!HasExamples(qtnode))
    {
      return QtBrushFactory().CreateGrayGradientBrush();
    }
    if (IsOnEdge(qtnode))
    {
      return QtBrushFactory().CreateBlueGradientBrush();
    }
    assert(!IsOnEdge(qtnode));
    return QtBrushFactory().CreateRedGradientBrush();
  };
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeBrushFunctionRate() noexcept
{
  return [](const QtNode& qtnode)
  {
    if (IsCenterNode(qtnode))
    {
      return QtBrushFactory().CreateGoldGradientBrush();
    }
    if (IsOnEdge(qtnode))
    {
      return QtBrushFactory().CreateBlueGradientBrush();
    }
    const int n_rated
      = (GetRatingComplexity(qtnode)   == -1 ? 0 : 1)
      + (GetRatingConcreteness(qtnode) == -1 ? 0 : 1)
      + (GetRatingSpecificity(qtnode)  == -1 ? 0 : 1);
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

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetQtNodes(const QGraphicsScene& scene) noexcept
{
  const auto qtnodes_all = GetQtNodesAlsoOnQtEdge(scene);
  std::vector<QtNode*> qtnodes;
  std::copy_if(std::begin(qtnodes_all), std::end(qtnodes_all),
    std::back_inserter(qtnodes),
    [](const QtNode* const qtnode)
    {
      return !IsOnEdge(*qtnode);
    }
  );
  return qtnodes;
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetQtNodesAlsoOnQtEdge(const QGraphicsScene& scene) noexcept
{
  std::vector<QtNode*> qtnodes;
  for (QGraphicsItem * const item: scene.items())
  {
    if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
    {
      qtnodes.push_back(qtnode);
    }
  }
  return qtnodes;
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeVignetteBrushFunction(const Mode mode) noexcept
{
  switch (mode)
  {
    case Mode::edit: return GetQtNodeVignetteBrushFunctionEdit();
    case Mode::rate: return GetQtNodeVignetteBrushFunctionRate();
    case Mode::uninitialized: return GetQtNodeVignetteBrushFunctionUninitialized();
  }
  assert(!"Should not get here"); //!OCLINT accepted idiom
  return GetQtNodeVignetteBrushFunctionUninitialized();
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeVignetteBrushFunctionEdit() noexcept
{
  return [](const QtNode&) { return Qt::transparent; };
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeVignetteBrushFunctionRate() noexcept
{
  return [](const QtNode& qtnode)
  {
    if (IsOnEdge(qtnode))
    {
      return QBrush(Qt::transparent);
    }
    if (!HasExamples(qtnode))
    {
      //return QBrush(Qt::transparent);
      return QBrush(QColor(0, 0, 255));
    }
    const int n_rated = CountExamplesRated(GetExamples(qtnode));
    const int n_examples = CountExamples(GetExamples(qtnode));
    if (n_rated == n_examples) return QBrush(Qt::green);
    if (n_rated == 0) return QBrush(Qt::red);
    return QBrush(QColor(255, 128, 0));
  };
}

std::function<QBrush(const ribi::cmap::QtNode&)>
ribi::cmap::GetQtNodeVignetteBrushFunctionUninitialized() noexcept
{
  return [](const QtNode&) { return Qt::transparent; };
}

std::vector<ribi::cmap::QtEdge *>
ribi::cmap::GetSelectedQtEdges(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtEdge *> v;
  const auto selected = scene.selectedItems();
  for (const auto item: selected)
  {
    if (QtEdge* const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      v.push_back(qtedge);
    }
    if (QtNode* const qtnode = qgraphicsitem_cast<QtNode*>(item))
    {
      if (QtEdge* const qtedge_again
        = qgraphicsitem_cast<QtEdge*>(qtnode->parentItem())
      )
      {
        v.push_back(qtedge_again);
      }
    }
  }
  return v;
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetSelectedQtNodes(const QGraphicsScene& scene) noexcept
{
  std::vector<ribi::cmap::QtNode *> selected;
  const auto qtnodes = GetQtNodes(scene);
  std::copy_if(
    std::begin(qtnodes),
    std::end(qtnodes),
    std::back_inserter(selected),
    [](QtNode* const qtnode)
    {
      return qtnode->isSelected()
        && !IsOnEdge(*qtnode)
      ;
    }
  );
  return selected;
}

std::vector<ribi::cmap::QtNode *>
ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(const QGraphicsScene& scene) noexcept
{
  //All QtNodes, also those on QtEdge
  const auto qtnodes = GetQtNodesAlsoOnQtEdge(scene);
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
  const QtNode * const qtnode = qgraphicsitem_cast<const QtNode*>(item);
  if (!qtnode) return false;
  return IsQtCenterNode(*qtnode);
}

bool ribi::cmap::IsQtCenterNode(const QtNode& qtnode)
{
  return IsCenterNode(qtnode);
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
  const QGraphicsItem * const item
) noexcept
{
  const QtNode* const qtnode{qgraphicsitem_cast<const QtNode*>(item)};
  return qtnode && !IsOnEdge(*qtnode);
}

bool ribi::cmap::IsQtNodeOnEdge(
  const QGraphicsItem * const item
) noexcept
{
  const QtNode* const qtnode{qgraphicsitem_cast<const QtNode*>(item)};
  return qtnode && IsOnEdge(*qtnode);
}
