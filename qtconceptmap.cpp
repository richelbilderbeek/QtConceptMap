#include "qtconceptmap.h"

#include <QDebug>
#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsView>
#include <gsl/gsl_assert>

#include "count_if_bundled_vertex.h"
#include "create_direct_neighbour_bundled_edges_and_vertices_subgraph.h"
#include "find_if_first_bundled_vertex.h"
#include "qtconceptmapcommand.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommanddeleteselected.h"
#include "qtconceptmapcommandmoveedge.h"
#include "qtconceptmapcommandmovenode.h"
#include "qtconceptmapcommandselect.h"
#include "qtconceptmapcommandsetconcept.h"
#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapcommandunselect.h"
#include "qtconceptmapcommandunselectall.h"
#include "qtconceptmapeditconceptdialog.h"
#include "qtconceptmaphelper.h"
#include "qtarrowitem.h"
#include "qtquadbezierarrowitem.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmapnewarrow.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaprateconceptdialog.h"
#include "qtconceptmapclassifyexamplesdialog.h"
#include "qtconceptmaptoolsitem.h"
#include "qtquadbezierarrowitem.h"

ribi::cmap::QtConceptMap::QtConceptMap(
  const Rating& rating,
  QWidget* parent
)
  : QtKeyboardFriendlyGraphicsView(parent),
    m_arrow{new QtNewArrow},
    m_highlighter{new QtItemHighlighter},
    m_mode{Mode::uninitialized},
    m_rating{rating},
    m_timer{new QTimer(this)},
    m_tools{new QtTool}
{
  static_assert(GetQtNewArrowZvalue() < GetQtToolZvalue(), "");
  static_assert(GetQtNodeZvalue() < GetQtNewArrowZvalue(), "");
  static_assert(GetQtEdgeZvalue() < GetQtNodeZvalue(), "");
  static_assert(GetQtEdgeArrowZvalue() <  GetQtEdgeZvalue(), "");

  this->setScene(new QGraphicsScene(this));
  assert(!m_highlighter->GetItem());

  //Add items
  assert(scene());
  scene()->addItem(m_arrow); //Add the QtNewArrow so it has a parent
  m_arrow->hide();

  //Without this line, mouseMoveEvent won't be called
  this->setMouseTracking(true);

  //Allow dragging and dropping
  this->setAcceptDrops(true);

  //Grey
  {
    QLinearGradient linearGradient(-500, -500, 500, 500);
    linearGradient.setColorAt(0.0, QColor(214, 214, 214));
    linearGradient.setColorAt(1.0, QColor(255, 255, 255));
    scene()->setBackgroundBrush(linearGradient);
    //this->scene()->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
  }

  //Connect the scene to respond to focus events
  QObject::connect(
    scene(),
    SIGNAL(focusItemChanged(QGraphicsItem*, QGraphicsItem*, Qt::FocusReason)),
    this,
    SLOT(OnFocusItemChanged(QGraphicsItem*, QGraphicsItem*, Qt::FocusReason))
  );
  //QObject::connect(scene(),SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));

  CheckInvariants(*this);

  QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(Respond()));
  //Timer is started in showEvent

  CheckInvariants(*this);

  this->setViewportUpdateMode(FullViewportUpdate);
}

ribi::cmap::QtConceptMap::~QtConceptMap() noexcept
{
  m_timer->stop();
  m_highlighter->Stop();
  m_undo.clear();
  m_highlighter->SetItem(nullptr); //Do this before destroying items
  m_tools->SetBuddyItem(nullptr);
  delete m_tools;
  delete m_arrow;
}

void ribi::cmap::AddEdgesToScene(
  QtConceptMap& qtconceptmap,
  const ConceptMap& conceptmap
) noexcept
{
  QGraphicsScene& scene = *qtconceptmap.scene();

  //Add the Edges
  const auto eip = edges(conceptmap);
  for(auto i = eip.first; i != eip.second; ++i)
  {
    const std::pair<Node, Node> from_to = GetFromTo(*i, conceptmap);
    const Node& from{from_to.first};
    const Node& to{from_to.second};
    assert(from.GetId() != to.GetId());
    assert(CountQtNodes(qtconceptmap) >= 2);
    QtNode * const qtfrom = FindFirstQtNode(qtconceptmap, QtNodeHasId(from.GetId()));
    QtNode * const qtto = FindFirstQtNode(qtconceptmap, QtNodeHasId(to.GetId()));
    assert(qtfrom);
    assert(qtto);
    assert(qtfrom != qtto);
    const Edge& edge = conceptmap[*i];
    QtEdge * const qtedge{
      new QtEdge(
        edge,
        qtfrom,
        qtto
      )
    };
    assert(edge.GetId() == qtedge->GetId());
    assert(std::abs(GetX(*qtedge) - GetX(edge)) < 2.0);
    assert(std::abs(GetY(*qtedge) - GetY(edge)) < 2.0);
    //Edges connected to the center node must be smaller
    if (IsConnectedToCenterNode(*qtedge))
    {
      qtedge->GetQtNode()->SetInnerWidth(8);
      qtedge->GetQtNode()->SetInnerHeight(8);
    }
    #ifdef STRAIGHT_LINES_BETWEEN_CENTER_NODE_AND_PRIMARY_CONCEPTS
    if (IsConnectedToCenterNode(*qtedge))
    {
      qtedge->GetQtNode()->setVisible(false);
    }
    #endif // STRAIGHT_LINES_BETWEEN_CENTER_NODE_AND_PRIMARY_CONCEPTS
    assert(qtedge && HasScene(*qtedge, nullptr));
    scene.addItem(qtedge);
    assert(HasScene(*qtedge, &scene));
    assert(std::abs(GetX(*qtedge) - GetX(edge)) < 2.0);
    assert(std::abs(GetY(*qtedge) - GetY(edge)) < 2.0);
    CheckInvariants(*qtedge);
  }
}

void ribi::cmap::AddNodesToScene(
  QtConceptMap& qtconceptmap,
  const ConceptMap& conceptmap
) noexcept
{
  assert(qtconceptmap.scene());
  QGraphicsScene& scene = *qtconceptmap.scene();

  //Add the nodes to the scene, if there are any
  const auto vip = vertices(conceptmap);
  for(auto i = vip.first; i!=vip.second; ++i)
  {
    assert(boost::num_vertices(conceptmap));
    const Node& node = conceptmap[*i];
    QtNode * const qtnode{new QtNode(node)};
    assert(qtnode);
    assert(node.GetId() == qtnode->GetId());
    assert(!qtnode->scene());
    scene.addItem(qtnode);
    assert(qtnode->scene());
    assert(FindFirstQtNode(qtconceptmap, QtNodeHasId(node.GetId())));
  }
}

void ribi::cmap::QtConceptMap::changeEvent(QEvent * event)
{
  CheckInvariants(*this);
  if (event->type() == QEvent::EnabledChange)
  {
    if (isEnabled())
    {
      this->StartTimer();
    }
    else
    {
      this->StopTimer();
    }
  }
  CheckInvariants(*this);
}

