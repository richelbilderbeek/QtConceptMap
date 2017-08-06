#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmap.h"

#include <iostream>
#include <set>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <QScrollBar>

#include "fuzzy_equal_to.h"
#include "qtconceptmapcollect.h"

#include "conceptmapconceptfactory.h"
#include "conceptmapconcept.h"
#include "conceptmapedgefactory.h"
#include "conceptmapedge.h"
#include "conceptmapfactory.h"
#include "conceptmap.h"
#include "conceptmaphelper.h"
#include "conceptmapnodefactory.h"
#include "conceptmapnode.h"
#include "container.h"
#include "count_edges_with_selectedness.h"
#include "count_vertices_with_selectedness.h"
#include "find_first_custom_edge.h"
#include "create_direct_neighbour_custom_and_selectable_edges_and_vertices_subgraph.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "get_my_custom_vertex.h"
#include "get_my_custom_edge.h"
#include "has_custom_edge_with_my_edge.h"
#include "qtarrowitem.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapcommandcreatenewedge.h"
#include "qtconceptmapcommandcreatenewnode.h"
#include "qtconceptmapcommanddeleteselected.h"
#include "qtconceptmapcommandtogglearrowhead.h"
#include "qtconceptmapcommandtogglearrowtail.h"
#include "qtconceptmapconcepteditdialog.h"
#include "qtconceptmapexamplesitem.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmapnewarrow.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaprateconceptdialog.h"
#include "qtconceptmaprateexamplesdialog.h"
#include "qtconceptmaptoolsitem.h"
#include "qtquadbezierarrowitem.h"
#include "qtscopeddisable.h"
#include "set_my_custom_vertex.h"
#include "xml.h"

#pragma GCC diagnostic pop

ribi::cmap::QtConceptMap::QtConceptMap(QWidget* parent)
  : QtKeyboardFriendlyGraphicsView(parent),
    m_arrow{new QtNewArrow},
    m_conceptmap{},
    m_examples_item(new QtExamplesItem),
    m_highlighter{new QtItemHighlighter},
    m_mode{Mode::uninitialized},
    m_popup_mode{PopupMode::normal},
    m_timer{new QTimer(this)},
    m_tools{new QtTool}
{
  this->setScene(new QGraphicsScene(this));
  assert(!m_highlighter->GetItem());

  //Add items
  assert(scene());
  scene()->addItem(m_arrow); //Add the QtNewArrow so it has a parent
  scene()->addItem(m_examples_item); //Add the examples so it has a parent
  scene()->addItem(m_tools);
  m_arrow->hide();

  //Without this line, mouseMoveEvent won't be called
  this->setMouseTracking(true);

  {
    QLinearGradient linearGradient(-500,-500,500,500);
    linearGradient.setColorAt(0.0,QColor(214,214,214));
    linearGradient.setColorAt(1.0,QColor(255,255,255));
    scene()->setBackgroundBrush(linearGradient);
    //this->scene()->setBackgroundBrush(QBrush(QColor(255,255,255)));
  }

  //Connect the scene
  QObject::connect(
    scene(),
    SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)), //DOES NOT EXIST
    this,
    SLOT(onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason))
  );
  QObject::connect(scene(),SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));

  m_examples_item->SetCenterPos(123,456); //Irrelevant where

  CheckInvariants(*this);

  {
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(Respond()));
    //Timer is started in showEvent
  }
}

ribi::cmap::QtConceptMap::~QtConceptMap() noexcept
{
  m_timer->stop();
  m_highlighter->Stop();
  m_undo.clear();
  m_highlighter->SetItem(nullptr); //Do this before destroying items
  m_tools->SetBuddyItem(nullptr);
  delete m_examples_item;
  delete m_tools;
  delete m_arrow;
}

void ribi::cmap::AddEdgesToScene(
  QtConceptMap& qtconceptmap
) noexcept
{
  const ConceptMap& conceptmap = qtconceptmap.GetConceptMap();
  QGraphicsScene& scene = *qtconceptmap.scene();

  //Add the Edges
  const auto eip = edges(conceptmap);
  for(auto i = eip.first; i != eip.second; ++i)
  {
    const std::pair<Node, Node> from_to = GetFromTo(*i, conceptmap);
    const Node from{from_to.first};
    const Node to{from_to.second};
    QtNode * const qtfrom = FindQtNode(from.GetId(), scene);
    QtNode * const qtto = FindQtNode(to.GetId(), scene);
    assert(qtfrom != qtto);
    const auto edge_map = get(boost::edge_custom_type, conceptmap);
    const Edge edge = get(edge_map, *i);
    QtEdge * const qtedge{new QtEdge(edge,qtfrom,qtto)};
    if (qtfrom->GetNode().IsCenterNode() || qtto->GetNode().IsCenterNode())
    {
      qtedge->GetQtNode()->setVisible(false);
    }
    assert(qtedge && HasScene(*qtedge, nullptr));
    scene.addItem(qtedge);
    //scene()->addItem(qtedge->GetQtNode()); //Get these for free when adding a QtEdge
    //scene()->addItem(qtedge->GetArrow()); //Get these for free when adding a QtEdge
    assert(qtedge && HasScene(*qtedge, &scene));
    CheckInvariants(*qtedge);
  }
}


