#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmap.h"

#include <iostream>
#include <boost/numeric/conversion/cast.hpp>


#include <QKeyEvent>
#include <QDebug>

#include <gsl/gsl_assert>

#include "add_custom_and_selectable_edge_between_vertices.h"
#include "add_custom_and_selectable_vertex.h"
#include "conceptmaphelper.h"
#include "count_vertices_with_selectedness.h"
#include "create_direct_neighbour_custom_and_selectable_edges_and_vertices_subgraph.h"
#include "find_first_custom_edge_with_my_edge.h"
#include "find_first_custom_vertex.h"
#include "find_first_custom_vertex_with_my_vertex.h"
#include "get_my_custom_edge.h"
#include "has_custom_edge.h"
#include "has_custom_vertex_with_my_vertex.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapcollect.h"
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
#include "qtconceptmapconcepteditdialog.h"
#include "qtconceptmapexamplesitem.h"
#include "qtconceptmaphelper.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmapnewarrow.h"
#include "qtconceptmaprateconceptdialog.h"
#include "qtconceptmapitemhighlighter.h"
#include "qtconceptmaprateexamplesdialog.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmaptoolsitem.h"
#include "qtquadbezierarrowitem.h"
#include "set_my_custom_vertex.h"
#include "set_vertex_selectedness.h"
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
  #ifdef KEEP_UNUSED_SLOTS_20170924
  QObject::connect(
    scene(),
    SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)), //DOES NOT EXIST
    this,
    SLOT(onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason))
  );
  QObject::connect(scene(),SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));
  #endif  // KEEP_UNUSED_SLOTS_20170924

  m_examples_item->SetCenterPos(123,456); //Irrelevant where

  CheckInvariants(*this);

  if (1 == 2) //!OCLINT Temporarily remove timer for bugfixing
  {
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(Respond()));
    //Timer is started in showEvent
  }
  CheckInvariants(*this);
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
    //const auto edge_map = get(boost::edge_custom_type, conceptmap);
    //const Edge edge = get get(edge_map, *i);
    const Edge edge = get_my_custom_edge(*i, conceptmap);
    QtEdge * const qtedge{new QtEdge(edge,qtfrom,qtto)};
    assert(GetX(*qtedge) == GetX(edge));
    assert(GetY(*qtedge) == GetY(edge));
    if (IsConnectedToCenterNode(*qtedge))
    {
      qtedge->GetQtNode()->setVisible(false);
    }
    assert(qtedge && HasScene(*qtedge, nullptr));
    scene.addItem(qtedge);
    //scene()->addItem(qtedge->GetQtNode()); //Get these for free when adding a QtEdge
    //scene()->addItem(qtedge->GetArrow()); //Get these for free when adding a QtEdge
    assert(HasScene(*qtedge, &scene));
    assert(GetX(*qtedge) == GetX(edge));
    assert(GetY(*qtedge) == GetY(edge));
    CheckInvariants(*qtedge);
    CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(qtconceptmap);
  }
  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(qtconceptmap);
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
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(qtconceptmap);
}

void ribi::cmap::AddQtEdge(
  QtEdge * const qtedge,
  QtConceptMap& q
)
{
  CheckInvariantAsMuchEdgesAsQtEdges(q);

  assert(qtedge);
  assert(!qtedge->scene());
  assert(!q.GetScene().items().contains(qtedge));
  assert(has_custom_vertex_with_my_vertex(qtedge->GetFrom()->GetNode(), q.GetConceptMap()));
  assert(has_custom_vertex_with_my_vertex(qtedge->GetTo()->GetNode(), q.GetConceptMap()));

  const auto vd_from = find_first_custom_vertex_with_my_vertex(
    qtedge->GetFrom()->GetNode(), q.GetConceptMap());
  const auto vd_to = find_first_custom_vertex_with_my_vertex(
    qtedge->GetTo()->GetNode(), q.GetConceptMap());

  add_custom_and_selectable_edge_between_vertices(
    qtedge->GetEdge(),
    true,
    vd_from,
    vd_to,
    q.GetConceptMap()
  );
  q.GetScene().addItem(qtedge);
  assert(qtedge->scene());
  qtedge->setZValue(GetQtEdgeZvalue());

  CheckInvariantAsMuchEdgesAsQtEdges(q);
}