void ribi::cmap::CheckInvariantAllQtEdgesHaveAscene( //!OCLINT I think the cyclomatic complexity is acceptable here
  const QtConceptMap&
  #ifndef NDEBUG
    q
  #endif
) noexcept
{

  #ifndef NDEBUG
  //All QtEdges, their QtNodes and Arrows must have a scene
  for (const auto qtedge: GetQtEdges(*q.scene()))
  {
    assert(qtedge);
    assert(qtedge->scene());
    assert(qtedge->GetArrow());
    assert(qtedge->GetArrow()->scene());
    assert(qtedge->GetQtNode());
    assert(qtedge->GetQtNode()->scene());
    assert(qtedge->GetFrom());
    assert(qtedge->GetFrom()->scene());
    assert(qtedge->GetTo());
    assert(qtedge->GetTo()->scene());
  }
  #endif
}


void ribi::cmap::CheckInvariantAllQtNodesHaveAscene(
  const QtConceptMap& q
  ) noexcept
{
  const auto qtnodes = GetQtNodes(*q.scene());
  for (const auto qtnode: qtnodes)
  {
    assert(qtnode);
    assert(qtnode->scene());
  }
}

void ribi::cmap::CheckInvariantNoLonelyQuadBezierArrows(const QtConceptMap& q) noexcept
{
  for (QGraphicsItem * const item: q.scene()->items())
  {
    if (QtQuadBezierArrowItem * const qtarrow
      = qgraphicsitem_cast<QtQuadBezierArrowItem*>(item))
    {
      assert(qtarrow->GetFromItem());
      assert(qtarrow->GetToItem());
      assert(qtarrow->GetFromItem()->scene());
      assert(qtarrow->GetToItem()->scene());
      assert(qtarrow->GetFromItem()->isVisible());
      assert(qtarrow->GetToItem()->isVisible());
    }
  }
}

void ribi::cmap::CheckInvariantNoUnknownItems(const QtConceptMap& q) noexcept
{
  for (QGraphicsItem * const item: q.scene()->items())
  {
    assert(
         qgraphicsitem_cast<QtNode*>(item)
      || qgraphicsitem_cast<QtEdge*>(item)
      || qgraphicsitem_cast<QtNewArrow*>(item)
      || qgraphicsitem_cast<QtQuadBezierArrowItem*>(item)
      || qgraphicsitem_cast<QtTool*>(item)
    );
  }
}

void ribi::cmap::CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(const QtConceptMap& q) noexcept
{
  QtNode * const qtnode = q.GetQtToolItem().GetBuddyItem();
  assert(!qtnode || IsQtNodeNotOnEdge(qtnode));
}

void ribi::cmap::CheckInvariants(const QtConceptMap&
  #ifndef NDEBUG
  q
  #endif
) noexcept
{
  #ifndef NDEBUG
  assert(q.GetQtNewArrow().scene());
  //Only show tools icon in edit mode
  assert(q.GetMode() != Mode::edit || q.GetQtToolItem().scene());
  CheckInvariantAllQtNodesHaveAscene(q);
  CheckInvariantAllQtEdgesHaveAscene(q);
  CheckInvariantNoLonelyQuadBezierArrows(q);
  CheckInvariantNoUnknownItems(q);
  CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(q);
  assert(!q.GetQtNewArrow().GetFrom() || q.GetQtNewArrow().GetFrom()->scene());
  #endif
}

int ribi::cmap::CountQtArrowHeads(const QtConceptMap& q) noexcept
{
  return CountQtArrowHeads(q.GetScene());
}

int ribi::cmap::CountQtArrowTails(const QtConceptMap& q) noexcept
{
  return CountQtArrowTails(q.GetScene());
}

int ribi::cmap::CountQtCenterNodes(const QtConceptMap& q) noexcept
{
  return CountQtCenterNodes(q.GetScene());
}

int ribi::cmap::CountQtEdges(const QtConceptMap& q) noexcept
{
  return CountQtEdges(q.GetScene());
}

int ribi::cmap::CountQtNodes(const QtConceptMap& q) noexcept
{
  return CountQtNodes(q.GetScene());
}

int ribi::cmap::CountSelectedQtNodes(const QtConceptMap& q) noexcept
{
  return CountSelectedQtNodes(q.GetScene());
}

int ribi::cmap::CountSelectedQtEdges(const QtConceptMap& q) noexcept
{
  return CountSelectedQtEdges(q.GetScene());
}

QGraphicsItem::GraphicsItemFlags ribi::cmap::CreateFlags(const QtNode& qtnode, const Mode mode) noexcept
{
  switch (mode)
  {
    case Mode::edit: return CreateEditFlags(qtnode);
    case Mode::rate: return CreateRateFlags(qtnode);
    case Mode::uninitialized: break;
  }
  return CreateUninitializedFlags(qtnode);
}

ribi::cmap::QtEdge * ribi::cmap::ExtractTheOneSelectedQtEdge(const QtConceptMap& q)
{
  return ExtractTheOneSelectedQtEdge(q.GetScene());
}

ribi::cmap::QtEdge * ribi::cmap::FindFirstQtEdge(
  const QtConceptMap& q,
  const std::function<bool(QtEdge*)> predicate) noexcept
{
  return FindFirstQtEdge(q.GetScene(), predicate);
}

ribi::cmap::QtNode * ribi::cmap::FindFirstQtNode(
  const QtConceptMap& q,
  const std::function<bool(QtNode*)> predicate) noexcept
{
  return FindFirstQtNode(q.GetScene(), predicate);
}

ribi::cmap::QtEdge * ribi::cmap::FindQtEdge(
  const QtNode * const qtnode,
  const QtConceptMap& qtconceptmap
) noexcept
{
  return FindQtEdge(qtnode, qtconceptmap.GetScene());
}

ribi::cmap::QtEdge * ribi::cmap::GetFirstQtEdge(const QtConceptMap& q) noexcept
{
  return GetFirstQtEdge(q.GetScene());
}

ribi::cmap::QtNode * ribi::cmap::GetFirstQtNode(const QtConceptMap& q) noexcept
{
  return GetFirstQtNode(q.GetScene());
}

ribi::cmap::QtEdge * ribi::cmap::GetLastQtEdge(const QtConceptMap& q) noexcept
{
  return GetLastQtEdge(q.GetScene());
}

ribi::cmap::QtNode * ribi::cmap::GetLastQtNode(const QtConceptMap& q) noexcept
{
  return ::ribi::cmap::GetLastQtNode(q.GetScene());
}

void ribi::cmap::QtConceptMap::focusInEvent(QFocusEvent *event)
{
  QLinearGradient linearGradient(-500, -500, 500, 500);
  linearGradient.setColorAt(0.0, QColor(214, 214, 255));
  linearGradient.setColorAt(1.0, QColor(255, 255, 255));
  scene()->setBackgroundBrush(linearGradient);
  QGraphicsView::focusInEvent(event);
}

void ribi::cmap::QtConceptMap::focusOutEvent(QFocusEvent *event)
{
  QLinearGradient linearGradient(-500, -500, 500, 500);
  linearGradient.setColorAt(0.0, QColor(214, 214, 214));
  linearGradient.setColorAt(1.0, QColor(255, 255, 255));
  scene()->setBackgroundBrush(linearGradient);
  QGraphicsView::focusOutEvent(event);
}

bool ribi::cmap::IsArrowVisible(QtConceptMap& q) noexcept
{
  return q.GetQtNewArrow().isVisible();
}