void ribi::cmap::AddNodesToScene(
  QtConceptMap& qtconceptmap
) noexcept
{
  const ConceptMap& conceptmap = qtconceptmap.GetConceptMap();
  QGraphicsScene& scene = *qtconceptmap.scene();

  //Add the nodes to the scene, if there are any
  const auto vip = vertices(conceptmap);
  for(auto i = vip.first; i!=vip.second; ++i)
  {
    assert(boost::num_vertices(conceptmap));
    const auto pmap = get(boost::vertex_custom_type, conceptmap);
    const Node node = get(pmap, *i);
    QtNode * const qtnode{new QtNode(node)};
    assert(qtnode);
    assert(!qtnode->scene());
    scene.addItem(qtnode);
    assert(qtnode->scene());
    assert(FindQtNode(node.GetId(), scene));
  }
}

void ribi::cmap::QtConceptMap::changeEvent(QEvent * event)
{
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
}

void ribi::cmap::CheckInvariantAllQtEdgesHaveAscene( //!OCLINT I think the cyclomatic complexity is acceptable here
  const QtConceptMap& q
) noexcept
{
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

void ribi::cmap::CheckInvariantAsMuchNodesAsQtNodesSelected(
  const QtConceptMap& q
  ) noexcept
{
  const int n_selected_qtnodes{CountSelectedQtNodes(q)};
  const int n_selected_nodes{count_vertices_with_selectedness(true, q.GetConceptMap())};
  if (n_selected_qtnodes != n_selected_nodes)
  {
    std::cerr << "n_selected_qtnodes (" << n_selected_qtnodes
      << ") mismatch n_selected_nodes (" << n_selected_nodes << ")\n";
  }
  assert(n_selected_qtnodes == n_selected_nodes);
}

void ribi::cmap::CheckInvariantOneQtNodeWithExamplesHasExamplesItem(
  const QtConceptMap&
  #ifndef NDEBUG
    q
  #endif
) noexcept
{
  #ifndef NDEBUG
  assert(CountSelectedQtNodes(*q.scene())
    == static_cast<int>(ribi::cmap::GetSelectedQtNodes(*q.scene()).size())
  );
  assert(CountSelectedQtEdges(*q.scene())
    == static_cast<int>(ribi::cmap::GetSelectedQtEdges(*q.scene()).size())
  );
  const auto qtnodes = ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(*q.scene());
  if (qtnodes.size() == 1)
  {
    //QtNode must have an example
    const ribi::cmap::QtNode * const qtnode = qtnodes[0];
    //if (HasExamples(qtnode))
    if (!qtnode->GetNode().GetConcept().GetExamples().Get().empty())
    {
      //QtExamplesItem must have that QtNode as its buddy
      const auto buddy_item = q.GetQtExamplesItem().GetBuddyItem();
      assert(buddy_item);
      assert(buddy_item == qtnode);
      assert(q.GetQtExamplesItem().isVisible());
      const bool is_close{IsClose(q.GetQtExamplesItem(), *qtnode)};
      if (!is_close)
      {
        qDebug() << "QtExamplesItem and QtNode not close";
      }
    }
  }
  #endif
}

void ribi::cmap::CheckInvariantSingleSelectQtEdgeMustHaveCorrespondingEdge(const QtConceptMap&
  #ifndef NDEBUG
  q
  #endif
) noexcept
{
  #ifndef NDEBUG
  //If there is one QtEdge selected, its Edge must be able to been found
  try
  {
    const auto qtedge = ExtractTheOneSelectedQtEdge(*q.scene());
    //The QtEdge its edge must be in the concept map
    //Can only compare IDs, as QtEdge::GetEdge() and its equivalent in the concept map may mismatch
    assert(
      has_custom_edge_with_my_edge(
        qtedge->GetEdge(),
        q.GetConceptMap(),
        [](const Edge& lhs, const Edge& rhs)
        {
          return lhs.GetId() == rhs.GetId();
        }
      )
    );
    const auto edge = ExtractTheOneSelectedEdge(q.GetConceptMap(), *q.scene());
    assert(qtedge->GetEdge().GetId() == edge.GetId());
  }
  catch (...) {} //!OCLINT This should be an empty catch statement
  #endif // NDEBUG
}

void ribi::cmap::CheckInvariantSingleSelectedQtNodeMustHaveQtTool(
  const QtConceptMap&
  #ifndef NDEBUG
    q
  #endif
) noexcept
{
  #ifndef NDEBUG
  assert(CountSelectedQtNodes(*q.scene())
    == static_cast<int>(ribi::cmap::GetSelectedQtNodes(*q.scene()).size())
  );
  assert(CountSelectedQtEdges(*q.scene())
    == static_cast<int>(ribi::cmap::GetSelectedQtEdges(*q.scene()).size())
  );
  const auto qtnodes = ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(*q.scene());
  if (qtnodes.size() == 1)
  {
    //QtNode must have an example
    const ribi::cmap::QtNode * const qtnode = qtnodes[0];
    //QtExamplesItem must have that QtNode as its buddy
    const auto buddy_item = q.GetQtToolItem().GetBuddyItem();
    assert(buddy_item);
    assert(buddy_item == qtnode);
  }
  #endif
}

void ribi::cmap::CheckInvariants(const QtConceptMap&
  #ifndef NDEBUG
  q
  #endif
) noexcept
{
  #ifndef NDEBUG
  qApp->processEvents();

  assert(q.GetQtNewArrow().scene());
  assert(q.GetQtExamplesItem().scene());
  assert(q.GetQtToolItem().scene());
  CheckInvariantAsMuchNodesAsQtNodesSelected(q);
  CheckInvariantAllQtNodesHaveAscene(q);
  CheckInvariantAllQtEdgesHaveAscene(q);
  CheckInvariantSingleSelectQtEdgeMustHaveCorrespondingEdge(q);

  CheckInvariantSingleSelectedQtNodeMustHaveQtTool(q);

  //If a QtNode with a vignette is selected, the QtExamplesItem must have that
  //QtNode as its buddy
  //For Issue #96, https://github.com/richelbilderbeek/Brainweaver/issues/96
  CheckInvariantOneQtNodeWithExamplesHasExamplesItem(q);
  #endif
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

void ribi::cmap::HideExamplesItem(QtConceptMap& q) noexcept
{
  q.GetQtExamplesItem().hide();
}

void ribi::cmap::RemoveConceptMap(QtConceptMap& q)
{
  q.GetQtNewArrow().hide();
  q.GetQtExamplesItem().hide();
  q.GetQtHighlighter().SetItem(nullptr); //Do this before destroying items
  q.GetQtToolItem().SetBuddyItem(nullptr);
  assert(!q.GetQtNewArrow().isVisible());

  for (auto qtedge: Collect<QtEdge>(*q.scene()))
  {
    q.scene()->removeItem(qtedge);
    delete qtedge;
  }

  for (auto qtnode: Collect<QtNode>(*q.scene()))
  {
    q.scene()->removeItem(qtnode);
    delete qtnode;
  }
}



void ribi::cmap::QtConceptMap::DoCommand(Command * const command) noexcept
{
  if (!command) return;

  CheckInvariants(*this);

  m_undo.push(command);

  CheckInvariants(*this);

  UpdateConceptMap(*this);

  CheckInvariants(*this);

  qApp->processEvents();

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
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  const QList<QGraphicsItem*> v = q.scene()->items(
    pos.x(),pos.y(),2.0,2.0,Qt::IntersectsItemShape,Qt::AscendingOrder
  );
  #else
  const QList<QGraphicsItem*> v = q.scene()->items(pos.x(),pos.y(),2.0,2.0);
  #endif
  std::vector<QtNode*> qtnodes;
  std::for_each(v.begin(),v.end(),
    [&qtnodes](QGraphicsItem* const item)
    {
      if (QtNode * const qtnode = dynamic_cast<QtNode*>(item))
      {
        assert(!dynamic_cast<QtTool*>(item) && "Cannot draw arrow to ToolsItem");
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

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetSelectedQtNodes(const QtConceptMap& q) noexcept
{
  return GetSelectedQtNodes(q.GetScene());
}

std::vector<ribi::cmap::QtNode *> ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(
  const QtConceptMap& q) noexcept
{
  return GetSelectedQtNodesAlsoOnQtEdge(q.GetScene());
}

const ribi::cmap::QtExamplesItem& ribi::cmap::QtConceptMap::GetQtExamplesItem() const noexcept
{
  assert(m_examples_item);
  return *m_examples_item;
}

ribi::cmap::QtExamplesItem& ribi::cmap::QtConceptMap::GetQtExamplesItem() noexcept
{
  assert(m_examples_item);
  return *m_examples_item;
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

void ribi::cmap::QtConceptMap::hideEvent(QHideEvent *)
{
  m_timer->stop();
}

void ribi::cmap::QtConceptMap::keyPressEvent(QKeyEvent *event)
{
  event->ignore();
  //event->setAccepted(false);
  CheckInvariants(*this);
  UpdateConceptMap(*this);
  CheckInvariants(*this);

  ProcessKey(*this, event);

  //Pass event to base class
  if (!event->isAccepted())
  {
    QtKeyboardFriendlyGraphicsView::keyPressEvent(event);
  }

  UpdateConceptMap(*this);

  CheckInvariants(*this);
}

void ribi::cmap::keyPressEventDelete(QtConceptMap& q, QKeyEvent *event) noexcept
{
  try
  {
    q.DoCommand(new CommandDeleteSelected(q));
    event->accept();
  }
  catch (std::exception&)
  {
    event->ignore();
  }
}

void ribi::cmap::keyPressEventE(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    try
    {
      q.DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(q));
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

void ribi::cmap::keyPressEventF1(QtConceptMap& q, QKeyEvent * const event) noexcept
{
  try
  {
    const auto items = q.scene()->selectedItems();
    if (items.size() != 1) return;
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(items.front()))
    {
      OnNodeKeyDownPressed(q, qtnode, event);
    }
  }
  catch (std::exception&) {} //!OCLINT Correct, nothing happens in catch
}

void ribi::cmap::keyPressEventF2(QtConceptMap& q, QKeyEvent * const event) noexcept
{
  try
  {
    const auto items = q.scene()->selectedItems();
    if (items.size() != 1) return;
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(items.front()))
    {
      OnNodeKeyDownPressed(q, qtnode, event);
    }
  }
  catch (std::exception&) {} //!OCLINT Correct, nothing happens in catch
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
  if (event->modifiers() & Qt::ControlModifier)
  {
    try
    {
      q.DoCommand(new CommandCreateNewNode(q, "", false, 0.0, 0.0));
    }
    catch (std::exception& e) {} //!OCLINT Correct, nothing happens in catch
  }
}

void ribi::cmap::keyPressEventQuestion(QtConceptMap& q, QKeyEvent *) noexcept
{
  UpdateConceptMap(q);
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

void ribi::cmap::MoveQtNodesAwayFromEachOther(ribi::cmap::QtConceptMap& q) noexcept
{
  for (const auto item: q.scene()->items())
  {
    if (!(item->flags() & QGraphicsItem::ItemIsMovable)) continue;
    QtNode* const node = dynamic_cast<QtNode*>(item);
    if (!node) continue;
    const auto others = item->collidingItems();
    for (const auto other: others)
    {
      const QtNode* const other_node = dynamic_cast<const QtNode*>(other);
      if (!other_node) continue;
      const double dx = node->x() - other_node->x() > 0.0 ? 1.0 : -1.0;
      const double dy = node->y() - other_node->y() > 0.0 ? 1.0 : -1.0;
      node->SetCenterPos(node->x()  + dx, node->y()  + dy);
    }
  }

}

void ribi::cmap::QtConceptMap::mouseDoubleClickEvent(QMouseEvent *event)
{
  CheckInvariants(*this);

  const QPointF pos = mapToScene(event->pos());

  //Do not create new nodes when double-clicking on an existing one
  if (scene()->itemAt(pos, QTransform())) return;

  //Create new node at the mouse cursor its position
  try
  {
    this->DoCommand(new CommandCreateNewNode(*this, "", false, pos.x(), pos.y()));
  }
  catch (std::logic_error& ) {} //!OCLINT This should be an empty catch statement
  UpdateExamplesItem(*this);

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::mouseMoveEvent(QMouseEvent * event)
{
  if (m_arrow->isVisible())
  {
    const QPointF pos = mapToScene(event->pos());
    m_arrow->SetHeadPos(pos.x(),pos.y());

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
    assert(m_highlighter);
    m_highlighter->SetItem(nullptr); //item_below is allowed to be nullptr
  }
  m_arrow->update();
  QtKeyboardFriendlyGraphicsView::mouseMoveEvent(event);
}

void ribi::cmap::QtConceptMap::mousePressEvent(QMouseEvent *event)
{
  UpdateConceptMap(*this);
  assert(!GetQtNewArrow().isSelected());
  if (GetQtNewArrow().isVisible())
  {
    assert(!GetQtNewArrow().isSelected());
    if (GetQtHighlighter().GetItem() && GetQtNewArrow().GetFrom() != GetQtHighlighter().GetItem())
    {
      //The command needs to find the two selected vertices
      for (auto& i: scene()->selectedItems()) { i->setSelected(false); }
      GetQtHighlighter().GetItem()->setSelected(true);
      GetQtNewArrow().GetFrom()->setSelected(true);
      try
      {
        const auto command = new CommandCreateNewEdgeBetweenTwoSelectedNodes(*this);
        this->DoCommand(command);
        UpdateConceptMap(*this);
        this->GetQtNewArrow().hide();
        this->GetQtHighlighter().SetItem(nullptr);
        event->setAccepted(true);
      }
      catch (std::logic_error&)
      {
        return;
      }
    }
  }
  //if (!event->isAccepted())
  {
    //qDebug() << "Not accepted yet";
    QtKeyboardFriendlyGraphicsView::mousePressEvent(event);
  }
  UpdateExamplesItem(*this);
  assert(!GetQtNewArrow().isSelected());
}

void ribi::cmap::QtConceptMap::Respond()
{
  if (!this->isEnabled())
  {
    qDebug() << "Warning: Respond called for disabled QtConceptMap";
    return;
  }
  if (!this->isVisible())
  {
    qDebug() << "Warning: Respond called for invisible QtConceptMap";
    return;
  }
  if (!this->scene())
  {
    qDebug() << "Warning: Respond called when there is no scene";
    return;
  }

  assert(this->isVisible());
  assert(this->isEnabled());
  qApp->processEvents();

  CheckInvariants(*this);
  MoveQtNodesAwayFromEachOther(*this);
  UpdateExamplesItem(*this);
  UpdateQtToolItem(*this);
  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::onFocusItemChanged(
  QGraphicsItem * newFocus, QGraphicsItem */*oldFocus*/, Qt::FocusReason reason
)
{
  //Focus on QtToolItem
  if (newFocus == &GetQtToolItem()
    && !GetQtNewArrow().isVisible()
    && GetQtToolItem().GetBuddyItem()
    && reason == Qt::MouseFocusReason
  )
  {
    GetQtNewArrow().Start(GetQtToolItem().GetBuddyItem()); //Also sets visibility
    GetQtToolItem().setSelected(false);

    //Will tigger onSelectionChanged and hide the arrow
    GetQtToolItem().GetBuddyItem()->setSelected(true);
    GetQtToolItem().GetBuddyItem()->setFocus();
    GetQtNewArrow().setVisible(true);
  }
  else
  {
    UpdateQtToolItem(*this);
    GetQtNewArrow().setVisible(false);

  }
}

void ribi::cmap::OnNodeKeyDownPressed(
  QtConceptMap& q,
  QtNode* const item,
  QKeyEvent * const event
)
{
  if (IsQtCenterNode(item))
  {
    event->ignore();
    return;
  }
  const int key{event->key()};

  //Note: item can also be the QtNode on a QtEdge
  assert(item);
  if (q.GetMode() == Mode::edit && key == Qt::Key_F2)
  {
    OnNodeKeyDownPressedEditF2(q, item, event);
  }
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F1)
  {
    OnNodeKeyDownPressedRateF1(q, item);
  }
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F2)
  {
    OnNodeKeyDownPressedRateF2(q, item);
  }

  q.show();
  q.setFocus();
  q.scene()->setFocusItem(item);
  item->setSelected(true);
  q.scene()->update();
}


void ribi::cmap::OnNodeKeyDownPressedEditF2(
  QtConceptMap& q,
  QtNode* const item,
  QKeyEvent * const event
)
{
  event->accept();

  //Edit concept
  QtConceptMapConceptEditDialog d(item->GetNode().GetConcept());
  q.setEnabled(false);
  //Block pop-ups in testing
  if (q.GetPopupMode() == PopupMode::normal)
  {
    d.exec();
  }
  q.setEnabled(true);

  //Find the original Node or Edge
  if (::has_custom_vertex_with_my_vertex(item->GetNode(), q.GetConceptMap()))
  {
    assert(::has_custom_vertex_with_my_vertex(item->GetNode(), q.GetConceptMap()));
    const auto vd = ::find_first_custom_vertex_with_my_vertex(item->GetNode(), q.GetConceptMap());
    //Update the node here
    auto node = item->GetNode();
    node.SetConcept(d.GetConcept());
    //Update the node in the concept map
    set_my_custom_vertex(node, vd, q.GetConceptMap());
  }
  else
  {
    //It is a node on an edge
    //Find the first (and hopefully only) edge with the node on it
    Node node = item->GetNode();
    const auto ed = ::find_first_custom_edge(
      [node](const Edge& e) { return e.GetNode() == node; },
      q.GetConceptMap()
    );
    //Get hold of the Edge
    Edge edge = ::get_my_custom_edge(ed, q.GetConceptMap());
    //Update the Edge here
    node.SetConcept(d.GetConcept());
    edge.SetNode(node);
    //Update the node in the concept map
    set_my_custom_edge(edge, ed, q.GetConceptMap());
  }
  //Update the QtNode
  item->GetNode().SetConcept(d.GetConcept());
  //Set the word-wrapped text
  item->SetText(Wordwrap(d.GetConcept().GetName(), GetWordWrapLength()));
}

void ribi::cmap::OnNodeKeyDownPressedRateF1(QtConceptMap& q, QtNode* const item)
{
  //Rate concept
  const auto vd = ::find_first_custom_vertex_with_my_vertex(
    item->GetNode(), q.GetConceptMap()
  );
  const auto subgraph
    = create_direct_neighbour_custom_and_selectable_edges_and_vertices_subgraph(
      vd,
      q.GetConceptMap()
    );
  ribi::cmap::QtRateConceptDialog d(subgraph);
  q.setEnabled(false);
  d.exec();
  q.setEnabled(true);
  if (d.GetOkClicked())
  {
    //Find the original Node
    //const auto vd = FindNode(item->GetNode(), m_conceptmap);
    //Update the node here
    auto node = item->GetNode();
    node.GetConcept().SetRatingComplexity(d.GetComplexity());
    node.GetConcept().SetRatingConcreteness(d.GetConcreteness());
    node.GetConcept().SetRatingSpecificity(d.GetSpecificity());
    //Update the node in the concept map
    set_my_custom_vertex(node, vd, q.GetConceptMap());
    //Update the QtNode
    item->GetNode().SetConcept(node.GetConcept());
    const int n_rated
      = (node.GetConcept().GetRatingComplexity()   == -1 ? 0 : 1)
      + (node.GetConcept().GetRatingConcreteness() == -1 ? 0 : 1)
      + (node.GetConcept().GetRatingSpecificity()  == -1 ? 0 : 1);
    switch (n_rated)
    {
      case 0:
        item->setBrush(QtBrushFactory().CreateRedGradientBrush());
        break;
      case 1:
      case 2:
        item->setBrush(QtBrushFactory().CreateYellowGradientBrush());
        break;
      case 3:
        item->setBrush(QtBrushFactory().CreateGreenGradientBrush());
        break;
      default:
        throw std::logic_error(
          "ribi::cmap::OnNodeKeyDownPressed: invalid n_rated"
        );
    }
  }
}

void ribi::cmap::OnNodeKeyDownPressedRateF2(QtConceptMap& q, QtNode* const item)
{
  //Rate examples
  if (item->GetNode().GetConcept().GetExamples().Get().empty()) return;
  ribi::cmap::QtRateExamplesDialog d(item->GetNode().GetConcept());
  q.setEnabled(false);
  d.exec();
  q.setEnabled(true);
  //Find the original Node
  const auto vd = ::find_first_custom_vertex_with_my_vertex(item->GetNode(), q.GetConceptMap());
  //Update the node here
  auto node = item->GetNode();
  node.GetConcept().SetExamples(d.GetRatedExamples());
  //Update the node in the concept map
  set_my_custom_vertex(node, vd, q.GetConceptMap());
  //Update the QtNode
  item->GetNode().GetConcept().SetExamples(d.GetRatedExamples());
}

void ribi::cmap::QtConceptMap::onSelectionChanged()
{
  ConceptMap& g = this->GetConceptMap();

  //Selectness of vertices
  const auto vip = vertices(g);
  std::for_each(vip.first,vip.second,
    [this, &g](const VertexDescriptor vd) {
      const auto vertex_map = get(boost::vertex_custom_type,g);
      const auto is_selected_map = get(boost::vertex_is_selected,g);
      const auto qtnode = FindQtNode(get(vertex_map,vd).GetId(), *scene());
      //qtnode can be nullptr when onSelectionChanged is called
      //while building up the QtConceptMap
      if (qtnode) { put(is_selected_map,vd,qtnode->isSelected()); }
    }
  );

  //Selectness of edges
  const auto eip = edges(g);
  std::for_each(eip.first,eip.second,
    [this, &g](const EdgeDescriptor ed) {
      const auto edge_map = get(boost::edge_custom_type,g);
      const auto is_selected_map = get(boost::edge_is_selected,g);
      const auto qtedge = FindQtEdge(get(edge_map,ed).GetId(), *scene());
      if (qtedge) { put(is_selected_map,ed,qtedge->isSelected()); }
    }
  );
  scene()->update();
}

void ribi::cmap::ProcessKey(QtConceptMap& q, QKeyEvent * const event) //!OCLINT Although the NCSS is high, the code is easy to read
{

  //Pass event
  switch (event->key())
  {
    case Qt::Key_Delete: keyPressEventDelete(q, event); break;
    case Qt::Key_E: keyPressEventE(q, event); break;
    case Qt::Key_Equal: q.scale(1.1,1.1); break;
    case Qt::Key_Escape: keyPressEventEscape(q, event); break;
    case Qt::Key_F1: keyPressEventF1(q, event); break;
    case Qt::Key_F2: keyPressEventF2(q, event); break;
    case Qt::Key_F4: keyPressEventF4(q, event); break;
    #ifndef NDEBUG
    case Qt::Key_F8: MessUp(*q.scene()); break;
    case Qt::Key_F9: std::exit(1); break; //Cause a deliberate hard crash
    #endif
    case Qt::Key_H: keyPressEventH(q, event); break;
    case Qt::Key_Minus: q.scale(0.9,0.9); break;
    case Qt::Key_N: keyPressEventN(q, event); break;
    case Qt::Key_Question: keyPressEventQuestion(q, event); break;
    case Qt::Key_T: keyPressEventT(q, event); break;
    case Qt::Key_Z: keyPressEventZ(q, event); break;
    case Qt::Key_Space: keyPressEventSpace(q, event); break;
    default: break;
  }

}


void ribi::cmap::QtConceptMap::Redo() noexcept
{
  m_undo.redo();
}

void ribi::cmap::SaveToFile(const QtConceptMap& q, const std::string& dot_filename)
{
  SaveToFile(q.GetConceptMap(), dot_filename);
}


void ribi::cmap::QtConceptMap::SetConceptMap(const ConceptMap& conceptmap)
{
  CheckInvariants(*this);
  RemoveConceptMap(*this);
  m_conceptmap = ConceptMap();
  QtNode* const no_qtnode = nullptr;
  SetQtExamplesBuddy(*this, no_qtnode);
  SetQtToolItemBuddy(*this, no_qtnode);
  CheckInvariants(*this);
  m_conceptmap = conceptmap;
  assert(GetConceptMap() == conceptmap);
  assert(Collect<QtNode>(*scene()).empty());
  AddNodesToScene(*this);
  AddEdgesToScene(*this);
  CheckInvariants(*this);

  //The new concept map must be displayed in full
  this->fitInView(this->sceneRect());
}

void ribi::cmap::SetFocus(QtConceptMap& q, QtNode* const new_focus_item)
{
  assert(new_focus_item);
  assert(!new_focus_item->isSelected());

  new_focus_item->setSelected(true);
  new_focus_item->setFocus();
  if (HasExamples(*new_focus_item))
  {
    SetQtExamplesBuddy(q, new_focus_item);
  }
  SetQtToolItemBuddy(q, new_focus_item);
  q.update();
  if (!new_focus_item->isSelected())
  {
    qDebug() << "Warning: SetFocus did not select the item";
  }
  if (!new_focus_item->hasFocus())
  {
    qDebug() << "Warning: SetFocus did not set focus to the item";
  }
  CheckInvariants(q);
}

void ribi::cmap::QtConceptMap::SetMode(const ribi::cmap::Mode mode) noexcept
{
  m_mode = mode;
  auto qtnodes = GetQtNodesAlsoOnQtEdge(*scene());
  for (auto qtnode: qtnodes)
  {
    const auto f = GetQtNodeBrushFunction(m_mode);
    qtnode->SetBrushFunction(f);
    switch (mode)
    {
      case Mode::edit:
        qtnode->setFlags(
            QGraphicsItem::ItemIsMovable
          | QGraphicsItem::ItemIsFocusable
          | QGraphicsItem::ItemIsSelectable
        );
      break;
      case Mode::rate:
        qtnode->setFlags(
            QGraphicsItem::ItemIsFocusable
          | QGraphicsItem::ItemIsSelectable
        );
      break;
      case Mode::uninitialized:
        qtnode->setFlags(0);
      break;
    }
  }
  if (QtNode * const qtnode = FindQtCenterNode(*scene()))
  {
    if (m_mode == Mode::edit)
    {
      qtnode->setFlags(
          QGraphicsItem::ItemIsFocusable
        | QGraphicsItem::ItemIsSelectable
      );
    }
    else
    {
      qtnode->setFlags(0);
    }
  }
}

void ribi::cmap::QtConceptMap::SetPopupMode(
  const ribi::cmap::PopupMode mode
) noexcept
{
  m_popup_mode = mode;
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

  //Pick random Focusable non-selected QtNotde
  static std::mt19937 rng_engine{0};
  std::uniform_int_distribution<int> distribution(0, static_cast<int>(items.size()) - 1);
  const int i{distribution(rng_engine)};
  assert(i >= 0);
  assert(i < static_cast<int>(items.size()));
  QtNode * const new_focus_item = dynamic_cast<QtNode*>(items[i]);
  assert(new_focus_item);
  assert(!new_focus_item->isSelected());

  if (QGraphicsItem * const old_focus_item = q.GetScene().focusItem())
  {
    //Really loose focus, but not selectedness
    old_focus_item->setEnabled(false);
    old_focus_item->setSelected(false);
    old_focus_item->clearFocus();
    old_focus_item->setEnabled(true);
    old_focus_item->setSelected(true);
  }
  SetFocus(q, new_focus_item);

  CheckInvariants(q);
}

void ribi::cmap::SetRandomFocusExclusive(
  QtConceptMap& q
)
{
  CheckInvariants(q);
  UnselectAllItems(q);
  ReallyLoseFocus(q);

  //Let a random QtNode receive focus
  const auto items = GetFocusableNonselectedItems(q);

  if (items.empty()) return;

  static std::mt19937 rng_engine{0};
  std::uniform_int_distribution<int> distribution(0, static_cast<int>(items.size()) - 1);
  const int i{distribution(rng_engine)};
  assert(i >= 0);
  assert(i < static_cast<int>(items.size()));
  QtNode * const new_focus_item = dynamic_cast<QtNode*>(items[i]);
  assert(new_focus_item);
  assert(!new_focus_item->isSelected());
  SetFocus(q, new_focus_item);

  CheckInvariants(q);
}

void ribi::cmap::SetQtExamplesBuddy(QtConceptMap& q, QtEdge * const qtedge)
{
  SetQtExamplesBuddy(q, qtedge->GetQtNode());
}

void ribi::cmap::SetQtExamplesBuddy(QtConceptMap& q, QtNode * const qtnode)
{
  if (qtnode)
  {
    assert(HasExamples(*qtnode));
  }
  q.GetQtExamplesItem().SetBuddyItem(qtnode);
}


void ribi::cmap::SetQtToolItemBuddy(QtConceptMap& q, QtNode * const qtnode)
{
  q.GetQtToolItem().SetBuddyItem(qtnode);
}

void ribi::cmap::SetQtToolItemBuddy(QtConceptMap& q, QtEdge * const qtedge)
{
  SetQtToolItemBuddy(q, qtedge->GetQtNode());
}

void ribi::cmap::QtConceptMap::showEvent(QShowEvent *)
{
  m_timer->start(10);
  // this->fitInView(this->sceneRect()); //Don't!
  // Caused https://github.com/richelbilderbeek/Brainweaver/issues/125
}

void ribi::cmap::QtConceptMap::StartTimer()
{
  m_timer->start();
}

void ribi::cmap::QtConceptMap::StopTimer()
{
  m_timer->stop();
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
  m_undo.undo();
  CheckInvariants(*this);
}

void ribi::cmap::UpdateConceptMap(QtConceptMap& q)
{
  for (const auto item: q.scene()->items()) { item->update(); }
  q.onSelectionChanged();
  UpdateExamplesItem(q);
  q.update();
  q.scene()->update();
}

void ribi::cmap::UpdateExamplesItem(QtConceptMap& q)
{
  //If nothing is selected, hide the Examples
  q.GetQtExamplesItem().SetBuddyItem(nullptr); //Handles visibility
  const auto selected_qtnodes = GetSelectedQtNodesAlsoOnQtEdge(*q.scene());
  if (selected_qtnodes.size() == 1)
  {
    const auto selected_qtnode = selected_qtnodes[0];
    q.GetQtExamplesItem().SetBuddyItem(selected_qtnode); //Handles visibility
  }
  q.update();
  q.show();
  q.scene()->update();
  //qApp->processEvents();
  CheckInvariantOneQtNodeWithExamplesHasExamplesItem(q);
}

void ribi::cmap::UpdateQtToolItem(QtConceptMap& q)
{
  const auto selected_qtnodes = GetSelectedQtNodesAlsoOnQtEdge(*q.scene());
  if (selected_qtnodes.size() == 1)
  {
    const auto selected_qtnode = selected_qtnodes[0];
    q.GetQtToolItem().SetBuddyItem(selected_qtnode);
  }
  else
  {
    q.GetQtToolItem().SetBuddyItem(nullptr);
  }
}

void ribi::cmap::QtConceptMap::wheelEvent(QWheelEvent *event)
{
  const double s{1.1};
  if (event->delta() > 0)
  {
    this->scale(s,s);
  }
  else if (event->delta() < 0)
  {
    this->scale(1.0 / s,1.0 / s);
  }

}
