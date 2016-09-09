//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmap.h"

#include <iostream>
#include <set>

#include <boost/bind.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <QApplication>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>

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
#include "qtconceptmapcenternode.h"
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
#include "trace.h"
#include "xml.h"

#pragma GCC diagnostic pop

ribi::cmap::QtConceptMap::QtConceptMap(QWidget* parent)
  : QtKeyboardFriendlyGraphicsView(parent),
    m_arrow{new QtNewArrow},
    m_conceptmap{},
    m_examples_item(new QtExamplesItem),
    m_highlighter{new QtItemHighlighter},
    m_mode{Mode::uninitialized},
    m_timer{new QTimer(this)},
    m_tools{new QtTool}
{
  #ifndef NDEBUG
  this->SetVerbosity(false);
  #endif
  this->setScene(new QGraphicsScene(this));
  assert(!m_highlighter->GetItem());

  //Add items
  GetScene().addItem(m_arrow); //Add the QtNewArrow so it has a parent
  GetScene().addItem(m_examples_item); //Add the examples so it has a parent
  GetScene().addItem(m_tools);
  m_arrow->hide();

  //Without this line, mouseMoveEvent won't be called
  this->setMouseTracking(true);

  {
    QLinearGradient linearGradient(-500,-500,500,500);
    linearGradient.setColorAt(0.0,QColor(214,214,214));
    linearGradient.setColorAt(1.0,QColor(255,255,255));
    GetScene().setBackgroundBrush(linearGradient);
    //this->scene()->setBackgroundBrush(QBrush(QColor(255,255,255)));
  }

  //Connect the scene
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  QObject::connect(scene(),SIGNAL(focusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)),this,SLOT(onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason)));
  #else

  #endif
  QObject::connect(scene(),SIGNAL(selectionChanged()),this,SLOT(onSelectionChanged()));

  m_examples_item->SetCenterPos(123,456); //Irrelevant where

  CheckInvariants(*this);

  {
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(Respond()));
    m_timer->start(10);
  }
}

ribi::cmap::QtConceptMap::~QtConceptMap()
{
  m_timer->stop();
  delete m_examples_item;
  delete m_tools;
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items
  delete m_arrow;
}

void ribi::cmap::CheckInvariantAllQtEdgesHaveAscene(
  const QtConceptMap& q
) noexcept
{
  //All QtEdges, their QtNodes and Arrows must have a scene
  const auto qtedges = GetQtEdges(q.GetScene());
  for (const auto qtedge: qtedges)
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
  const auto qtnodes = GetQtNodes(q.GetScene());
  for (const auto qtnode: qtnodes)
  {
    assert(qtnode);
    assert(qtnode->scene());
  }
}