bool ribi::cmap::IsInScene(const QtEdge& qtedge, const QGraphicsScene& scene) noexcept
{
  return scene.items().contains(const_cast<QtEdge*>(&qtedge));
}

bool ribi::cmap::IsQtCenterNodeSelected(const QtConceptMap& q)
{
  return IsQtCenterNodeSelected(q.GetScene());
}

void ribi::cmap::RemoveConceptMap(QtConceptMap& q)
{
  q.clearFocus();
  q.GetQtNewArrow().hide();
  q.GetQtHighlighter().SetItem(nullptr); //Do this before destroying items
  q.GetQtToolItem().SetBuddyItem(nullptr);
  assert(!q.GetQtNewArrow().isVisible());

  for (auto qtedge: GetQtEdges(q))
  {
    assert(qtedge);
    //SetSelectedness(false, *qtedge, q);
    q.scene()->removeItem(qtedge);
    delete qtedge;
  }

  for (auto qtnode: GetQtNodes(q))
  {
    SetSelectedness(false, *qtnode);
    q.scene()->removeItem(qtnode);
    delete qtnode;
  }
}



void ribi::cmap::QtConceptMap::DoCommand(Command * const command)
{
  if (!command) return;

  CheckInvariants(*this);
  #ifndef NDEBUG
  qCritical() << "Do command:" << command->text();
  #endif
  m_undo.push(command);

  CheckInvariants(*this);
}

std::vector<QGraphicsItem *> ribi::cmap::GetFocusableItems(
  const QtConceptMap& q
)
{
  auto all_items = GetQtNodesAlsoOnQtEdge(*q.scene());
  std::vector<QGraphicsItem *> items;
  std::copy_if(std::begin(all_items),std::end(all_items),std::back_inserter(items),
    [](const QGraphicsItem* const item)
    {
      return (item->flags() & QGraphicsItem::ItemIsFocusable)
        && (item->flags() & QGraphicsItem::ItemIsSelectable)
        && item->isVisible()
      ;
    }
  );
  return items;
}

std::vector<QGraphicsItem *> ribi::cmap::GetFocusableNonselectedItems(
  const QtConceptMap& q
)
{
  auto all_items = GetFocusableItems(q);
  std::vector<QGraphicsItem *> items;
  std::copy_if(
    std::begin(all_items),
    std::end(all_items),
    std::back_inserter(items),
    [](const QGraphicsItem* const item)
    {
      return !item->isSelected();
    }
  );
  return items;
}


ribi::cmap::QtNode* ribi::cmap::GetItemBelowCursor(
  const QtConceptMap& q,
  const QPointF& pos
) noexcept
{
  const QList<QGraphicsItem*> v = q.scene()->items(
    pos.x(),pos.y(), 2.0, 2.0, Qt::IntersectsItemShape, Qt::AscendingOrder
  );
  std::vector<QtNode*> qtnodes;
  std::for_each(v.begin(),v.end(),
    [&qtnodes](QGraphicsItem* const item)
    {
      if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
      {
        assert(!qgraphicsitem_cast<QtTool*>(item) && "Cannot draw arrow to ToolsItem");
        qtnodes.push_back(qtnode);
      }
    }
  );
  if (!qtnodes.empty())
  {
    return qtnodes[0];
  }
  return nullptr;
}

ribi::cmap::QtNode * ribi::cmap::GetQtCenterNode(const QtConceptMap& q) noexcept
{
  return GetQtCenterNode(q.GetScene());
}