void ribi::cmap::AddQtNode(
  QtNode * const qtnode,
  QtConceptMap& q
)
{
  CheckInvariantAsMuchNodesAsQtNodes(q);

  assert(qtnode);
  assert(!qtnode->scene());
  assert(!q.GetScene().items().contains(qtnode));
  add_custom_and_selectable_vertex(qtnode->GetNode(), true, q.GetConceptMap());
  qtnode->setSelected(true);
  q.GetScene().addItem(qtnode);
  assert(qtnode->scene());

  CheckInvariantAsMuchNodesAsQtNodes(q);
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
  const QtConceptMap& q
) noexcept
{
  #ifndef NDEBUG
  //All QtEdges, their QtNodes and Arrows must have a scene
  for (const auto qtedge: GetQtEdges(*q.scene()))
  {
    if (!qtedge->GetFrom()->scene())
    {
      qCritical()
        << "QtEdge '"
        << GetText(*qtedge).c_str()
        << "' has source QtNode '"
        << GetText(*qtedge->GetFrom()).c_str()
        << "' without scene";
    }
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

void ribi::cmap::CheckInvariantAsMuchEdgesAsQtEdges(const QtConceptMap& q) noexcept
{
  const int n_qtedges{CountQtEdges(q)};
  const int n_edges{boost::numeric_cast<int>(boost::num_edges(q.GetConceptMap()))};
  if (n_qtedges != n_edges)
  {
    std::cerr << "n_qtedges (" << n_qtedges
      << ") mismatch n_edges (" << n_edges << ")\n";
  }
  assert(n_qtedges == n_edges);
}

void ribi::cmap::CheckInvariantAsMuchNodesAsQtNodes(const QtConceptMap& q) noexcept
{
  const int n_qtnodes{CountQtNodes(q)};
  const int n_nodes{boost::numeric_cast<int>(boost::num_vertices(q.GetConceptMap()))};
  if (n_qtnodes != n_nodes)
  {
    std::cerr << "n_qtnodes (" << n_qtnodes
      << ") mismatch n_nodes (" << n_nodes << ")\n";
  }
  assert(n_qtnodes == n_nodes);
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

void ribi::cmap::CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(const QtConceptMap&
  #ifndef NDEBUG
  q
  #endif
) noexcept
{
  #ifndef NDEBUG
  for (const QtEdge * const qtedge: GetQtEdges(q))
  {
    assert(qtedge);
    const double edge_x{GetX(qtedge->GetEdge())};
    const double qtedge_x{GetX(*qtedge)};
    assert(std::abs(edge_x - qtedge_x) < 2.0);

    const double edge_y{GetY(qtedge->GetEdge())};
    const double qtedge_y{GetY(*qtedge)};
    assert(std::abs(edge_y - qtedge_y) < 2.0);
  }
  #endif // NDEBUG
}

void ribi::cmap::CheckInvariantQtNodesAndNodesHaveSameCoordinats(const QtConceptMap&
  #ifndef NDEBUG
  q
  #endif
) noexcept
{
  #ifndef NDEBUG
  for (const QtNode * const qtnode: GetQtNodesAlsoOnQtEdge(q))
  {
    assert(qtnode);
    const double node_x{GetX(qtnode->GetNode())};
    const double qtnode_x{qtnode->GetCenterX()};
    assert(std::abs(node_x - qtnode_x) < 2.0);

    const double node_y{GetY(qtnode->GetNode())};
    const double qtnode_y{qtnode->GetCenterY()};
    assert(std::abs(node_y - qtnode_y) < 2.0);
  }
  #endif // NDEBUG
}

void ribi::cmap::CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(const QtConceptMap& q) noexcept
{
  QtNode * const qtnode = q.GetQtToolItem().GetBuddyItem();
  assert(!qtnode || IsQtNodeNotOnEdge(qtnode, q));
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
    assert(has_custom_edge(
        [id = qtedge->GetEdge().GetId()](const Edge& edge) { return edge.GetId() == id; },
        q.GetConceptMap()
      )
    );
    const auto edge = ExtractTheOneSelectedEdge(q.GetConceptMap(), *q.scene());
    assert(qtedge->GetEdge().GetId() == edge.GetId());
  }
  catch (...) {} //!OCLINT This should be an empty catch statement
  #endif // NDEBUG
}

void ribi::cmap::CheckInvariants(const QtConceptMap&
  #ifndef NDEBUG
  q
  #endif
) noexcept
{
  #ifndef NDEBUG
  assert(q.GetQtNewArrow().scene());
  assert(q.GetQtExamplesItem().scene());
  assert(q.GetQtToolItem().scene());
  CheckInvariantAsMuchNodesAsQtNodes(q);
  CheckInvariantAsMuchEdgesAsQtEdges(q);
  CheckInvariantAsMuchNodesAsQtNodesSelected(q);
  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(q);
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(q);
  CheckInvariantAllQtNodesHaveAscene(q);
  CheckInvariantAllQtEdgesHaveAscene(q);
  CheckInvariantSingleSelectQtEdgeMustHaveCorrespondingEdge(q);
  CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(q);
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

ribi::cmap::QtNode * ribi::cmap::FindQtNode(
  const int node_id,
  const QtConceptMap& q) noexcept
{
  return FindQtNode(node_id, q.GetScene());
}

ribi::cmap::QtEdge * ribi::cmap::GetFirstQtEdge(const QtConceptMap& q) noexcept
{
  return GetFirstQtEdge(q.GetScene());
}

ribi::cmap::QtNode * ribi::cmap::GetFirstQtNode(const QtConceptMap& q) noexcept
{
  return GetFirstQtNode(q.GetScene());
}

void ribi::cmap::HideExamplesItem(QtConceptMap& q) noexcept
{
  q.GetQtExamplesItem().hide();
}

bool ribi::cmap::IsArrowVisible(QtConceptMap& q) noexcept
{
  return q.GetQtNewArrow().isVisible();
}

bool ribi::cmap::IsInScene(const QtEdge& qtedge, const QGraphicsScene& scene) noexcept
{
  return scene.items().contains(const_cast<QtEdge*>(&qtedge));
}

bool ribi::cmap::IsOnEdge(const QtNode& qtnode, const QtConceptMap& q) noexcept
{
  return IsOnEdge(&qtnode, q.GetScene());
}

bool ribi::cmap::IsQtCenterNodeSelected(const QtConceptMap& q)
{
  return IsQtCenterNodeSelected(q.GetScene());
}

bool ribi::cmap::IsQtNodeNotOnEdge(
  const QGraphicsItem * const item,
  const QtConceptMap& q
) noexcept
{
  return IsQtNodeNotOnEdge(item, q.GetScene());
}

bool ribi::cmap::IsQtNodeOnEdge(
  const QGraphicsItem * const item,
  const QtConceptMap& q
) noexcept
{
  return IsQtNodeOnEdge(item, q.GetScene());
}

void ribi::cmap::RemoveConceptMap(QtConceptMap& q)
{
  q.clearFocus();
  q.GetQtNewArrow().hide();
  q.GetQtExamplesItem().hide();
  q.GetQtHighlighter().SetItem(nullptr); //Do this before destroying items
  q.GetQtToolItem().SetBuddyItem(nullptr);
  assert(!q.GetQtNewArrow().isVisible());

  for (auto qtedge: Collect<QtEdge>(*q.scene()))
  {
    assert(qtedge);
    //SetSelectedness(false, *qtedge, q);
    q.scene()->removeItem(qtedge);
    delete qtedge;
  }

  for (auto qtnode: Collect<QtNode>(*q.scene()))
  {
    SetSelectedness(false, *qtnode, q);
    q.scene()->removeItem(qtnode);
    delete qtnode;
  }
}



void ribi::cmap::QtConceptMap::DoCommand(Command * const command)
{
  if (!command) return;

  CheckInvariants(*this);

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

std::vector<ribi::cmap::QtEdge *> ribi::cmap::GetQtEdges(const QtConceptMap& q) noexcept
{
  return GetQtEdges(q.GetScene());
}

ribi::cmap::QtNode * ribi::cmap::GetQtExamplesItemBuddy(const QtConceptMap& q) noexcept
{
  return q.GetQtExamplesItem().GetBuddyItem();
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
  CheckInvariants(*this);

  m_timer->stop();

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::keyPressEvent(QKeyEvent *event)
{
  CheckInvariants(*this);

  event->ignore();
  //event->setAccepted(false);
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

void ribi::cmap::keyPressEventArrows(QtConceptMap& q, QKeyEvent *event) noexcept
{
  CheckInvariants(q);
  if (!event->modifiers() || (event->modifiers() &  Qt::ShiftModifier))
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

  assert(!dynamic_cast<QtTool*>(item));
  assert(!dynamic_cast<QtExamplesItem*>(item));
  assert(!dynamic_cast<QtNewArrow*>(item));
  assert(!dynamic_cast<QtItemHighlighter*>(item));

  if (!item) return;

  assert(dynamic_cast<QtNode*>(item) || dynamic_cast<QtEdge*>(item));

  //If select exclusively (no shift), unselect all current select items
  if (!event->modifiers())
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
  CheckInvariants(q);

  event->ignore();

  double dx{0.0};
  double dy{0.0};
  switch (event->key())
  {
    case Qt::Key_Up: dy = -10.0; break;
    case Qt::Key_Right: dx =  10.0; break;
    case Qt::Key_Down: dy =  10.0; break;
    case Qt::Key_Left: dx = -10.0; break;
    default:
      assert(!"Should not get here"); //!OCLINT accepted idiom
      break;
  }

  //Move edges
  for (QtEdge * const qtedge: GetQtEdges(q))
  {
    if (IsSelected(*qtedge)
      && IsMovable(*qtedge))
    {
      q.DoCommand(new CommandMoveEdge(q, qtedge, dx, dy));
      event->accept();
    }
  }

  //Move nodes
  for (QtNode * const qtnode: GetQtNodes(q))
  {
    if (IsSelected(*qtnode)
      && IsQtNodeNotOnEdge(qtnode, q)
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
  CheckInvariants(q);

  try
  {
    q.DoCommand(new CommandDeleteSelected(q));
    event->accept();
  }
  catch (std::exception&)
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
      OnNodeKeyDownPressed(q, *qtnode, event);
    }
  }
  catch (std::exception&) {} //!OCLINT Correct, nothing happens in catch
}

void ribi::cmap::keyPressEventF2(QtConceptMap& q, QKeyEvent * const event) noexcept
{
  CheckInvariants(q);
  try
  {
    const auto items = q.scene()->selectedItems();
    if (items.size() != 1) return;
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(items.front()))
    {
      OnNodeKeyDownPressed(q, *qtnode, event);
    }
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
  if (event->modifiers() & Qt::ControlModifier)
  {
    try
    {
      q.DoCommand(new CommandCreateNewNode(q, "", false, 0.0, 0.0));
    }
    catch (std::exception& e) {} //!OCLINT Correct, nothing happens in catch
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
    if (QtEdge * const qtedge = dynamic_cast<QtEdge*>(item))
    {
      Move(*qtedge, dx, dy);
    }
    else
    {
      QtNode * const qtnode = dynamic_cast<QtNode*>(item);
      if (qtnode && IsQtNodeNotOnEdge(qtnode, q))
      {
        Move(*qtnode, dx, dy);
      }
    }
  }
  CheckInvariants(q);
}

void ribi::cmap::MoveQtEdge(QtEdge& qtedge, const double dx, const double dy, QtConceptMap& q)
{
  //Model
  const auto ed = find_first_custom_edge(
    [id = qtedge.GetEdge().GetId()](const Edge& edge)
    {
      return edge.GetId() == id;
    },
    q.GetConceptMap()
  );
  const auto my_custom_edges_map = get(boost::edge_custom_type, q.GetConceptMap());
  Edge& edge = get(my_custom_edges_map, ed);
  Move(edge, dx, dy);

  //View
  Move(qtedge, dx, dy);

  //Make QtToolItem follow
  q.GetQtToolItem().Reposition();
  q.GetScene().update();
}

void ribi::cmap::MoveQtNode(QtNode& qtnode, const double dx, const double dy, QtConceptMap& q)
{
  //Model
  const auto vd = find_first_custom_vertex(
    [id = qtnode.GetNode().GetId()](const Node& node)
    {
      return node.GetId() == id;
    },
    q.GetConceptMap()
  );
  const auto vertices_map = get(boost::vertex_custom_type, q.GetConceptMap());
  Node& node = get(vertices_map, vd);
  Move(node, dx, dy);

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
    QtNode* const qtnode = dynamic_cast<QtNode*>(item);
    if (!qtnode) continue;
    const auto others = item->collidingItems();
    for (const auto other: others)
    {
      const QtNode* const other_qtnode = dynamic_cast<const QtNode*>(other);
      if (!other_qtnode) continue;

      const double dx = qtnode->x() - other_qtnode->x() > 0.0 ? 1.0 : -1.0;
      const double dy = qtnode->y() - other_qtnode->y() > 0.0 ? 1.0 : -1.0;

      if (IsQtNodeOnEdge(qtnode, q.GetScene()))
      {
        QtEdge * const qtedge = FindQtEdge(qtnode, q.GetScene());
        assert(qtedge);
        Move(*qtedge, dx, dy);
      }
      else
      {
        assert(IsQtNodeNotOnEdge(qtnode, q.GetScene()));
        Move(*qtnode, dx, dy);
      }
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
  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::mouseMoveEvent(QMouseEvent * event)
{
  CheckInvariants(*this);

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
  CheckInvariants(*this);

  m_arrow->update();

  CheckInvariants(*this);

  QtKeyboardFriendlyGraphicsView::mouseMoveEvent(event);

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::mousePressEvent(QMouseEvent *event)
{
  CheckInvariants(*this);

  if (IsArrowVisible(*this))
  {
    mousePressEventArrowActive(*this, event);
  }
  else
  {
    mousePressEventNoArrowActive(*this, event);
  }

  CheckInvariants(*this);
}

void ribi::cmap::mousePressEventNoArrowActive(QtConceptMap& q, QMouseEvent *event) noexcept
{
  CheckInvariants(q);

  const QPointF pos = q.mapToScene(event->pos());
  QGraphicsItem * const item = q.GetScene().itemAt(pos, QTransform());

  if (!item)
  {
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

  if (dynamic_cast<QtTool*>(item))
  {
    qDebug() << "\nClicked on QtTool, "
      << "which does nothing in mousePressEventNoArrowActive";
    event->ignore();
    return;
  }
  assert(dynamic_cast<QtEdge*>(item) || dynamic_cast<QtNode*>(item));
  try
  {
    if (item->isSelected())
    {
      q.DoCommand(new CommandUnselect(q, *item));
    }
    else
    {
      q.DoCommand(new CommandSelect(q, *item));
    }
    event->accept();
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
    assert(!q.GetQtNewArrow().isSelected());
    if (q.GetQtHighlighter().GetItem()
      && q.GetQtNewArrow().GetFrom() != q.GetQtHighlighter().GetItem())
    {
      //The command needs to find the two selected vertices
      for (auto& i: q.GetScene().selectedItems()) { i->setSelected(false); }
      q.GetQtHighlighter().GetItem()->setSelected(true);
      q.GetQtNewArrow().GetFrom()->setSelected(true);
      try
      {
        const auto command = new CommandCreateNewEdgeBetweenTwoSelectedNodes(q);
        q.DoCommand(command);
        q.GetQtNewArrow().hide();
        q.GetQtHighlighter().SetItem(nullptr);
      }
      catch (std::logic_error&)
      {
        return;
      }
    }
  }

  assert(!q.GetQtNewArrow().isSelected());
  CheckInvariants(q);
}

void ribi::cmap::QtConceptMap::Respond()
{
  //Do not check invariants here! These may be checked in the middle of another process
  //CheckInvariants(*this);

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

  //CheckInvariants(*this);


  MoveQtNodesAwayFromEachOther(*this);

  #ifdef REALLY_KEEP_RESPOND_OR_SHOULD_THE_COMMANDS_BE_CORRECT
  UpdateExamplesItem(*this);
  UpdateQtToolItem(*this);
  #endif

  //TODO: Move the QtNodes at the QtEdges connect to the center QtNode
  //to in the middle between source and target of the QtEdge

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::onFocusItemChanged(
  QGraphicsItem * newFocus, QGraphicsItem */*oldFocus*/, Qt::FocusReason reason
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

    //Will tigger onSelectionChanged and hide the arrow
    GetQtToolItem().GetBuddyItem()->setSelected(true);

    qDebug() << __func__ << ": setFocus";
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
  }
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F1)
  {
    OnNodeKeyDownPressedRateF1(q, qtnode);
  }
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F2)
  {
    OnNodeKeyDownPressedRateF2(q, qtnode);
  }

  q.show();
  q.setFocus();
  q.scene()->setFocusItem(&qtnode);
  qtnode.setSelected(true);
  q.scene()->update();
}


void ribi::cmap::OnNodeKeyDownPressedEditF2(
  QtConceptMap& q,
  QtNode& qtnode,
  QKeyEvent * const event
)
{
  event->accept();

  //Edit concept
  QtConceptMapConceptEditDialog d(qtnode.GetNode().GetConcept());
  q.setEnabled(false);
  //Block pop-ups in testing
  if (q.GetPopupMode() == PopupMode::normal)
  {
    d.exec();
  }
  q.setEnabled(true);

  assert(GetSelectedQtNodesAlsoOnQtEdge(q).size() == 1);
  assert(GetSelectedQtNodesAlsoOnQtEdge(q)[0] == &qtnode);
  q.DoCommand(new CommandSetConcept(q, d.GetConcept()));

  CheckInvariants(q);
}

void ribi::cmap::OnNodeKeyDownPressedRateF1(
  QtConceptMap& q,
  QtNode& item)
{
  //Rate concept
  const auto vd = ::find_first_custom_vertex_with_my_vertex(
    item.GetNode(), q.GetConceptMap()
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
    auto node = item.GetNode();
    node.GetConcept().SetRatingComplexity(d.GetComplexity());
    node.GetConcept().SetRatingConcreteness(d.GetConcreteness());
    node.GetConcept().SetRatingSpecificity(d.GetSpecificity());
    //Update the node in the concept map
    set_my_custom_vertex(node, vd, q.GetConceptMap());
    //Update the QtNode
    item.GetNode().SetConcept(node.GetConcept());
    const int n_rated
      = (node.GetConcept().GetRatingComplexity()   == -1 ? 0 : 1)
      + (node.GetConcept().GetRatingConcreteness() == -1 ? 0 : 1)
      + (node.GetConcept().GetRatingSpecificity()  == -1 ? 0 : 1);
    switch (n_rated)
    {
      case 0:
        item.setBrush(QtBrushFactory().CreateRedGradientBrush());
        break;
      case 1:
      case 2:
        item.setBrush(QtBrushFactory().CreateYellowGradientBrush());
        break;
      case 3:
        item.setBrush(QtBrushFactory().CreateGreenGradientBrush());
        break;
      default:
        throw std::logic_error(
          "ribi::cmap::OnNodeKeyDownPressed: invalid n_rated"
        );
    }
  }
}

void ribi::cmap::OnNodeKeyDownPressedRateF2(
  QtConceptMap& q, QtNode& item)
{
  //Rate examples
  if (!HasExamples(item)) return;
  ribi::cmap::QtRateExamplesDialog d(item.GetNode().GetConcept());
  q.setEnabled(false);
  d.exec();
  q.setEnabled(true);
  //Find the original Node
  const auto vd = ::find_first_custom_vertex_with_my_vertex(item.GetNode(), q.GetConceptMap());
  //Update the node here
  auto node = item.GetNode();
  node.GetConcept().SetExamples(d.GetRatedExamples());
  //Update the node in the concept map
  set_my_custom_vertex(node, vd, q.GetConceptMap());
  //Update the QtNode
  item.GetNode().GetConcept().SetExamples(d.GetRatedExamples());
}

void ribi::cmap::ProcessKey(QtConceptMap& q, QKeyEvent * const event) //!OCLINT Although the NCSS is high, the code is easy to read
{
  CheckInvariants(q);

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
    case Qt::Key_F8: MoveQtEdgesAndQtNodesRandomly(q); break;
    case Qt::Key_F9: std::exit(1); break; //Cause a deliberate hard crash
    #endif
    case Qt::Key_H: keyPressEventH(q, event); break;
    case Qt::Key_Minus: q.scale(0.9,0.9); break;
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

  m_undo.redo();

  CheckInvariants(*this);
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
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(*this);
  AddEdgesToScene(*this);
  CheckInvariantQtNodesAndNodesHaveSameCoordinats(*this);
  CheckInvariantQtEdgesAndEdgesHaveSameCoordinats(*this);
  CheckInvariants(*this);

  //The new concept map must be displayed in full
  fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
  //this->fitInView(this->sceneRect().marginsAdded(QMarginsF(40, 40, 40, 40)));
  //ensureVisible(scene()->sceneRect(), 50, 100);
  //fitInView(scene()->sceneRect(), Qt::KeepAspectRatioByExpanding);
  CheckInvariants(*this);
}

///Class T may be either a QtNode or a QtEdge
template <class T>
void SelectImpl(
  ribi::cmap::QtConceptMap& q,
  T& t
)
{
  static_assert(
    std::is_same<T, ribi::cmap::QtNode>() || std::is_same<T, ribi::cmap::QtEdge>(),
    "T is either QtEdge or QtNode");

  if (HasExamples(t))
  {
    SetQtExamplesBuddy(q, &t);
  }
  else
  {
    T * const no_qtedge_nor_qtnode{nullptr};
    SetQtExamplesBuddy(q, no_qtedge_nor_qtnode);
  }
  SetSelectedness(true, t, q);
}

void ribi::cmap::Select(QtConceptMap& q, QtEdge& qtedge)
{
  SelectImpl(q, qtedge);
  //Edges do not get a tool item
  QtNode * const no_qtnode{nullptr};
  SetQtToolItemBuddy(q, no_qtnode);
}

void ribi::cmap::Select(QtConceptMap& q, QtNode& qtnode)
{
  SelectImpl(q, qtnode);
  SetQtToolItemBuddy(q, &qtnode);
}

void ribi::cmap::SetFocus(QtConceptMap& q, QtNode* const new_focus_item)
{
  CheckInvariants(q);

  assert(new_focus_item);
  assert(!new_focus_item->isSelected());

  if (QtEdge * const qtedge = FindQtEdge(new_focus_item, q.GetScene()))
  {
    SetSelectedness(true, *qtedge, q);
  }
  else
  {
    SetSelectedness(true, *new_focus_item, q);
  }

  if (HasExamples(*new_focus_item))
  {
    SetQtExamplesBuddy(q, new_focus_item);
  }
  //Only QtNodes (not on QtEdge) have a QtToolItem
  if (IsQtNodeNotOnEdge(new_focus_item, q))
  {
    SetQtToolItemBuddy(q, new_focus_item);
  }
  else
  {
    QtNode * const no_qtnode{nullptr};
    SetQtToolItemBuddy(q, no_qtnode);
  }
  CheckInvariantQtToolItemIsNotAssociatedWithQtEdge(q);

  qDebug() << __func__ << ": set focus";
  new_focus_item->setFocus(); //Do after SetQt(Tool&Example)Buddies
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
  CheckInvariants(*this);

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

  CheckInvariants(*this);
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
  QtNode * const new_focus_item = dynamic_cast<QtNode*>(items[i]);
  assert(new_focus_item);
  assert(!new_focus_item->isSelected());
  SetFocus(q, new_focus_item);

  CheckInvariants(q);
}

void ribi::cmap::SetQtExamplesBuddy(QtConceptMap& q, QtEdge * const qtedge)
{
  if (qtedge)
  {
    SetQtExamplesBuddy(q, qtedge->GetQtNode());
  }
  else
  {
    QtNode * const no_qtnode{nullptr};
    SetQtExamplesBuddy(q, no_qtnode);
  }

  Ensures(qtedge == nullptr || GetQtExamplesItemBuddy(q) == qtedge->GetQtNode()); //!OCLINT no double negation for the reader
}

void ribi::cmap::SetQtExamplesBuddy(QtConceptMap& q, QtNode * const qtnode)
{
  if (qtnode)
  {
    assert(HasExamples(*qtnode));
  }
  q.GetQtExamplesItem().SetBuddyItem(qtnode);

  Ensures(GetQtExamplesItemBuddy(q) == qtnode); //!OCLINT no double negation for the reader
}


void ribi::cmap::SetQtToolItemBuddy(QtConceptMap& q, QtNode * const qtnode)
{
  q.GetQtToolItem().SetBuddyItem(qtnode);
}

void ribi::cmap::SetQtToolItemBuddy(QtConceptMap& q, QtEdge * const qtedge)
{
  SetQtToolItemBuddy(q, qtedge->GetQtNode());
}

void ribi::cmap::SetSelectedness(
  const bool is_selected,
  QtEdge& qtedge,
  QtConceptMap& q
)
{
  //First select Node
  const auto ed = find_first_custom_edge_with_my_edge(
    qtedge.GetEdge(),
    q.GetConceptMap()
  );
  set_edge_selectedness(is_selected, ed, q.GetConceptMap());
  qtedge.SetSelected(is_selected);

  assert(IsSelected(qtedge) == is_selected);

  assert(!is_selected || CountSelectedQtEdges(q) > 0 || IsConnectedToCenterNode(qtedge));
}

void ribi::cmap::SetSelectedness(const bool is_selected,
  QtNode& qtnode,
  QtConceptMap& q
)
{
  //Otherwise find_first_custom_vertex_with_my_vertex will fail
  assert(!IsQtNodeOnEdge(&qtnode, q.GetScene()));

  //First unselect Node ...
  assert(has_custom_vertex_with_my_vertex(qtnode.GetNode(), q.GetConceptMap()));
  const auto vd = find_first_custom_vertex_with_my_vertex(
    qtnode.GetNode(),
    q.GetConceptMap()
  );
  set_vertex_selectedness(is_selected, vd, q.GetConceptMap());

  // ... then unselect QtNode (as onSelectionChanged will be triggered)
  qtnode.setSelected(is_selected);
}

void ribi::cmap::QtConceptMap::showEvent(QShowEvent *)
{
  CheckInvariants(*this);

  m_timer->start(10);
  // this->fitInView(this->sceneRect()); //Don't!
  // Caused https://github.com/richelbilderbeek/Brainweaver/issues/125
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
  SetQtExamplesBuddy(q, no_qtnode);
  SetQtToolItemBuddy(q, no_qtnode);
  SetSelectedness(false, t, q);
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
/*
void ribi::cmap::UnselectAll(QtConceptMap& q)
{
  CheckInvariants(q);

  UnselectAllQtNodes(q);
  UnselectAllQtEdges(q);
  q.GetQtExamplesItem().SetBuddyItem(nullptr);
  q.GetQtToolItem().SetBuddyItem(nullptr);

  CheckInvariants(q);
}

void ribi::cmap::UnselectAllQtEdges(QtConceptMap& q)
{
  for (QtEdge * const qtedge: GetQtEdges(q))
  {
    SetSelectedness(false, *qtedge, q);
  }
}

void ribi::cmap::UnselectAllQtNodes(QtConceptMap& q)
{
  for (QtNode * const qtnode: GetQtNodes(q))
  {
    SetSelectedness(false, *qtnode, q);
  }
}
*/

void ribi::cmap::QtConceptMap::wheelEvent(QWheelEvent *event)
{
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
