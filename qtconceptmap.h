#ifndef QTCONCEPTMAPCONCEPTMAP_H
#define QTCONCEPTMAPCONCEPTMAP_H

#include <functional>
#include <QUndoStack>
#include "qtkeyboardfriendlygraphicsview.h"
#include "qtconceptmapfwd.h"
#include "qtconceptmapmode.h"
#include "qtconceptmappopupmode.h"
#include "qtconceptmapqtedge.h"
#include "conceptmap.h"

namespace ribi {
namespace cmap {

///QtConceptMap displays a ConceptMap
///It does not offer UI interaction with the user
///QtConceptMap does offer UI interaction
///Cannot mark it as a final class, because that would break Qt4 compatibility
///The Z values of its objects are set as such:
///  3: QtTool
///  2: QtNewArrow
///  0: QtNodes
/// -1: QtEdges
class QtConceptMap : public ribi::QtKeyboardFriendlyGraphicsView //!OCLINT cannot get it smaller
{
  Q_OBJECT //!OCLINT

public:

  explicit QtConceptMap(QWidget* parent = 0);
  QtConceptMap(const QtConceptMap&) = delete;
  QtConceptMap& operator=(const QtConceptMap&) = delete;
  ~QtConceptMap() noexcept;

  ///Raw pointer, because ConceptMap its QUndoStack will take over ownership of pointer
  ///Will throw if the command cannot be done, for example, if
  ///an item with a name that is absent in the concept map is moved
  void DoCommand(Command * const command);

  ///Obtain the concept map
  const ConceptMap& GetConceptMap() const noexcept { return m_conceptmap; }
        ConceptMap& GetConceptMap()       noexcept { return m_conceptmap; }

  Mode GetMode() const noexcept { return m_mode; }

  PopupMode GetPopupMode() const noexcept { return m_popup_mode; }

  ///The square showing the examples
  const QtExamplesItem& GetQtExamplesItem() const noexcept;
        QtExamplesItem& GetQtExamplesItem()       noexcept;

  ///The QGraphicsItem that can highlight selected QGraphicsItems
  const QtItemHighlighter& GetQtHighlighter() const noexcept;
        QtItemHighlighter& GetQtHighlighter()       noexcept;

  ///The arrow that must be clicked to add a new edge
  const QtNewArrow& GetQtNewArrow() const noexcept;
        QtNewArrow& GetQtNewArrow()       noexcept;

  ///The arrow that must be clicked to add a new edge
  const QtTool& GetQtToolItem() const noexcept;
        QtTool& GetQtToolItem()       noexcept;

  const QUndoStack& GetUndo() const noexcept { return m_undo; }
        QUndoStack& GetUndo()       noexcept { return m_undo; }


  void Redo() noexcept;

  void SetConceptMap(const ConceptMap& conceptmap);

  void SetPopupMode(const PopupMode mode) noexcept;

  void SetMode(const Mode mode) noexcept;

  ///Restart the timer
  void StartTimer();

  ///Stop the timer
  void StopTimer();

  ///Will throw if there is nothing to be undone
  void Undo();

public slots:

  void changeEvent(QEvent *) override;
  void hideEvent(QHideEvent *) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mouseMoveEvent(QMouseEvent * event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason);
  void onSelectionChanged();
  void showEvent(QShowEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private slots:

  ///A timed event to check for collisions, update QtExamplesItem and QtToolItem
  void Respond();

private:

  ///The arrow used to create a new arrow
  ///Is hidden when not active
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtNewArrow * const m_arrow;

  ///The concept map to work on, the Model
  ConceptMap m_conceptmap;

  ///The item showing the examples
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtExamplesItem * const m_examples_item;

  ///The item highlighter, used when creating a new relation
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtItemHighlighter * const m_highlighter;

  ///The type of concept map: plain (best for printing),
  /// edit (modify the graph) or rate (only grade the existing nodes)
  Mode m_mode;

  ///The type of pop ups: absent (best for printing) or present (best for UX)
  PopupMode m_popup_mode;

  ///Responds to many things
  QTimer * const m_timer;

  ///The item showing the tools
  QtTool * const m_tools;

  QUndoStack m_undo;
};

void AddEdgeToScene(QtConceptMap& qtconceptmap, const EdgeDescriptor ed) noexcept;
void AddEdgesToScene(QtConceptMap& qtconceptmap) noexcept;

void AddNodesToScene(QtConceptMap& qtconceptmap) noexcept;

///Add a QtNode to the scene, add its Node to the ConceptMap
///This is used by, among others, CommandDeleteSelected::undo
void AddQtEdge(
  QtEdge * const qtedge,
  QtConceptMap& q
);

///Add a QtNode to the scene, add its Node to the ConceptMap
///This is used by, among others, CommandDeleteSelected::undo
void AddQtNode(
  QtNode * const qtnode,
  QtConceptMap& q
);

///Checks if the QtConceptMap is in a valid state
void CheckInvariants(const QtConceptMap& q) noexcept;

///All QtEdges must have a QScene
void CheckInvariantAllQtEdgesHaveAscene(const QtConceptMap& q) noexcept;

///All QtNodes must have a QScene
void CheckInvariantAllQtNodesHaveAscene(const QtConceptMap& q) noexcept;

///There must be as much Edges as QtEdges
void CheckInvariantAsMuchEdgesAsQtEdges(const QtConceptMap& q) noexcept;

///There must be as much Nodes as QtNodes
void CheckInvariantAsMuchNodesAsQtNodes(const QtConceptMap& q) noexcept;

///There must be as much Nodes as QtNodes selected
void CheckInvariantAsMuchNodesAsQtNodesSelected(const QtConceptMap& q) noexcept;

///If one QtNode with examples is selected, the ExamplesItem must be visible and close
void CheckInvariantOneQtNodeWithExamplesHasExamplesItem(const QtConceptMap& q) noexcept;

///All QtEdges and Edges must have approximately the same X and Y coordinat
void CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(const QtConceptMap& q) noexcept;

///All QtNodes and Nodes must have approximately the same X and Y coordinat
void CheckInvariantQtNodesAndNodesHaveSameCoordinats(const QtConceptMap& q) noexcept;

///If there is a single QtEdge selected, its corresponding Edge must be present
void CheckInvariantSingleSelectQtEdgeMustHaveCorrespondingEdge(const QtConceptMap& q) noexcept;

///If there is a single QtNode selected (may also be on a QtNode on a QtEdge),
///the QtToolItem must be connected to it
void CheckInvariantSingleSelectedQtNodeMustHaveQtTool(const QtConceptMap& q) noexcept;

///Counts the QtEdges
int CountQtEdges(const QtConceptMap& q) noexcept;

///Counts the QtNodes that are Nodes, i.e. are not on an edge
int CountQtNodes(const QtConceptMap& q) noexcept;

int CountSelectedQtNodes(const QtConceptMap& q) noexcept;

int CountSelectedQtEdges(const QtConceptMap& q) noexcept;

///Finds the first QtEdge by a predicate
///Returns nullptr if there is none
QtEdge * FindFirstQtEdge(
  const QtConceptMap& q,
  const std::function<bool(QtEdge*)> predicate) noexcept;


///Finds the first QtNode by a predicate
///Returns nullptr if there is none
QtNode * FindFirstQtNode(
  const QtConceptMap& q,
  const std::function<bool(QtNode*)> predicate) noexcept;

///Finds the first QtNode in a QGraphicsScene with a matching node ID
///Returns nullptr if there is none
QtNode * FindQtNode(const int node_id, const QtConceptMap& q) noexcept;

///Get all the items that can be focused and selected on
///This also depends on the mode: if the user is rating a concept map,
///the center node is not selectable.
std::vector<QGraphicsItem *> GetFocusableItems(const QtConceptMap& q);

///Get all the items that can be focused and selected on
///This also depends on the mode: if the user is rating a concept map,
///the center node is not selectable.
std::vector<QGraphicsItem *> GetFocusableNonselectedItems(const QtConceptMap& q);

///Obtain the first QtNode under the cursor
///Returns nullptr if none is present
QtNode* GetItemBelowCursor(const QtConceptMap& q, const QPointF& pos) noexcept;

///Get all the QtEdges
std::vector<QtEdge *> GetQtEdges(const QtConceptMap& q) noexcept;

constexpr double GetQtEdgeZvalue() { return -1.0; }

///Get all the 'standalone' (those not on an edge) QtNodes
std::vector<QtNode *> GetQtNodes(const QtConceptMap& q) noexcept;

///Get both the QtNodes that are 'standalone' or are on an edge
std::vector<QtNode *> GetQtNodesAlsoOnQtEdge(const QtConceptMap& q) noexcept;

///Get the selected QtEdges
std::vector<QtEdge *> GetSelectedQtEdges(const QtConceptMap& q) noexcept;

///Get all the selected 'standalone' QtNodes; QtNodes that QtEdge can connect to;
std::vector<QtNode *> GetSelectedQtNodes(const QtConceptMap& q) noexcept;

///Get all the selected QtNodes, also those on a QtEdge
std::vector<QtNode *> GetSelectedQtNodesAlsoOnQtEdge(const QtConceptMap& q) noexcept;

///Checks if the qtedge has its QGraphicsScenes in place
///scene can be nullptr
bool HasScene(const QtEdge& qtedge, const QGraphicsScene * const scene) noexcept;

///Hide the QtExamplesItem
void HideExamplesItem(QtConceptMap& q) noexcept;

///Is the QtEdge in the QGraphicsScene?
bool IsInScene(const QtEdge& qtedge, const QGraphicsScene& scene) noexcept;

///Is this QtNode in the center on a QtEdge?
bool IsOnEdge(const QtNode& qtnode, const QtConceptMap& q) noexcept;

///Is there a QtCenterNode among the selected items?
bool IsQtCenterNodeSelected(const QtConceptMap& q);

///Is this QGraphicsItem an autonomous QtNode, that is, a QtNode not on an edge?
bool IsQtNodeNotOnEdge(
  const QGraphicsItem * const item,
  const QtConceptMap& q
) noexcept;

///Is this QGraphicsItem an QtNode on an edge, instead of an autonomous QtNode?
bool IsQtNodeOnEdge(
  const QGraphicsItem * const item,
  const QtConceptMap& q
) noexcept;

void keyPressEventDelete(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventE(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventEscape(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventF1(QtConceptMap& q, QKeyEvent * const event) noexcept;
void keyPressEventF2(QtConceptMap& q, QKeyEvent * const event) noexcept;
void keyPressEventF4(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventH(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventN(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventQuestion(QtConceptMap& q, QKeyEvent *) noexcept;
void keyPressEventSpace(QtConceptMap& q, QKeyEvent *) noexcept;
void keyPressEventT(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventZ(QtConceptMap& q, QKeyEvent *event) noexcept;


///Messes up the scene, by moving items, etc
void MoveQtEdgesAndQtNodesRandomly(QtConceptMap& q);

///Move the QtEdge (the view) and its Edge (the model)
void MoveQtEdge(QtEdge& qtedge, const double dx, const double dy, QtConceptMap& q);

///Move the QtEdge (the view) and its Edge (the model)
void MoveQtNode(QtNode& qtnode, const double dx, const double dy, QtConceptMap& q);

///Checks for QtNodes that are colliding and move these
///away from each other
void MoveQtNodesAwayFromEachOther(QtConceptMap& q) noexcept;


///Called when an item wants to be edited
void OnEdgeKeyDownPressed(QtConceptMap& q, QtEdge * const item, const int key);

///Called when an item wants to be edited
void OnNodeKeyDownPressed(
  QtConceptMap& q,
  QtNode* const item,
  QKeyEvent * const event
);

///An item wants to be edited from F2 in edit mode
void OnNodeKeyDownPressedEditF2(
  QtConceptMap& q,
  QtNode* const item,
  QKeyEvent * const event
);

///An item wants to be edited from F1 in rate mode
void OnNodeKeyDownPressedRateF1(QtConceptMap& q, QtNode* const item);

///An item wants to be edited from F2 in rate mode
void OnNodeKeyDownPressedRateF2(QtConceptMap& q, QtNode* const item);

void ProcessKey(QtConceptMap& q, QKeyEvent * const event);

///Remove all Qt and non-Qt items
void RemoveConceptMap(QtConceptMap& q);

///Save the QtConceptMap to a .dot file
void SaveToFile(const QtConceptMap& q, const std::string& dot_filename);

///Set focus to the QtNode. Assumes the QtConceptMap has no focus anymore
void SetFocus(QtConceptMap& q, QtNode* const qtnode);

///Set the buddy of the QtExamplesItem
void SetQtExamplesBuddy(QtConceptMap& q, QtNode * const qtnode);
void SetQtExamplesBuddy(QtConceptMap& q, QtEdge * const qtedge);

///Set the buddy of the QtToolItem
void SetQtToolItemBuddy(QtConceptMap& q, QtNode * const qtnode);
void SetQtToolItemBuddy(QtConceptMap& q, QtEdge * const qtedge);

///Focus on a random QtNode (both as vertices as those on the edges)
/// @param keep_old_selection if false, all previous selection loses focus
void SetRandomFocus(QtConceptMap& q, const bool keep_old_selection);

///Set a new random focus, keep the older items selected
void SetRandomFocusAdditive(QtConceptMap& q);

///Set a new random focus, unselect the previously selected items
void SetRandomFocusExclusive(QtConceptMap& q);

///Set the selectedness of a QtEdge, also updating the
///selectness of the Edge in the ConceptMap
void SetSelectedness(
  const bool is_selected,
  QtEdge& qtedge,
  QtConceptMap& q
);

///Set the selectedness of a QtNode, also updating the
///selectness of the node in the ConceptMap
void SetSelectedness(
  const bool is_selected,
  QtNode& qtnode,
  QtConceptMap& q
);

///Unselect all QtEdges (and the Edges in the ConceptMap)
void UnselectAllQtEdges(QtConceptMap& q);

///Unselect all QtNodes (and the Nodes in the ConceptMap)
void UnselectAllQtNodes(QtConceptMap& q);

/// Writes the selecteness of the QtConceptMap
/// to the ConceptMap
void UpdateConceptMap(QtConceptMap& q);

///The QtExamplesItem must be shown on nodes with examples
void UpdateExamplesItem(QtConceptMap& q);

///The QtToolItem must be shown on nodes
void UpdateQtToolItem(QtConceptMap& q);


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTMAP_H