std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetQtEdges(const QtConceptMap& q) noexcept
{
  return GetQtEdges(q.GetScene());
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetQtNodes(const QtConceptMap& q) noexcept
{
  return GetQtNodes(q.GetScene());
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetQtNodesAlsoOnQtEdge(
  const QtConceptMap& q) noexcept
{
  return GetQtNodesAlsoOnQtEdge(q.GetScene());
}

ribi::cmap::QtNode * ribi::cmap::GetQtToolItemBuddy(const QtConceptMap& q) noexcept
{
  return q.GetQtToolItem().GetBuddyItem();
}

std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetSelectedQtEdges(
  const QtConceptMap& q) noexcept
{
  return GetSelectedQtEdges(q.GetScene());
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetSelectedQtNodes(const QtConceptMap& q) noexcept
{
  return GetSelectedQtNodes(q.GetScene());
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(
  const QtConceptMap& q) noexcept
{
  return GetSelectedQtNodesAlsoOnQtEdge(q.GetScene());
}

const ribi::cmap::QtItemHighlighter& ribi::cmap::QtConceptMap::GetQtHighlighter() const noexcept
{
  assert(m_highlighter);
  return *m_highlighter;
}

ribi::cmap::QtItemHighlighter& ribi::cmap::QtConceptMap::GetQtHighlighter() noexcept
{
  assert(m_highlighter);
  return *m_highlighter;
}

const ribi::cmap::QtNewArrow& ribi::cmap::QtConceptMap::GetQtNewArrow() const noexcept
{
  assert(m_arrow);
  return *m_arrow;
}

ribi::cmap::QtNewArrow& ribi::cmap::QtConceptMap::GetQtNewArrow() noexcept
{
  assert(m_arrow);
  return *m_arrow;
}

const ribi::cmap::QtTool& ribi::cmap::QtConceptMap::GetQtToolItem() const noexcept
{
  assert(m_tools);
  return *m_tools;
}

ribi::cmap::QtTool& ribi::cmap::QtConceptMap::GetQtToolItem() noexcept
{
  assert(m_tools);
  return *m_tools;
}


bool ribi::cmap::HasScene(const QtEdge& qtedge, const QGraphicsScene * const scene) noexcept
{
  return qtedge.scene() == scene
    && qtedge.GetQtNode()->scene() == scene
    && qtedge.GetArrow()->scene() == scene
  ;
}

bool ribi::cmap::HasSelectedItems(const QtConceptMap& q) noexcept
{
  for (const auto * const item:  q.items())
  {
    if (item->isSelected())
    {
      return true;
    }
  }
  return false;
}

void ribi::cmap::QtConceptMap::hideEvent(QHideEvent *)
{
  CheckInvariants(*this);

  m_timer->stop();

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::keyPressEvent(QKeyEvent *event)
{
  CheckInvariants(*this);

  event->ignore();
  //event->setAccepted(false);
  this->m_arrow->Stop();
  CheckInvariants(*this);

  ProcessKey(*this, event);

  //Pass event to base class
  if (!event->isAccepted())
  {
    //Don't.
    //QtKeyboardFriendlyGraphicsView::keyPressEvent(event);
  }

  CheckInvariants(*this);
}

void ribi::cmap::keyPressEvent1(QtConceptMap& q, QKeyEvent *event) noexcept
{
  try
  {
    const auto cmd = new CommandToggleArrowTail(q);
    q.DoCommand(cmd);
  }
  catch (const std::exception&)
  {
    return;
  }
  event->setAccepted(true);
}

void ribi::cmap::keyPressEvent2(QtConceptMap& q, QKeyEvent *event) noexcept
{
  try
  {
    const auto cmd = new CommandToggleArrowHead(q);
    q.DoCommand(cmd);
  }
  catch (const std::exception&)
  {
    return;
  }
  event->setAccepted(true);
}

void ribi::cmap::keyPressEventArrows(QtConceptMap& q, QKeyEvent *event) noexcept
{
  CheckInvariants(q);
  if (!event->modifiers() || (event->modifiers() & Qt::ShiftModifier))
  {
    keyPressEventArrowsSelect(q, event);
  }
  else if (event->modifiers() &  Qt::ControlModifier)
  {
    keyPressEventArrowsMove(q, event);
  }
  CheckInvariants(q);
}

void ribi::cmap::keyPressEventArrowsSelect(QtConceptMap& q, QKeyEvent *event) noexcept
{
  CheckInvariants(q);

  event->ignore();

  if (!q.GetScene().focusItem()) return;


  //GetClosestNonselectedItem must not find the QtToolItem,
  //so it is temporarily selected
  assert(!q.GetQtToolItem().isSelected());
  q.GetQtToolItem().setSelected(true);

  QGraphicsItem * item = ribi::GetClosestNonselectedItem(
    q,
    q.GetScene().focusItem(),
    KeyToDirection(event->key())
  );

  q.GetQtToolItem().setSelected(false);
  assert(!q.GetQtToolItem().isSelected());

  assert(!qgraphicsitem_cast<QtTool*>(item));
  assert(!qgraphicsitem_cast<QtNewArrow*>(item));

  //Cannot convert QObject to QGraphicsItem
  //assert(!qgraphicsitem_cast<QtItemHighlighter*>(item));

  if (!item) return;

  assert(qgraphicsitem_cast<QtNode*>(item) || qgraphicsitem_cast<QtEdge*>(item));

  //If select exclusively (no shift), unselect all current select items
  if (!event->modifiers() || q.GetMode() != Mode::edit)
  {
    //UnselectAll(q)
    if (CountSelectedQtEdges(q) + CountSelectedQtNodes(q) > 0)
    {
      q.DoCommand(new CommandUnselectAll(q));
    }
  }

  try
  {
    q.DoCommand(new CommandSelect(q, *item));
    event->accept();
  }
  catch (std::exception&) {} //OK
  CheckInvariants(q);
}

void ribi::cmap::keyPressEventArrowsMove(QtConceptMap& q, QKeyEvent *event) noexcept
{
  assert(event->modifiers() &  Qt::ControlModifier);

  CheckInvariants(q);

  event->ignore();

  double dx{0.0};
  double dy{0.0};
  switch (event->key())
  {
    case Qt::Key_Up: dy = -10.0; break;
    case Qt::Key_Right: dx =  10.0; break;
    case Qt::Key_Down: dy =  10.0; break;
    default:
      assert(event->key() == Qt::Key_Left);
      dx = -10.0;
      break;
  }

  //Move edges
  for (QtEdge * const qtedge: GetQtEdges(q))
  {
    if (IsSelected(*qtedge))
    {
      try
      {
        q.DoCommand(new CommandMoveEdge(q, qtedge, dx, dy));
        event->accept();
      }
      catch (const std::exception&)
      {
        //OK, get here in Rate and Uninitialized mode
      }
    }
  }

  //Move nodes
  for (QtNode * const qtnode: GetQtNodes(q))
  {
    if (IsSelected(*qtnode)
      && IsQtNodeNotOnEdge(qtnode)
      && IsMovable(*qtnode))
    {
      q.DoCommand(new CommandMoveNode(q, qtnode, dx, dy));
      event->accept();
    }
  }

  CheckInvariants(q);
}

void ribi::cmap::keyPressEventDelete(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (q.GetMode() != Mode::edit) return;
  if (event->modifiers() != Qt::NoModifier) return;

  CheckInvariants(q);

  try
  {
    q.DoCommand(new CommandDeleteSelected(q));
    event->accept();
  }
  catch (const std::exception&)
  {
    event->ignore();
  }

  CheckInvariants(q);
}

void ribi::cmap::keyPressEventE(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    try
    {
      q.DoCommand(new CommandCreateNewEdge(q));
    }
    catch (std::exception&) {} //!OCLINT Correct, nothing happens in catch
    event->setAccepted(true);
  }
}

void ribi::cmap::keyPressEventEscape(QtConceptMap& q, QKeyEvent *event) noexcept
{
  //Only remove the 'new arrow' if present
  if (q.GetQtNewArrow().isVisible())
  {
    q.GetQtNewArrow().hide();
    assert(!q.GetQtNewArrow().isVisible());
    return;
  }
  event->setAccepted(false); //Signal to dialog to close
}

void ribi::cmap::keyPressEventF1(
  QtConceptMap& q,
  QKeyEvent * const event
) noexcept
{
  //Cannot edit/rate in uninitialized mode
  if (q.GetMode() == Mode::uninitialized) return;

  try
  {
    const auto items = q.scene()->selectedItems();
    std::vector<QtNode*> qtnodes;
    for (auto * const item: items)
    {
      if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
      {
        qtnodes.push_back(qtnode);
      }
    }
    if (qtnodes.size() != 1)
    {
      return;
    }
    OnNodeKeyDownPressed(q, *qtnodes.back(), event);
  }
  catch (const std::exception&) {} //!OCLINT Correct, nothing happens in catch
}

void ribi::cmap::keyPressEventF2(QtConceptMap& q, QKeyEvent * const event) noexcept
{
  //Cannot edit/rate in uninitialized mode
  if (q.GetMode() == Mode::uninitialized) return;

  CheckInvariants(q);
  try
  {
    const auto items = q.scene()->selectedItems();
    std::vector<QtNode*> qtnodes;
    for (auto * const item: items)
    {
      if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
      {
        qtnodes.push_back(qtnode);
      }
    }
    if (qtnodes.size() != 1)
    {
      return;
    }
    OnNodeKeyDownPressed(q, *qtnodes.back(), event);
  }
  catch (std::exception&) {} //!OCLINT Correct, nothing happens in catch
  CheckInvariants(q);
}

void ribi::cmap::keyPressEventF4(QtConceptMap&, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::AltModifier)
  {
    event->setAccepted(false); //Signal to dialog to close
  }
}


void ribi::cmap::keyPressEventH(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    try
    {
      const auto cmd = new CommandToggleArrowHead(q);
      q.DoCommand(cmd);
    }
    catch (std::exception&) {} //!OCLINT Correct, nothing happens in catch
    event->setAccepted(true);
  }
}

void ribi::cmap::keyPressEventN(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (q.GetMode() != Mode::edit) return;
  if (event->modifiers() & Qt::ControlModifier)
  {
    //Always works
    q.DoCommand(new CommandCreateNewNode(q));
  }
}

void ribi::cmap::keyPressEventSpace(QtConceptMap& q, QKeyEvent * event) noexcept
{
  const auto items = GetFocusableItems(q);
  if (items.size() == 0) return;

  const bool keep_old_selection = event->modifiers() & Qt::ShiftModifier;
  SetRandomFocus(q, keep_old_selection);
  event->setAccepted(true);
}

void ribi::cmap::keyPressEventT(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    try
    {
      const auto cmd = new CommandToggleArrowTail(q);
      q.DoCommand(cmd);
    }
    catch (std::exception& e) {} //!OCLINT Correct, nothing happens in catch
    event->setAccepted(true);
  }
}

void ribi::cmap::keyPressEventZ(QtConceptMap& q, QKeyEvent *event) noexcept
{
  try
  {
    if (event->modifiers() & Qt::ControlModifier)
    {
      if (event->modifiers() & Qt::ShiftModifier)
      {
        q.Redo();
      }
      else
      {
        q.Undo();
      }
    }
  }
  catch (std::exception&) {} //!OCLINT Correct, nothing happens in catch
}

void ribi::cmap::MoveQtEdgesAndQtNodesRandomly(QtConceptMap& q)
{
  CheckInvariants(q);
  for (QGraphicsItem * const item: q.GetScene().items())
  {
    const int dx{(std::rand() % 20) - 10};
    const int dy{(std::rand() % 20) - 10};
    if (QtEdge * const qtedge = qgraphicsitem_cast<QtEdge*>(item))
    {
      Move(*qtedge, dx, dy);
    }
    else
    {
      QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item);
      if (qtnode && IsQtNodeNotOnEdge(qtnode))
      {
        Move(*qtnode, dx, dy);
      }
    }
  }
  CheckInvariants(q);
}

