#ifndef QTCONCEPTMAPCONCEPTMAP_H
#define QTCONCEPTMAPCONCEPTMAP_H

#include <iosfwd>
#include <QGraphicsItem>
#include <QUndoStack>
#include "qtkeyboardfriendlygraphicsview.h"
#include "qtconceptmapfwd.h"
#include "qtconceptmapmode.h"
#include "conceptmapgraphtypes.h"
#include "qtconceptmaprating.h"

namespace ribi {
namespace cmap {

///QtConceptMap displays a ConceptMap
///It does not offer UI interaction with the user
///QtConceptMap does offer UI interaction
///Cannot mark it as a final class, because that would break Qt4 compatibility
///The Z values of its objects are set as such:
///  3: QtTool: GetQtToolZvalue()
///  2: QtNewArrow: GetQtNewArrowZvalue()
///  0: QtNodes: GetQtNodeZvalue()
/// -1: QtEdges: GetQtEdgeZvalue()
/// -2: QtEdge's arrow: GetQtEdgeArrowZvalue()`
class QtConceptMap final //!OCLINT cannot get it smaller
  : public ribi::QtKeyboardFriendlyGraphicsView
{
  Q_OBJECT //!OCLINT

public:

  explicit QtConceptMap(
    const Rating& rating = CreateDefaultRating(),
    QWidget* parent = nullptr
  );
  QtConceptMap(const QtConceptMap&) = delete;
  QtConceptMap& operator=(const QtConceptMap&) = delete;
  ~QtConceptMap() noexcept;

  ///Raw pointer, because ConceptMap its QUndoStack will take over ownership of pointer
  ///Will throw if the command cannot be done, for example, if
  ///an item with a name that is absent in the concept map is moved
  void DoCommand(Command * const command);


  ///Get the mode the object is in
  Mode GetMode() const noexcept { return m_mode; }

  const auto& GetRating() const noexcept { return m_rating; }

  ///Get the QGraphicsItem that can highlight selected QGraphicsItems
  const QtItemHighlighter& GetQtHighlighter() const noexcept;

  ///Get the QGraphicsItem that can highlight selected QGraphicsItems
  QtItemHighlighter& GetQtHighlighter() noexcept;

  ///Get the arrow that must be clicked to add a new edge
  const QtNewArrow& GetQtNewArrow() const noexcept;

  ///Get the arrow that must be clicked to add a new edge
  QtNewArrow& GetQtNewArrow()       noexcept;

  ///The arrow that must be clicked to add a new edge
  const QtTool& GetQtToolItem() const noexcept;
        QtTool& GetQtToolItem()       noexcept;

  const QUndoStack& GetUndo() const noexcept { return m_undo; }
        QUndoStack& GetUndo()       noexcept { return m_undo; }


  void Redo() noexcept;

  void SetConceptMap(const ConceptMap& conceptmap);

  void SetMode(const Mode mode) noexcept;

  ///Restart the timer
  void StartTimer();

  ///Stop the timer
  void StopTimer();

  ///Convert to concept map
  ConceptMap ToConceptMap() const noexcept;

  ///Will throw if there is nothing to be undone
  void Undo();

public slots:

  void changeEvent(QEvent *) override;
  void focusInEvent(QFocusEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void hideEvent(QHideEvent *) override;
  void keyPressEvent(QKeyEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent * event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  ///No override, this is the slot called
  ///whenever a GraphicsScene::onFocusItemChanged signal
  ///is emitted
  void OnFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason);

  void showEvent(QShowEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private slots:

  ///A timed event to check for collisions, update QtToolItem
  void Respond();

private:

  ///The arrow used to create a new arrow
  ///Is hidden when not active
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtNewArrow * const m_arrow;

  ///The item highlighter, used when creating a new relation
  ///Must be a pointer, as it will be owned by QGraphicsScene
  QtItemHighlighter * const m_highlighter;

  ///The type of concept map: plain (best for printing),
  /// edit (modify the graph) or rate (only grade the existing nodes)
  Mode m_mode;

  ///Will be the QGraphicsScene coordinat.
  ///Will have size 1 at the moment the mouse is pressed
  ///Will be empty of the mouse has been released
  std::vector<QPointF> m_last_mouse_click_pos;

  ///The suggested rating of the concepts
  const Rating m_rating;

  ///Responds to many things
  QTimer * const m_timer;

  ///The item showing the tools
  QtTool * const m_tools;

  QUndoStack m_undo;
};

void AddEdgeToScene(QtConceptMap& qtconceptmap, const EdgeDescriptor ed) noexcept;
void AddEdgesToScene(QtConceptMap& qtconceptmap, const ConceptMap& conceptmap) noexcept;

void AddNodesToScene(QtConceptMap& qtconceptmap, const ConceptMap& conceptmap) noexcept;

///Checks if the QtConceptMap is in a valid state
void CheckInvariants(const QtConceptMap& q) noexcept;

///All QtEdges must have a QScene
void CheckInvariantAllQtEdgesHaveAscene(const QtConceptMap& q) noexcept;

///All QtNodes must have a QScene
void CheckInvariantAllQtNodesHaveAscene(const QtConceptMap& q) noexcept;

///There must be no left-alone quad bezier arrows
void CheckInvariantNoLonelyQuadBezierArrows(const QtConceptMap& q) noexcept;

///There must be no QGraphicsItem that cannot be cast to known
///and expected types
void CheckInvariantNoUnknownItems(const QtConceptMap& q) noexcept;

///A QtToolItem is never associated with a QtEdge, nor a QtNode
///one a QtEdge
void CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(const QtConceptMap& q) noexcept;

///If there is a single QtEdge selected, its corresponding Edge must be present
void CheckInvariantSingleSelectQtEdgeMustHaveCorrespondingEdge(const QtConceptMap& q) noexcept;

///Counts the number of arrow heads on all QtEdges
int CountQtArrowHeads(const QtConceptMap& q) noexcept;

///Counts the number of arrow tails on all QtEdges
int CountQtArrowTails(const QtConceptMap& q) noexcept;

///Counts the QtNodes that are center nodes
int CountQtCenterNodes(const QtConceptMap& q) noexcept;

///Counts the QtEdges
int CountQtEdges(const QtConceptMap& q) noexcept;

///Counts the QtNodes that are Nodes, i.e. are not on an edge
int CountQtNodes(const QtConceptMap& q) noexcept;

int CountSelectedQtNodes(const QtConceptMap& q) noexcept;

int CountSelectedQtEdges(const QtConceptMap& q) noexcept;

///Create the flags for a QtNode in a certain Mode
QGraphicsItem::GraphicsItemFlags CreateFlags(const QtNode& qtnode, const Mode mode) noexcept;

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

///Find the edge this QtNode is in the center of a QtEdge.
///Returns nullptr if the QtNode is not on a QtEdge
QtEdge * FindQtEdge(
  const QtNode * const qtnode,
  const QtConceptMap& qtconceptmap
) noexcept;

///Get the first QtEdge
///Returns nullpt if there are no QtEdges in the scene
QtEdge * GetFirstQtEdge(const QtConceptMap& q) noexcept;

///Find the first QtNode
///Returns nullpt if there are no QtNodes in the scene
QtNode * GetFirstQtNode(const QtConceptMap& q) noexcept;

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

///Obtain the center node, if there is any.
///Will return nullptr is there is no center node.
QtNode * GetQtCenterNode(const QtConceptMap& q) noexcept;

///Get all the QtEdges
std::vector<QtEdge *> GetQtEdges(const QtConceptMap& q) noexcept;

///Get all the 'standalone' (those not on an edge) QtNodes
std::vector<QtNode *> GetQtNodes(const QtConceptMap& q) noexcept;

///Get both the QtNodes that are 'standalone' or are on an edge
std::vector<QtNode *> GetQtNodesAlsoOnQtEdge(const QtConceptMap& q) noexcept;

///Get the QtToolItem its Buddy item
///Will return a QtNode that is either standalone or on an edge
QtNode * GetQtToolItemBuddy(const QtConceptMap& q) noexcept;

///Get the selected QtEdges
std::vector<QtEdge *> GetSelectedQtEdges(const QtConceptMap& q) noexcept;

///Get all the selected 'standalone' QtNodes; QtNodes that QtEdge can connect to;
std::vector<QtNode *> GetSelectedQtNodes(const QtConceptMap& q) noexcept;

///Get all the selected QtNodes, also those on a QtEdge
std::vector<QtNode *> GetSelectedQtNodesAlsoOnQtEdge(const QtConceptMap& q) noexcept;

///Checks if the qtedge has its QGraphicsScenes in place
///scene can be nullptr
bool HasScene(const QtEdge& qtedge, const QGraphicsScene * const scene) noexcept;

///Returns true if there is one QGraphicsItem selected
bool HasSelectedItems(const QtConceptMap& q) noexcept;

///Is the arrow to connect a select QtNode to a potential other
///QtNode (to draw an edge) visible?
bool IsArrowVisible(QtConceptMap& q) noexcept;

///Is the QtEdge in the QGraphicsScene?
bool IsInScene(const QtEdge& qtedge, const QGraphicsScene& scene) noexcept;

///Is this QtNode in the center on a QtEdge?
bool IsOnEdge(const QtNode& qtnode) noexcept;

///Is there a QtCenterNode among the selected items?
bool IsQtCenterNodeSelected(const QtConceptMap& q);

///Is this QGraphicsItem an autonomous QtNode, that is, a QtNode not on an edge?
bool IsQtNodeNotOnEdge(
  const QGraphicsItem * const item
) noexcept;

///Is this QGraphicsItem an QtNode on an edge, instead of an autonomous QtNode?
bool IsQtNodeOnEdge(
  const QGraphicsItem * const item
) noexcept;

///
void keyPressEventArrows(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventArrowsMove(QtConceptMap& q, QKeyEvent *event) noexcept;

///Can be additive (with Shift pressed) and exclusive (without shift pressed)
void keyPressEventArrowsSelect(QtConceptMap& q, QKeyEvent *event) noexcept;

///1: Toggle arrow tail
void keyPressEvent1(QtConceptMap& q, QKeyEvent * const event) noexcept;

///2:Toggle arrow head
void keyPressEvent2(QtConceptMap& q, QKeyEvent * const event) noexcept;

void keyPressEventDelete(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventE(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventEscape(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventF1(QtConceptMap& q, QKeyEvent * const event) noexcept;
void keyPressEventF2(QtConceptMap& q, QKeyEvent * const event) noexcept;
void keyPressEventF4(QtConceptMap& q, QKeyEvent *event) noexcept;

///CTRL + H: Toggle arrow head
void keyPressEventH(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventN(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventSpace(QtConceptMap& q, QKeyEvent *) noexcept;

///CTRL + T: Toggle arrow tail
void keyPressEventT(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventZ(QtConceptMap& q, QKeyEvent *event) noexcept;

///The default handler of a mouse press event, when there is no arrow
void mousePressEventNoArrowActive(QtConceptMap& q, QMouseEvent *event) noexcept;

///The handler of a mouse press event, when the arrow is active
/// (it points from a selected QtNode to a potential other
/// QtNode to draw an edge between)
void mousePressEventArrowActive(QtConceptMap& q, QMouseEvent *event) noexcept;

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
  QtNode& item,
  QKeyEvent * const event
);

///An item wants to be edited from F2 in edit mode
void OnNodeKeyDownPressedEditF2(
  QtConceptMap& q,
  QtNode& qtnode,
  QKeyEvent * const event
);

///An item wants to be edited from F1 in rate mode
void OnNodeKeyDownPressedRateF1(
  QtConceptMap& q,
  QtNode& item,
  QKeyEvent * const event
);

///An item wants to be rated from F2 in rate mode
void OnNodeKeyDownPressedRateF2(
  QtConceptMap& q,
  QtNode& item,
  QKeyEvent * const event
);

void ProcessKey(QtConceptMap& q, QKeyEvent * const event);

///Remove all Qt and non-Qt items
void RemoveConceptMap(QtConceptMap& q);

///Save the QtConceptMap to a .dot file
void SaveToFile(const QtConceptMap& q, const std::string& dot_filename);

///Select the QtNode, including updating the QtToolItem
void Select(QtConceptMap& q, QtEdge& qtedge);

///Select the QtNode, including updating the QtToolItem
void Select(QtConceptMap& q, QtNode& qtnode);

///Set focus to the QtNode. Assumes the QtConceptMap has no focus anymore
void SetFocus(QtConceptMap& q, QtNode* const qtnode);

///Set the buddy of the QtToolItem
void SetQtToolItemBuddy(QtConceptMap& q, QtNode * const qtnode);

///Focus on a random QtNode (both as vertices as those on the edges)
/// @param keep_old_selection if false, all previous selection loses focus
void SetRandomFocus(QtConceptMap& q, const bool keep_old_selection);

///Set a new random focus, keep the older items selected
void SetRandomFocusAdditive(QtConceptMap& q);

///Set a new random focus, unselect the previously selected items
void SetRandomFocusExclusive(QtConceptMap& q);

///Set the selectedness of a QtEdge, also updating the
///selectness of the Edge in the QtConceptMap.
///Use 'Select' to also update the QtToolItem
void SetSelectedness(
  const bool is_selected,
  QtEdge& qtedge
);

///Set the selectedness of a QtNode, also updating the
///selectness of the node in the ConceptMap.
///Use 'Select' to also update the QtToolItem
void SetSelectedness(
  const bool is_selected,
  QtNode& qtnode
);

///Unselect the QtEdge, including updating the QtToolItem
void Unselect(QtConceptMap& q, QtEdge& qtedge);

///Unselect the QtNode, including updating the QtToolItem
void Unselect(QtConceptMap& q, QtNode& qtnode);

std::ostream& operator<<(std::ostream& os, const QtConceptMap& c) noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCONCEPTMAP_H