void ribi::cmap::CheckInvariantOneQtNodeWithExamplesHasExamplesItem(
  const QtConceptMap& q
) noexcept
{
  #ifndef NDEBUG
  assert(CountSelectedQtNodes(q.GetScene())
    == static_cast<int>(ribi::cmap::GetSelectedQtNodes(q.GetScene()).size())
  );
  assert(CountSelectedQtEdges(q.GetScene())
    == static_cast<int>(ribi::cmap::GetSelectedQtEdges(q.GetScene()).size())
  );
  const auto qtnodes = ribi::cmap::GetSelectedQtNodesAlsoOnQtEdge(q.GetScene());
  if (qtnodes.size() == 1)
  {
    //QtNode must have an example
    const ribi::cmap::QtNode * const qtnode = qtnodes[0];
    if (!qtnode->GetNode().GetConcept().GetExamples().Get().empty())
    {
      //QtExamplesItem must have that QtNode as its buddy
      assert(q.GetQtExamplesItem().GetBuddyItem() == qtnode);
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

void ribi::cmap::CheckInvariants(const QtConceptMap& q) noexcept
{
  #ifndef NDEBUG

  assert(q.GetQtNewArrow().scene());
  assert(q.GetQtExamplesItem().scene());
  assert(q.GetQtToolItem().scene());

  CheckInvariantAllQtNodesHaveAscene(q);
  CheckInvariantAllQtEdgesHaveAscene(q);

  //If there is one QtEdge selected, its Edge must be able to been found
  try
  {
    const auto qtedge = ExtractTheOneSelectedQtEdge(q.GetScene());
    //The QtEdge its edge must be in the concept map
    //Can only compare IDs, as QtEdge::GetEdge() and its equivalent in the concept map may mismatch
    assert(
      has_custom_edge_with_my_edge(
        qtedge->GetEdge(), q.GetConceptMap(), [](const Edge& lhs, const Edge& rhs) { return lhs.GetId() == rhs.GetId(); }
      )
    );
    const auto edge = ExtractTheOneSelectedEdge(q.GetConceptMap(), q.GetScene());
    assert(qtedge->GetEdge().GetId() == edge.GetId());
  }
  catch (...) {} //No problem


  //If a QtNode with a vignette is selected, the QtExamplesItem must have that
  //QtNode as its buddy
  //For Issue #96, https://github.com/richelbilderbeek/Brainweaver/issues/96
  CheckInvariantOneQtNodeWithExamplesHasExamplesItem(q);
  #endif
}

void ribi::cmap::HideExamplesItem(QtConceptMap& q) noexcept
{
  q.GetQtExamplesItem().hide();
}

void ribi::cmap::QtConceptMap::RemoveConceptMap()
{
  assert(m_arrow);
  m_arrow->hide();  
  assert(m_examples_item);
  m_examples_item->hide();
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items
  GetQtToolItem().SetBuddyItem(nullptr);
  assert(!m_arrow->isVisible());

  for (auto qtedge: Collect<QtEdge>(GetScene()))
  {
    GetScene().removeItem(qtedge);
    delete qtedge;
  }

  for (auto qtnode: Collect<QtNode>(GetScene()))
  {
    GetScene().removeItem(qtnode);
    delete qtnode;
  }
}



void ribi::cmap::QtConceptMap::DoCommand(Command * const command) noexcept
{
  CheckInvariants(*this);

  m_undo.push(command);

  CheckInvariants(*this);

  UpdateConceptMap(*this);

  CheckInvariants(*this);

  qApp->processEvents();

  CheckInvariants(*this);
}




ribi::cmap::QtNode* ribi::cmap::QtConceptMap::GetItemBelowCursor(const QPointF& pos) const
{
  #if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  const QList<QGraphicsItem*> v = GetScene().items(pos.x(),pos.y(),2.0,2.0,Qt::IntersectsItemShape,Qt::AscendingOrder);
  #else
  const QList<QGraphicsItem*> v = GetScene().items(pos.x(),pos.y(),2.0,2.0);
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

///The arrow that must be clicked to add a new edge
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

QGraphicsScene& ribi::cmap::QtConceptMap::GetScene() noexcept
{
  assert(scene());
  return *scene();
}

const QGraphicsScene& ribi::cmap::QtConceptMap::GetScene() const noexcept
{
  assert(scene());
  return *scene();
}

void ribi::cmap::QtConceptMap::keyPressEvent(QKeyEvent *event)
{
  event->setAccepted(false);
  CheckInvariants(*this);
  UpdateConceptMap(*this);
  CheckInvariants(*this);

  //Pass event
  switch (event->key())
  {
    case Qt::Key_F1: keyPressEventF1(*this); break;
    case Qt::Key_F2: keyPressEventF2(*this); break;
    case Qt::Key_F4: keyPressEventF4(*this, event); break;
    case Qt::Key_Delete: keyPressEventDelete(*this, event); break;
    #ifndef NDEBUG
    case Qt::Key_F8: MessUp(GetScene()); break;
    case Qt::Key_F9: std::exit(1); //Cause a deliberate hard crash
    #endif
    case Qt::Key_Escape: keyPressEventEscape(*this, event); break;
    case Qt::Key_Equal: this->scale(1.1,1.1); break;
    case Qt::Key_Minus: this->scale(0.9,0.9); break;
    case Qt::Key_E: keyPressEventE(*this, event); break;
    case Qt::Key_H: keyPressEventH(*this, event); break;
    case Qt::Key_N: keyPressEventN(*this, event); break;
    case Qt::Key_T: keyPressEventT(*this, event); break;
    case Qt::Key_Z: keyPressEventZ(*this, event); break;
    case Qt::Key_Question: keyPressEventQuestion(*this, event); break;
  }

  //Pass event to QtEdges
  for (auto qtedge: GetSelectedQtEdges(GetScene())) {
    if (!event->isAccepted())
    {
      qtedge->keyPressEvent(event);
    }
    qtedge->update();
  }

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
  if (q.GetVerbosity()) { TRACE("Pressing delete"); }
  try
  {
    q.DoCommand(
      new CommandDeleteSelected(q.GetConceptMap(), q.GetScene(), q.GetQtToolItem())
    );
  }
  catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
  event->setAccepted(true);
}

void ribi::cmap::keyPressEventE(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (q.GetVerbosity()) { TRACE("Pressing CTRL-E"); }
    try
    {
      q.DoCommand(
        new CommandCreateNewEdgeBetweenTwoSelectedNodes(
          q.GetConceptMap(), q.GetMode(), q.GetScene(), q.GetQtToolItem()
        )
      );
    }
    catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
    event->setAccepted(true);
  }
}

void ribi::cmap::keyPressEventEscape(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (q.GetVerbosity()) { TRACE("Pressing Escape"); }
  //Only remove the 'new arrow' if present
  if (q.GetQtNewArrow().isVisible())
  {
    if (q.GetVerbosity()) { TRACE("Remove the new arrow"); }
    q.GetQtNewArrow().hide();

    assert(!q.GetQtNewArrow().isVisible());
    return;
  }
  else
  {
    event->setAccepted(false); //Signal to dialog to close
  }
}

void ribi::cmap::keyPressEventF1(QtConceptMap& q) noexcept
{
  try
  {
    const auto items = q.GetScene().selectedItems();
    if (items.size() != 1) return;
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(items.front()))
    {
      OnNodeKeyDownPressed(q, qtnode, Qt::Key_F1);
    }
  }
  catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
}

void ribi::cmap::keyPressEventF2(QtConceptMap& q) noexcept
{
  try
  {
    const auto items = q.GetScene().selectedItems();
    if (items.size() != 1) return;
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(items.front()))
    {
      OnNodeKeyDownPressed(q, qtnode, Qt::Key_F2);
    }
  }
  catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
}

void ribi::cmap::keyPressEventF4(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::AltModifier)
  {
    if (q.GetVerbosity()) { TRACE("Pressing Alt-F4"); }
    event->setAccepted(false); //Signal to dialog to close
  }
}