void ribi::cmap::MoveQtEdge(QtEdge& qtedge, const double dx, const double dy, QtConceptMap& q)
{
  //View
  Move(qtedge, dx, dy);

  //Make QtToolItem follow
  q.GetQtToolItem().Reposition();
  q.GetScene().update();
}

void ribi::cmap::MoveQtNode(QtNode& qtnode, const double dx, const double dy, QtConceptMap& q)
{
  //View
  Move(qtnode, dx, dy);

  //Make QtToolItem follow
  q.GetQtToolItem().Reposition();
  q.GetScene().update();
}

void ribi::cmap::MoveQtNodesAwayFromEachOther(ribi::cmap::QtConceptMap& q) noexcept
{
  //Do not check invariants here! These may be checked in the middle of another process
  for (const auto item: q.scene()->items())
  {
    if (!(item->flags() & QGraphicsItem::ItemIsMovable)) continue;
    QtNode* const qtnode = qgraphicsitem_cast<QtNode*>(item);
    if (!qtnode) continue;
    const auto others = item->collidingItems();
    for (const auto other: others)
    {
      const QtNode* const other_qtnode = qgraphicsitem_cast<const QtNode*>(other);
      if (!other_qtnode) continue;

      const double dx = qtnode->x() - other_qtnode->x() > 0.0 ? 1.0 : -1.0;
      const double dy = qtnode->y() - other_qtnode->y() > 0.0 ? 1.0 : -1.0;

      if (IsQtNodeOnEdge(qtnode))
      {
        QtEdge * const qtedge = FindQtEdge(qtnode, q.GetScene());
        assert(qtedge);
        Move(*qtedge, dx, dy);
      }
      else
      {
        assert(IsQtNodeNotOnEdge(qtnode));
        Move(*qtnode, dx, dy);
      }
    }
  }
}

