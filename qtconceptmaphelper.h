#ifndef QTCONCEPTMAPHELPER_H
#define QTCONCEPTMAPHELPER_H

#include <vector>
#include <QBrush>
#include "qtconceptmapfwd.h"
#include "qtconceptmapmode.h"
#include "conceptmap.h"

namespace ribi {
namespace cmap {

///Counts the number of arrow heads on all QtEdges
int CountQtArrowHeads(const QGraphicsScene& scene) noexcept;

///Counts the number of arrow heads on all QtEdges
int CountQtArrowTails(const QGraphicsScene& scene) noexcept;

///Counts the QtNodes that are center nodes
int CountQtCenterNodes(const QGraphicsScene& scene) noexcept;

///Counts the QtEdges
int CountQtEdges(const QGraphicsScene& scene) noexcept;

///Counts the QtNodes that are Nodes, i.e. are not on an edge
int CountQtNodes(const QGraphicsScene& scene) noexcept;

///Only counts the QtNodes that are not on an edge
int CountSelectedQtNodes(const QGraphicsScene& scene) noexcept;

///Counts selected QtNodes that are on a QtEdge
int CountSelectedQtEdges(const QGraphicsScene& scene) noexcept;

///Get the one selected QtEdge. Throws if there is not exactly one edge selected
QtEdge * ExtractTheOneSelectedQtEdge(const QGraphicsScene& scene);

///Finds the one QtCenterNode
///Returns nullptr if there is none, two or more
QtNode * FindQtCenterNode(const QGraphicsScene& scene) noexcept;

///Find the edge this QtNode is in the center of
///Returns nullptr if the QtNode is not on a QtEdge
QtEdge * FindQtEdge(
  const QtNode * const qtnode,
  const QGraphicsScene& scene
) noexcept;

///Finds the first QtNode in a QGraphicsScene with a matching node ID
///Returns nullptr if there is none
QtNode * FindQtNode(const int node_id, const QGraphicsScene& scene) noexcept;

///Find the first QtEdge
///Returns nullpt if there are no QtEdges in the scene
QtEdge * GetFirstQtEdge(const QGraphicsScene& scene) noexcept;

///Finds the first QtEdge in a QGraphicsScene by a predicate
///Returns nullptr if there is none
QtEdge * FindFirstQtEdge(
  const QGraphicsScene& scene,
  const std::function<bool(QtEdge*)> predicate) noexcept;

///Find the first QtNode
///Returns nullpt if there are no QtNodes in the scene
QtNode * GetFirstQtNode(const QGraphicsScene& scene) noexcept;

///Finds the first QtNode in a QGraphicsScene by a predicate
///Returns nullptr if there is none
QtNode * FindFirstQtNode(
  const QGraphicsScene& scene,
  const std::function<bool(QtNode*)> predicate) noexcept;

///Find the last QtEdge
///Returns nullpt if there are no QtEdges in the scene
QtEdge * GetLastQtEdge(const QGraphicsScene& scene) noexcept;

///Obtain the center node, if there is any.
///Will return nullptr is there is no center node.
QtNode * GetQtCenterNode(const QGraphicsScene& scene) noexcept;

///Get all the (standalone) center QtNodes, should usually be zero or one nodes
std::vector<QtNode *> GetQtCenterNodes(const QGraphicsScene& scene) noexcept;

///Get all the QtEdges
std::vector<QtEdge *> GetQtEdges(const QGraphicsScene& scene) noexcept;

///Get all the edges connected to the concept
std::vector<QtEdge*> GetQtEdges(
  const QtNode * const from,
  const QGraphicsScene& scene
) noexcept;

///The function how a QtNode determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunction(
  const Mode mode
) noexcept;

///The function how a QtNode in Mode::Edit determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunctionEdit() noexcept;

///The function how a QtNode in Mode::Rate determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunctionRate() noexcept;

///The function how a QtNode in Mode::Unitialized determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeBrushFunctionUninitialized() noexcept;

///Get all the 'standalone' QtNodes
std::vector<QtNode *> GetQtNodes(const QGraphicsScene& scene) noexcept;

///Get both the QtNodes that are 'standalone' or are on an edge
std::vector<QtNode *> GetQtNodesAlsoOnQtEdge(const QGraphicsScene& scene) noexcept;

///The function how a QtNode's vignette determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeVignetteBrushFunction(
  const Mode mode
) noexcept;

///The function how a QtNode in Mode::Edit determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeVignetteBrushFunctionEdit() noexcept;

///The function how a QtNode in Mode::Rate determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeVignetteBrushFunctionRate() noexcept;

///The function how a QtNode in Mode::Unitialized determines it is colored
std::function<QBrush(const QtNode&)> GetQtNodeVignetteBrushFunctionUninitialized() noexcept;

///Get the selected QtEdges
std::vector<QtEdge *> GetSelectedQtEdges(const QGraphicsScene& scene) noexcept;

///Get all the selected 'standalone' QtNodes; QtNodes that QtEdge can connect to;
std::vector<QtNode *> GetSelectedQtNodes(const QGraphicsScene& scene) noexcept;

///Get all the selected QtNodes, also those on a QtEdge
std::vector<QtNode *> GetSelectedQtNodesAlsoOnQtEdge(const QGraphicsScene& scene) noexcept;

///Does the pen draw dashed lines?
bool IsDashed(const QPen& pen) noexcept;

///Check if this item is the center node
bool IsQtCenterNode(const QGraphicsItem* const item);
bool IsQtCenterNode(const QtNode& qtnode);

///Is there a QtCenterNode among the selected items?
bool IsQtCenterNodeSelected(const QGraphicsScene& scene);

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPHELPER_H