void ribi::cmap::keyPressEventH(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (q.GetVerbosity()) { TRACE("Pressing CTRL-H"); }
    try
    {
      const auto cmd = new CommandToggleArrowHead(q.GetConceptMap(), q.GetScene());
      q.DoCommand(cmd);
    }
    catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
    event->setAccepted(true);
  }
}

void ribi::cmap::keyPressEventN(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (q.GetVerbosity()) { TRACE("Pressing CTRL-N"); }
    try
    {
      q.DoCommand(
        new CommandCreateNewNode(
          q.GetConceptMap(),
          q.GetMode(),
          q.GetScene(),
          q.GetQtToolItem(),
          0.0,
          0.0
        )
      );
    }
    catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
  }
}

void ribi::cmap::keyPressEventQuestion(QtConceptMap& q, QKeyEvent *) noexcept
{
  if (q.GetVerbosity()) { TRACE("Pressing Qt::Key_Question"); }
  UpdateConceptMap(q);
}

void ribi::cmap::keyPressEventT(QtConceptMap& q, QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (q.GetVerbosity()) { TRACE("Pressing CTRL-T"); }
    try
    {
      const auto cmd = new CommandToggleArrowTail(q.GetConceptMap(), q.GetScene());
      q.DoCommand(cmd);
    }
    catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
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
  catch (std::exception& e) { if (q.GetVerbosity()) { TRACE(e.what()); } }
}