void ribi::cmap::QtConceptMap::mouseDoubleClickEvent(QMouseEvent *event)
{
  CheckInvariants(*this);

  if (m_mode != Mode::edit) return;

  const QPointF pos = mapToScene(event->pos());

  //Do not create new nodes when double-clicking on an existing one
  if (scene()->itemAt(pos, QTransform())) return;

  //Create new node at the mouse cursor its position
  DoCommand(
    new CommandCreateNewNode(
      *this,
      "",
      NodeType::normal,
      pos.x(), pos.y()
    )
  );

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::mouseMoveEvent(QMouseEvent * event)
{
  CheckInvariants(*this);
  event->ignore();

  if (m_arrow->isVisible())
  {
    const QPointF pos = mapToScene(event->pos());
    m_arrow->SetHeadPos(pos.x(), pos.y());

    //Move the item under the arrow
    QtNode* const item_below = GetItemBelowCursor(*this, mapToScene(event->pos()));

    //If there is no item, set nullptr to be highlighted
    //Alse, only highlight solitary QtNodes, not the QtNodes on the QtEdges
    if (!item_below || (item_below && !item_below->parentItem()))
    {
      assert(m_highlighter);
      m_highlighter->SetItem(item_below); //item_below is allowed to be nullptr
    }
  }
  else
  {
    m_highlighter->SetItem(nullptr); //item_below is allowed to be nullptr

    if (event->buttons() & Qt::LeftButton)
    {
      assert(!m_last_mouse_click_pos.empty());
      const QPointF new_pos = mapToScene(event->pos());
      const double dx{new_pos.x() - m_last_mouse_click_pos[0].x()};
      const double dy{new_pos.y() - m_last_mouse_click_pos[0].y()};
      for (auto * const item  : scene()->selectedItems())
      {
        if (QtNode * const qtnode = qgraphicsitem_cast<QtNode*>(item))
        {
          qtnode->moveBy(dx, dy);
          qtnode->update();
          if (IsOnEdge(*qtnode))
          {
            QtEdge * const qtedge{
              qgraphicsitem_cast<QtEdge*>(qtnode->parentItem())
            };
            assert(qtedge);
            qtedge->update();
            qtedge->GetArrow()->update();
          }
          else
          {
            for (QGraphicsItem * const this_item: scene()->items())
            {
              if (QtEdge * const this_qtedge = qgraphicsitem_cast<QtEdge*>(this_item))
              {
                this_qtedge->update();
                this_qtedge->GetArrow()->update();
              }
            }
            #ifdef WOULD_GETQTEDGES_AND_COLLECT_WORK
            const auto qtedges = GetQtEdges(qtnode, scene());
            for (QtEdge* const qtedge: qtedges)
            {
              qtedge->update();
              qtedge->GetQtNode()->update();
              qtedge->GetArrow()->update();
            }
            #endif
          }
        }
      }
      m_last_mouse_click_pos[0] = new_pos;
    }
    assert(m_highlighter);
  }
  CheckInvariants(*this);

  m_arrow->update();

  CheckInvariants(*this);

  QtKeyboardFriendlyGraphicsView::mouseMoveEvent(event);

  //The tools icon must follow the freshly moved item
  m_tools->Reposition();

  CheckInvariants(*this);

  this->repaint();
  this->update();
}

void ribi::cmap::QtConceptMap::mousePressEvent(QMouseEvent *event)
{
  CheckInvariants(*this);

  if (GetMode() == Mode::uninitialized)
  {
    event->ignore();
    return;
  }

  m_last_mouse_click_pos.resize(1);
  m_last_mouse_click_pos[0] = mapToScene(event->pos());

  if (IsArrowVisible(*this))
  {
    mousePressEventArrowActive(*this, event);
  }
  else
  {
    mousePressEventNoArrowActive(*this, event);
  }

  if (!event->isAccepted())
  {
    QtKeyboardFriendlyGraphicsView::mousePressEvent(event);
  }

  CheckInvariants(*this);
}

void ribi::cmap::mousePressEventNoArrowActive(QtConceptMap& q, QMouseEvent *event) noexcept
{
  CheckInvariants(q);

  const QPointF pos = q.mapToScene(event->pos());
  QGraphicsItem * const item = q.GetScene().itemAt(pos, QTransform());

  //If clicking in the void, unselect all if no modifier
  if (!item)
  {
    if (event->modifiers() != Qt::NoModifier) return;
    try
    {
      q.DoCommand(new CommandUnselectAll(q));
      event->accept();
    }
    catch (std::exception&)
    {
      event->ignore();
    }
    return;
  }

  //If clicking on a tool icon, start a new arrow
  if (QtTool * const qtool = qgraphicsitem_cast<QtTool*>(item))
  {
    assert(q.GetMode() == Mode::edit);
    assert(qtool->GetBuddyItem());
    q.GetQtNewArrow().Start(qtool->GetBuddyItem());
    event->accept();
    return;
  }

  //Clicking on the quad Bezier arrow anyway ...
  if (qgraphicsitem_cast<QtQuadBezierArrowItem*>(item))
  {
    #ifndef NDEBUG
    qDebug() << "Should not click on the QtQuadBezierArrow,"
      "but on the QtEdge instead"
    ;
    #endif // NDEBUG
    event->ignore();
    return;
  }

  //Click on an edge or a node
  assert(qgraphicsitem_cast<QtEdge*>(item) || qgraphicsitem_cast<QtNode*>(item));
  try
  {
    assert(item);
    if (item->isSelected())
    {
      try
      {
        q.DoCommand(new CommandUnselect(q, *item));
      }
      catch (const std::exception& e)
      {
        qCritical() << e.what();
        assert(!"Should not get here?");
      }
      catch (...)
      {
        assert(!"Should really not get here?");
      }
    }
    else
    {
      Command * const command{new CommandSelect(q, *item)};
      if (HasSelectedItems(q)
        && (
          event->modifiers() == Qt::NoModifier //Unselect all without shift
          || q.GetMode() != Mode::edit         //Unselect all with shift in non-edit mode
        )
      )
      {
        q.DoCommand(new CommandUnselectAll(q));
      }
      q.DoCommand(command);

      //Essential for having movable QtNodes and QtEdges
      //event->ignore(); //Nonsense, use mouseMoveEvent instead

      //Essential for selecting QtNodes and QtEdges cleanly
      event->accept();
    }
  }
  catch (std::exception&) {} //OK

  CheckInvariants(q);
}

void ribi::cmap::mousePressEventArrowActive(QtConceptMap& q, QMouseEvent *event) noexcept
{
  CheckInvariants(q);

  event->accept();

  assert(!q.GetQtNewArrow().isSelected());
  if (q.GetQtNewArrow().isVisible())
  {
    QtNode * const from{q.GetQtNewArrow().GetFrom()};
    assert(from);
    assert(from->scene());
    assert(!IsOnEdge(*from));
    QtNode * const to{q.GetQtHighlighter().GetItem()};
    if (to && from != to)
    {
      assert(!IsOnEdge(*to));
      assert(to->scene());
      //The command needs to find the two selected vertices
      assert(!q.GetQtNewArrow().isSelected());
      for (auto& i: q.GetScene().selectedItems()) { i->setSelected(false); }
      to->setSelected(true);
      from->setSelected(true);
      const auto command = new CommandCreateNewEdge(q, from, to);
      q.DoCommand(command);
      q.GetQtNewArrow().hide();
      q.GetQtHighlighter().SetItem(nullptr);
      assert(command->GetAddedQtEdge().GetFrom()->scene());
      assert(command->GetAddedQtEdge().GetTo()->scene());

    }
  }

  assert(!q.GetQtNewArrow().isSelected());
  CheckInvariants(q);
}

void ribi::cmap::QtConceptMap::mouseReleaseEvent(QMouseEvent *)
{
  m_last_mouse_click_pos.resize(0);
}

void ribi::cmap::QtConceptMap::paintEvent(QPaintEvent *event)
{
  try
  {
    QtKeyboardFriendlyGraphicsView::paintEvent(event);
  }
  catch (const std::exception& e)
  {
    #ifndef NDEBUG
    qCritical() << e.what();
    #endif
  }
}

void ribi::cmap::QtConceptMap::Respond()
{
  //Do not check invariants here! These may be checked in the middle of another process
  assert(this->isVisible());
  assert(this->isEnabled());
  assert(this->scene());
  MoveQtNodesAwayFromEachOther(*this);
}

void ribi::cmap::QtConceptMap::OnFocusItemChanged(
  QGraphicsItem * newFocus, QGraphicsItem *, Qt::FocusReason reason
)
{
  CheckInvariants(*this);
  //Focus on QtToolItem
  if (newFocus == &GetQtToolItem()
    && !GetQtNewArrow().isVisible()
    && GetQtToolItem().GetBuddyItem()
    && reason == Qt::MouseFocusReason
  )
  {
    GetQtNewArrow().Start(GetQtToolItem().GetBuddyItem()); //Also sets visibility
    GetQtToolItem().setSelected(false);

    //Will trigger onSelectionChanged and hide the arrow
    GetQtToolItem().GetBuddyItem()->setSelected(true);

    GetQtToolItem().GetBuddyItem()->setFocus();
    GetQtNewArrow().setVisible(true);
  }
  else
  {
    GetQtNewArrow().setVisible(false);
  }

  CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(*this);
  CheckInvariants(*this);
}

void ribi::cmap::OnNodeKeyDownPressed(
  QtConceptMap& q,
  QtNode& qtnode,
  QKeyEvent * const event
)
{
  //Cannot select a QtNode in uninitialized mode
  assert(q.GetMode() != Mode::uninitialized);

  //Cannot edit center nodes
  if (IsQtCenterNode(&qtnode))
  {
    event->ignore();
    return;
  }

  const int key{event->key()};

  //Note: item can also be the QtNode on a QtEdge
  if (q.GetMode() == Mode::edit && key == Qt::Key_F2)
  {
    OnNodeKeyDownPressedEditF2(q, qtnode, event);
    //Will accept at 'OnNodeKeyDownPressedEditF2' iff accepted
    //event->accept();
  }
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F1)
  {
    //Can only rate relations
    if (!IsOnEdge(qtnode))
    {
      OnNodeKeyDownPressedRateF1(q, qtnode, event);
      //Will accept at 'OnNodeKeyDownPressedRateF1' iff accepted
      //event->accept();
    }
  }
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F2)
  {
    OnNodeKeyDownPressedRateF2(q, qtnode, event);
    //Will accept at 'OnNodeKeyDownPressedRateF2' iff accepted
    //event->accept();
  }
  q.show();
  q.setEnabled(true);
  q.setFocus();
  q.scene()->setFocusItem(&qtnode);
  qtnode.setSelected(true);
  q.scene()->update();
}

//Moved to _no_codecov.cpp
//void ribi::cmap::OnNodeKeyDownPressedEditF2(
//  QtConceptMap& q,
//  QtNode& qtnode,
//  QKeyEvent * const event
//)

//Moved to _no_codecov.cpp
//void ribi::cmap::OnNodeKeyDownPressedRateF1(
//  QtConceptMap& q,
//  QtNode& qtnode,
//  QKeyEvent * const event
//)

//Moved to _no_codecov.cpp
//void ribi::cmap::OnNodeKeyDownPressedRateF2(
//  QtConceptMap& q,
//  QtNode& qtnode,
//  QKeyEvent * const event
//)

void ribi::cmap::ProcessKey(QtConceptMap& q, QKeyEvent * const event) //!OCLINT Although the NCSS is high, the code is easy to read
{
  CheckInvariants(q);

  //Pass event
  switch (event->key())
  {
    case Qt::Key_1: keyPressEvent1(q, event); break;
    case Qt::Key_2: keyPressEvent2(q, event); break;
    case Qt::Key_Delete: keyPressEventDelete(q, event); break;
    case Qt::Key_E: keyPressEventE(q, event); break;
    case Qt::Key_Equal:
      if (q.GetMode() != Mode::uninitialized) q.scale(1.1, 1.1);
      break;
    case Qt::Key_Escape: keyPressEventEscape(q, event); break;
    case Qt::Key_F1: keyPressEventF1(q, event); break;
    case Qt::Key_F2: keyPressEventF2(q, event); break;
    case Qt::Key_F4: keyPressEventF4(q, event); break;
    #ifndef NDEBUG
    case Qt::Key_F7: q.DoCommand(new CommandUnselectAll(q)); break;
    case Qt::Key_F8: MoveQtEdgesAndQtNodesRandomly(q); break;
    case Qt::Key_F9: std::exit(1); break; //Cause a deliberate hard crash
    #endif
    case Qt::Key_H: keyPressEventH(q, event); break;
    case Qt::Key_Minus:
      if (q.GetMode() != Mode::uninitialized) q.scale(0.9, 0.9);
      break;
    case Qt::Key_N: keyPressEventN(q, event); break;
    case Qt::Key_T: keyPressEventT(q, event); break;
    case Qt::Key_Z: keyPressEventZ(q, event); break;
    case Qt::Key_Space: keyPressEventSpace(q, event); break;
    case Qt::Key_Up:
    case Qt::Key_Right:
    case Qt::Key_Left:
    case Qt::Key_Down: keyPressEventArrows(q, event); break;
    default: break;
  }

  CheckInvariants(q);
}


void ribi::cmap::QtConceptMap::Redo() noexcept
{
  CheckInvariants(*this);

  if (m_undo.canRedo())
  {
    #ifndef NDEBUG
    qCritical() << "Redo command:" << m_undo.command(m_undo.index())->text();
    #endif // NDEBUG
  }
  m_undo.redo();

  CheckInvariants(*this);
}

void ribi::cmap::SaveToFile(const QtConceptMap& q, const std::string& dot_filename)
{
  SaveToFile(q.ToConceptMap(), dot_filename);
}

void ribi::cmap::QtConceptMap::SetConceptMap(const ConceptMap& conceptmap)
{
  CheckInvariants(*this);
  RemoveConceptMap(*this);

  QtNode* const no_qtnode = nullptr;
  SetQtToolItemBuddy(*this, no_qtnode);
  CheckInvariants(*this);

  assert(GetQtNodes(*this).empty());
  AddNodesToScene(*this, conceptmap);
  AddEdgesToScene(*this, conceptmap);
  CheckInvariants(*this);

  //Do not fit to view, as requested by the employer
  //fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);

  CheckInvariants(*this);
  Ensures(CountQtNodes(*this) ==
    static_cast<int>(boost::num_vertices(conceptmap)));
  Ensures(CountQtEdges(*this) ==
    static_cast<int>(boost::num_edges(conceptmap)));
}

///Class T may be either a QtNode or a QtEdge
template <class T>
void SelectImpl(
  T& t
)
{
  static_assert(
    std::is_same<T, ribi::cmap::QtNode>() || std::is_same<T, ribi::cmap::QtEdge>(),
    "T is either QtEdge or QtNode");
  SetSelectedness(true, t);
}

void ribi::cmap::Select(QtConceptMap& q, QtEdge& qtedge)
{
  SelectImpl(qtedge);
  //Edges do not get a tool item
  QtNode * const no_qtnode{nullptr};
  SetQtToolItemBuddy(q, no_qtnode);
}

void ribi::cmap::Select(QtConceptMap& q, QtNode& qtnode)
{
  SelectImpl(qtnode);
  SetQtToolItemBuddy(q, &qtnode);
}

void ribi::cmap::SetFocus(QtConceptMap& q, QtNode* const new_focus_item)
{
  CheckInvariants(q);

  assert(new_focus_item);
  assert(!new_focus_item->isSelected());

  if (QtEdge * const qtedge = FindQtEdge(new_focus_item, q.GetScene()))
  {
    SetSelectedness(true, *qtedge);
  }
  else
  {
    SetSelectedness(true, *new_focus_item);
  }

  //Only QtNodes (not on QtEdge) have a QtToolItem
  if (IsQtNodeNotOnEdge(new_focus_item))
  {
    SetQtToolItemBuddy(q, new_focus_item);
  }
  else
  {
    QtNode * const no_qtnode{nullptr};
    SetQtToolItemBuddy(q, no_qtnode);
  }
  CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(q);

  new_focus_item->setFocus(); //Do after SetQt(Tool&Example)Buddies
  q.update();
  CheckInvariants(q);
}

void ribi::cmap::QtConceptMap::SetMode(const ribi::cmap::Mode mode) noexcept
{
  CheckInvariants(*this);
  this->m_arrow->Stop();
  m_mode = mode;

  auto qtnodes = GetQtNodesAlsoOnQtEdge(*scene());
  for (auto qtnode: qtnodes)
  {
    qtnode->SetBrushFunction(GetQtNodeBrushFunction(m_mode));
    qtnode->SetVignetteBrushFunction(GetQtNodeVignetteBrushFunction(m_mode));
    qtnode->setFlags(CreateFlags(*qtnode, mode));
  }
  if (mode == Mode::edit && !m_tools->scene())
  {
    scene()->addItem(m_tools);
    assert(m_tools->scene());
  }
  else if (m_tools->scene())
  {
    scene()->removeItem(m_tools);
    assert(!m_tools->scene());
  }

  CheckInvariants(*this);
}

void ribi::cmap::SetRandomFocus(
  QtConceptMap& q,
  const bool keep_old_selection
)
{
  CheckInvariants(q);
  if (keep_old_selection)
  {
    SetRandomFocusAdditive(q);
  }
  else
  {
    SetRandomFocusExclusive(q);
  }
  CheckInvariants(q);
}

void ribi::cmap::SetRandomFocusAdditive(
  QtConceptMap& q
)
{
  CheckInvariants(q);

  //Let a random QtNode receive focus
  const auto items = GetFocusableNonselectedItems(q);

  if (items.empty()) return;


  QGraphicsItem * const cur_focus_item = q.GetScene().focusItem();

  //There is only one item: the currently focussed item,
  //so there is no item to additively give focus
  if (items.size() == 1 && items[0] == cur_focus_item) return;


  QtNode * next_focus_item = nullptr;

  //Pick random Focusable non-selected QtNotde
  static std::mt19937 rng_engine{0};
  std::uniform_int_distribution<int> distribution(0, static_cast<int>(items.size()) - 1);
  while (!next_focus_item || cur_focus_item == next_focus_item)
  {
    const int i{distribution(rng_engine)};
    assert(i >= 0);
    assert(i < static_cast<int>(items.size()));
    next_focus_item = qgraphicsitem_cast<QtNode*>(items[i]);
    assert(next_focus_item);
    assert(!next_focus_item->isSelected());
  }

  if (cur_focus_item)
  {
    //Really loose focus, but not selectedness
    cur_focus_item->setEnabled(false);
    cur_focus_item->setSelected(false);
    cur_focus_item->clearFocus();
    cur_focus_item->setEnabled(true);
    cur_focus_item->setSelected(true);
  }
  SetFocus(q, next_focus_item);

  CheckInvariants(q);
}