void ribi::cmap::QtConceptMap::mouseDoubleClickEvent(QMouseEvent *event)
{
  CheckInvariants(*this);

  const QPointF pos = mapToScene(event->pos());

  //Do not create new nodes when double-clicking on an existing one
  if (GetScene().itemAt(pos, QTransform())) return;

  //Create new node at the mouse cursor its position
  try
  {
    this->DoCommand(
      new CommandCreateNewNode(
        m_conceptmap,
        m_mode,
        GetScene(),
        GetQtToolItem(),
        pos.x(),
        pos.y()
      )
    );
  }
  catch (std::logic_error& ) {}
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
    QtNode* const item_below = GetItemBelowCursor(mapToScene(event->pos()));

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
  if (GetVerbosity()) { TRACE_FUNC(); }
  UpdateConceptMap(*this);
  assert(m_highlighter);
  assert(!m_arrow->isSelected());
  if (m_arrow->isVisible())
  {
    assert(!m_arrow->isSelected());
    if (m_highlighter->GetItem() && m_arrow->GetFrom() != m_highlighter->GetItem())
    {
      //The command needs to find the two selected vertices
      for (auto& i: GetScene().selectedItems()) { i->setSelected(false); }
      m_highlighter->GetItem()->setSelected(true);
      m_arrow->GetFrom()->setSelected(true);
      try
      {
        const auto command = new CommandCreateNewEdgeBetweenTwoSelectedNodes(
          m_conceptmap,
          m_mode,
          GetScene(),
          GetQtToolItem()
        );
        DoCommand(command);
        UpdateConceptMap(*this);
        m_arrow->hide();
        assert(m_highlighter);
        m_highlighter->SetItem(nullptr);
      }
      catch (std::logic_error&) { return; }
    }
    assert(!m_arrow->isSelected());
  }

  assert(!m_arrow->isSelected());
  QtKeyboardFriendlyGraphicsView::mousePressEvent(event);
  assert(!m_arrow->isSelected());
  UpdateExamplesItem(*this);
  assert(!m_arrow->isSelected());
}

void ribi::cmap::QtConceptMap::Respond()
{
  CheckInvariants(*this);
  for (const auto item: GetScene().items())
  {
    if (!(item->flags() & QGraphicsItem::ItemIsMovable)) continue;
    QtNode* const node = dynamic_cast<QtNode*>(item);
    if (!node) continue;
    const auto others = item->collidingItems();
    for (const auto other: others)
    {
      if (!(other->flags() & QGraphicsItem::ItemIsMovable)) continue;
      const QtNode* const other_node = dynamic_cast<const QtNode*>(other);
      if (!other_node) continue;
      const double dx = node->x() - other_node->x() > 0.0 ? 1.0 : -1.0;
      const double dy = node->y() - other_node->y() > 0.0 ? 1.0 : -1.0;
      node->SetCenterPos(node->x()  + dx, node->y()  + dy);
    }
  }
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
    GetQtToolItem().GetBuddyItem()->setSelected(true); //Will tigger onSelectionChanged and hide the arrow
    GetQtToolItem().GetBuddyItem()->setFocus();
    GetQtNewArrow().setVisible(true);
  }
  else
  {
    UpdateQtToolItem(*this);
    GetQtNewArrow().setVisible(false);

  }
}

void ribi::cmap::OnNodeKeyDownPressed(QtConceptMap& q, QtNode* const item, const int key)
{
  //Note: item can also be the QtNode on a QtEdge
  assert(item);
  if (q.GetMode() == Mode::edit && key == Qt::Key_F2)
  {
    //Edit concept
    QtScopedDisable<QtConceptMap> disable(&q);
    QtConceptMapConceptEditDialog d(item->GetNode().GetConcept());
    d.exec();
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
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F1)
  {
    //Rate concept
    QtScopedDisable<QtConceptMap> disable(&q);
    const auto vd = ::find_first_custom_vertex_with_my_vertex(item->GetNode(), q.GetConceptMap());
    const auto subgraph = create_direct_neighbour_custom_and_selectable_edges_and_vertices_subgraph(vd, q.GetConceptMap());
    ribi::cmap::QtRateConceptDialog d(subgraph);
    d.exec();
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
        = (node.GetConcept().GetRatingComplexity()   != -1 ? 1 : 0)
        + (node.GetConcept().GetRatingConcreteness() != -1 ? 1 : 0)
        + (node.GetConcept().GetRatingSpecificity()  != -1 ? 1 : 0);
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
        default: assert(!"Should not get here");
      }
    }
  }
  else if (q.GetMode() == Mode::rate && key == Qt::Key_F2)
  {
    //Rate examples
    if (item->GetNode().GetConcept().GetExamples().Get().empty()) return;
    QtScopedDisable<QtConceptMap> disable(&q);
    ribi::cmap::QtRateExamplesDialog d(item->GetNode().GetConcept());
    d.exec();
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

  q.show();
  q.setFocus();
  q.GetScene().setFocusItem(item);
  item->setSelected(true);
  q.GetScene().update();
}



void ribi::cmap::onSelectionChanged(QtConceptMap& q)
{
  ConceptMap& g = q.GetConceptMap();

  //Selectness of vertices
  const auto vip = vertices(g);
  std::for_each(vip.first,vip.second,
    [&q, &g](const VertexDescriptor vd) {
      const auto vertex_map = get(boost::vertex_custom_type,g);
      const auto is_selected_map = get(boost::vertex_is_selected,g);
      const auto qtnode = FindQtNode(get(vertex_map,vd).GetId(), q.GetScene());
      //qtnode can be nullptr when onSelectionChanged is called while building up the QtConceptMap
      if (qtnode) { put(is_selected_map,vd,qtnode->isSelected()); }
    }
  );

  //Selectness of edges
  const auto eip = edges(g);
  std::for_each(eip.first,eip.second,
    [&q, &g](const EdgeDescriptor ed) {
      const auto edge_map = get(boost::edge_custom_type,g);
      const auto is_selected_map = get(boost::edge_is_selected,g);
      const auto qtedge = FindQtEdge(get(edge_map,ed).GetId(), q.GetScene());
      if (qtedge) { put(is_selected_map,ed,qtedge->isSelected()); }
    }
  );
  q.GetScene().update();
}


void ribi::cmap::QtConceptMap::Redo() noexcept
{
  m_undo.redo();
}