void ribi::cmap::SetRandomFocusExclusive(
  QtConceptMap& q
)
{
  CheckInvariants(q);

  if (CountSelectedQtEdges(q) + CountSelectedQtNodes(q) > 0)
  {
    q.DoCommand(new CommandUnselectAll(q));
  }

  if (q.GetScene().focusItem())
  {
    q.GetScene().focusItem()->clearFocus();
  }

  //Let a random QtNode receive focus
  const auto items = GetFocusableNonselectedItems(q);

  if (items.empty()) return;

  static std::mt19937 rng_engine{0};
  std::uniform_int_distribution<int> distribution(0, static_cast<int>(items.size()) - 1);
  const int i{distribution(rng_engine)};
  assert(i >= 0);
  assert(i < static_cast<int>(items.size()));
  QtNode * const new_focus_item = qgraphicsitem_cast<QtNode*>(items[i]);
  assert(new_focus_item);
  assert(!new_focus_item->isSelected());
  SetFocus(q, new_focus_item);

  CheckInvariants(q);
}

void ribi::cmap::SetQtToolItemBuddy(QtConceptMap& q, QtNode * const qtnode)
{
  q.GetQtToolItem().SetBuddyItem(qtnode);
}

void ribi::cmap::SetSelectedness(
  const bool is_selected,
  QtEdge& qtedge
)
{
  qtedge.SetSelected(is_selected);
}

void ribi::cmap::SetSelectedness(
  const bool is_selected,
  QtNode& qtnode
)
{
  qtnode.setSelected(is_selected);
}

void ribi::cmap::QtConceptMap::showEvent(QShowEvent *)
{
  CheckInvariants(*this);

  m_timer->start(10);

  //Do not fit to view, as requested by the employer
  // this->fitInView(this->sceneRect());

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::StartTimer()
{
  m_timer->start();
}

void ribi::cmap::QtConceptMap::StopTimer()
{
  m_timer->stop();
}

ribi::cmap::ConceptMap ribi::cmap::QtConceptMap::ToConceptMap() const noexcept
{
  const auto qtnodes = GetQtNodes(*this);
  ConceptMap g;
  std::map<QtNode *, VertexDescriptor> vds;
  //First add center node
  for (auto qtnode: qtnodes)
  {
    assert(qtnode);
    if (!IsCenterNode(*qtnode)) continue;
    const auto vd = add_bundled_vertex(GetNode(*qtnode), g);
    vds.insert( { qtnode, vd } );
  }

  //Second, add non-center nodes
  for (auto qtnode: qtnodes)
  {
    assert(qtnode);
    if (IsCenterNode(*qtnode)) continue;
    const auto vd = add_bundled_vertex(GetNode(*qtnode), g);
    vds.insert( { qtnode, vd } );
  }
  const auto qtedges = GetQtEdges(*this);
  for (auto qtedge: qtedges)
  {
    assert(qtedge);
    const Edge edge = qtedge->GetEdge();
    assert(edge.GetId() == qtedge->GetId());
    QtNode * const qtnode_from = qtedge->GetFrom();
    QtNode * const qtnode_to = qtedge->GetTo();
    const VertexDescriptor vd_from = vds[qtnode_from];
    const VertexDescriptor vd_to = vds[qtnode_to];
    add_bundled_edge_between_vertices(edge, vd_from, vd_to, g);
  }
  assert(CountQtNodes(*this) == static_cast<int>(boost::num_vertices(g)));
  assert(CountQtEdges(*this) == static_cast<int>(boost::num_edges(g)));
  return g;
}

void ribi::cmap::QtConceptMap::Undo()
{
  if (!m_undo.count())
  {
    throw std::logic_error(
      "Cannot undo if nothing has been done yet"
    );
  }
  CheckInvariants(*this);
  #ifndef NDEBUG
  if (m_undo.index() - 1 >= 0)
  {
    qCritical() << "Undo command:" << m_undo.command(m_undo.index() - 1)->text();
  }
  #endif // NDEBUG
  m_undo.undo();
  CheckInvariants(*this);
}

///Class T may be either a QtNode or a QtEdge
template <class T>
void UnselectImpl(
  ribi::cmap::QtConceptMap& q,
  T& t
)
{
  static_assert(
    std::is_same<T, ribi::cmap::QtNode>() || std::is_same<T, ribi::cmap::QtEdge>(),
    "T is either QtEdge or QtNode");

  ribi::cmap::QtNode * const no_qtnode{nullptr};
  SetQtToolItemBuddy(q, no_qtnode);
  SetSelectedness(false, t);
  if (q.hasFocus()) q.clearFocus();
}

void ribi::cmap::Unselect(QtConceptMap& q, QtEdge& qtedge)
{
  UnselectImpl(q, qtedge);
}

void ribi::cmap::Unselect(QtConceptMap& q, QtNode& qtnode)
{
  UnselectImpl(q, qtnode);
}

void ribi::cmap::QtConceptMap::wheelEvent(QWheelEvent *event)
{
  if (m_mode != Mode::edit) return;
  CheckInvariants(*this);
  const double s{1.1};
  if (event->delta() > 0)
  {
    this->scale(s,s);
  }
  else if (event->delta() < 0)
  {
    this->scale(1.0 / s,1.0 / s);
  }

  CheckInvariants(*this);
}

std::ostream& ribi::cmap::operator<<(std::ostream& os, const QtConceptMap& c) noexcept
{
  int index{0};
  for (const QGraphicsItem* const item: c.scene()->items())
  {
    os << index << ' ' << item << ": ";
    if (const ribi::cmap::QtNode * const qtnode = qgraphicsitem_cast<const ribi::cmap::QtNode*>(item))
    {
      os << "QtNode, has parent item " << qtnode->parentItem();
    }
    else if (const ribi::cmap::QtEdge * const qtedge = qgraphicsitem_cast<const ribi::cmap::QtEdge*>(item))
    {
      os << "QtEdge, "
        << "from " << qtedge->GetFrom()
        << ", center " << qtedge->GetQtNode()
        << ", to " << qtedge->GetTo()
      ;
    }
    else if (const ribi::QtQuadBezierArrowItem * const qtarrow = qgraphicsitem_cast<const ribi::QtQuadBezierArrowItem*>(item))
    {
      os << "QtQuadBezierArrowItem, "
        << "from " << qtarrow->GetFromItem()
        << ", mid " << qtarrow->GetMidItem()
        << ", to" << qtarrow->GetToItem()
      ;
    }
    else if (const ribi::cmap::QtTool * const qttool = qgraphicsitem_cast<const ribi::cmap::QtTool*>(item))
    {
      os << "QtTool, "
        << "is visible: " << qttool->isVisible()
        << ", buddy item: " << qttool->GetBuddyItem();
    }
    else if (const ribi::QtArrowItem * const qtstraightarrow = qgraphicsitem_cast<const ribi::QtArrowItem*>(item))
    {
      os << "QtArrowItem";
    }
    else if (const ribi::cmap::QtNewArrow * const qtnewarrow = qgraphicsitem_cast<const ribi::cmap::QtNewArrow*>(item))
    {
      os << "QtNewArrow, "
        << "is visible: " << qtnewarrow->isVisible()
        << ", connected to " << qtnewarrow->GetFrom();
    }
    else
    {
      os << "UNKNOWN ITEM";
      assert(!"Should not get here");
    }
    os << '\n';
    ++index;
  }
  return os;
}