void ribi::cmap::QtConceptMap::SetConceptMap(const ConceptMap& conceptmap)
{
  RemoveConceptMap();
  m_conceptmap = conceptmap;
  assert(GetConceptMap() == conceptmap);

  //This std::vector keeps the QtNodes in the same order as the nodes in the concept map
  //You cannot rely on Collect<QtConceptMapNodeConcept*>(scene), as this shuffles the order
  //std::vector<QtNode*> qtnodes;

  assert(Collect<QtNode>(GetScene()).empty());

  //Add the nodes to the scene, if there are any
  const auto vip = vertices(m_conceptmap);
  for(auto i=vip.first; i!=vip.second; ++i)
  {
    assert(boost::num_vertices(m_conceptmap));
    const auto pmap = get(boost::vertex_custom_type, m_conceptmap);
    const Node node = get(pmap, *i);
    const bool is_focal_node{i == vip.first};
    QtNode * const qtnode{new QtNode(node)};
    if (is_focal_node)
    {
      qtnode->setFlags(
          QGraphicsItem::ItemIsFocusable
        // | QGraphicsItem::ItemIsMovable
        | QGraphicsItem::ItemIsSelectable
      );
    }
    assert(qtnode);
    assert(!qtnode->scene());
    GetScene().addItem(qtnode);
    assert(qtnode->scene());
    assert(FindQtNode(node.GetId(), GetScene()));
  }
  //Add the Edges
  const auto eip = edges(m_conceptmap);
  for(auto i = eip.first; i != eip.second; ++i)
  {
    assert(boost::num_edges(m_conceptmap));
    const VertexDescriptor vd_from = boost::source(*i, m_conceptmap);
    const VertexDescriptor vd_to = boost::target(*i, m_conceptmap);
    assert(vd_from != vd_to);
    const auto vertex_map = get(boost::vertex_custom_type, m_conceptmap);
    const Node from = get(vertex_map, vd_from);
    const Node to = get(vertex_map, vd_to);
    assert(from.GetId() != to.GetId());
    QtNode * const qtfrom = FindQtNode(from.GetId(), GetScene());
    QtNode * const qtto = FindQtNode(to.GetId(), GetScene());
    assert(qtfrom != qtto);
    const auto edge_map = get(boost::edge_custom_type, m_conceptmap);
    const Edge edge = get(edge_map, *i);
    QtEdge * const qtedge{new QtEdge(edge,qtfrom,qtto)};
    if (qtfrom->GetNode().IsCenterNode() || qtto->GetNode().IsCenterNode())
    {
      qtedge->GetQtNode()->setVisible(false);
    }

    assert(!qtedge->scene());
    assert(!qtedge->GetQtNode()->scene());
    assert(!qtedge->GetArrow()->scene());
    GetScene().addItem(qtedge);
    //scene()->addItem(qtedge->GetQtNode()); //Get these for free when adding a QtEdge
    //scene()->addItem(qtedge->GetArrow()); //Get these for free when adding a QtEdge
    assert(qtedge->scene());
    assert(qtedge->GetQtNode()->scene());
    assert(qtedge->GetArrow()->scene());
  }
  assert(GetConceptMap() == conceptmap);

  CheckInvariants(*this);
}

void ribi::cmap::QtConceptMap::SetMode(const ribi::cmap::Mode mode) noexcept
{
  m_mode = mode;
  auto qtnodes = GetQtNodes(GetScene());
  for (auto qtnode: qtnodes)
  {
    const auto f = GetQtNodeBrushFunction(m_mode);
    qtnode->SetBrushFunction(f);
  }
}
void ribi::cmap::QtConceptMap::Undo() noexcept
{
  assert(m_undo.count() > 0);
  m_undo.undo();
}

void ribi::cmap::UpdateConceptMap(QtConceptMap& q)
{
  for (const auto item: q.GetScene().items()) { item->update(); }
  onSelectionChanged(q);
  UpdateExamplesItem(q);
  q.update();
  q.GetScene().update();
}

void ribi::cmap::UpdateExamplesItem(QtConceptMap& q)
{
  //If nothing is selected, hide the Examples
  q.GetQtExamplesItem().SetBuddyItem(nullptr); //Handles visibility
  const auto selected_qtnodes = GetSelectedQtNodesAlsoOnQtEdge(q.GetScene());
  if (selected_qtnodes.size() == 1)
  {
    const auto selected_qtnode = selected_qtnodes[0];
    q.GetQtExamplesItem().SetBuddyItem(selected_qtnode); //Handles visibility
  }
  q.update();
  q.show();
  q.GetScene().update();
  qApp->processEvents();
  CheckInvariantOneQtNodeWithExamplesHasExamplesItem(q);
}

void ribi::cmap::UpdateQtToolItem(QtConceptMap& q)
{
  const auto selected_qtnodes = GetSelectedQtNodes(q.GetScene());
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
