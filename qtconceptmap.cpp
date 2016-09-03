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
    m_tools{new QtTool}
{
  #ifndef NDEBUG
  this->SetVerbosity(false);
  #endif
  this->setScene(new QGraphicsScene(this));
  assert(!m_highlighter->GetItem());

  //Add QtNewArrow
  assert(!m_arrow->scene());
  GetScene().addItem(m_arrow); //Add the QtNewArrow so it has a parent
  assert(m_arrow->scene());

  m_arrow->hide();
  assert(!m_arrow->isVisible());

  //Add QtExamplesItem
  assert(!m_examples_item->scene());
  GetScene().addItem(m_examples_item); //Add the examples so it has a parent
  assert(m_examples_item->scene());

  //Add QtTool
  assert(!m_tools->scene());
  GetScene().addItem(m_tools);
  assert(m_tools->scene());

  //Responds when selectionChanged is triggered


  assert(Collect<QtNode>(GetScene()).empty());

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

  CheckInvariants();

  {
    QTimer * const timer{new QTimer(this)};
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onCheckCollision()));
    timer->start(10);
  }
}

ribi::cmap::QtConceptMap::~QtConceptMap()
{
  delete m_examples_item;
  delete m_tools;
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items
  delete m_arrow;
}

void ribi::cmap::QtConceptMap::CheckInvariantAllQtEdgesHaveAscene() const noexcept
{
  //All QtEdges, their QtNodes and Arrows must have a scene
  const auto qtedges = GetQtEdges(GetScene());
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

void ribi::cmap::QtConceptMap::CheckInvariantAllQtNodesHaveAscene() const noexcept
{
  const auto qtnodes = GetQtNodes(GetScene());
  for (const auto qtnode: qtnodes)
  {
    assert(qtnode);
    assert(qtnode->scene());
  }
}

void ribi::cmap::QtConceptMap::CheckInvariants() const noexcept
{
  #ifndef NDEBUG
  CheckInvariantAllQtNodesHaveAscene();
  CheckInvariantAllQtEdgesHaveAscene();

  //If there is one QtEdge selected, its Edge must be able to been found
  try
  {
    const auto qtedge = ExtractTheOneSelectedQtEdge(GetScene());
    //The QtEdge its edge must be in the concept map
    //Can only compare IDs, as QtEdge::GetEdge() and its equivalent in the concept map may mismatch
    assert(
      has_custom_edge_with_my_edge(
        qtedge->GetEdge(), GetConceptMap(), [](const Edge& lhs, const Edge& rhs) { return lhs.GetId() == rhs.GetId(); }
      )
    );
    const auto edge = ExtractTheOneSelectedEdge(this->GetConceptMap(), GetScene());
    assert(qtedge->GetEdge().GetId() == edge.GetId());
  }
  catch (...) {} //No problem


  //If a QtNode with a vignette is selected, the QtExamplesItem must have that
  //QtNode as its buddy
  //For Issue #96, https://github.com/richelbilderbeek/Brainweaver/issues/96
  {
    #ifndef NDEBUG
    {
      const int n1{CountSelectedQtNodes(GetScene())};
      const int n2{static_cast<int>(ribi::cmap::GetSelectedQtNodes(GetScene()).size())};
      if (n1 != n2)
      {
        qCritical() << n1 << " versus " << n2;
      }
      assert(n1 == n2);
    }
    #endif
    assert(CountSelectedQtNodes(GetScene())
      == static_cast<int>(ribi::cmap::GetSelectedQtNodes(GetScene()).size())
    );
    assert(CountSelectedQtEdges(GetScene())
      == static_cast<int>(ribi::cmap::GetSelectedQtEdges(GetScene()).size())
    );
    const auto qtnodes = ribi::cmap::GetSelectedQtNodes(GetScene());
    if (qtnodes.size() == 1)
    {
      //QtNode must have an example
      const ribi::cmap::QtNode * const qtnode = qtnodes[0];
      if (!qtnode->GetNode().GetConcept().GetExamples().Get().empty())
      {
        //QtExamplesItem must have that QtNode as its buddy
        assert(GetQtExamplesItem()->GetBuddyItem() == qtnode);
        assert(GetQtExamplesItem()->isVisible());
        //Must be close
        assert(std::abs(GetQtExamplesItem()->x() - qtnode->GetCenterX()) < 1.0);
        assert(std::abs(qtnode->GetCenterY() - (qtnode->GetOuterHeight() / 2.0) - 16.0 - GetQtExamplesItem()->y()) < 1.0);
      }
    }
  }

  #endif
}

void ribi::cmap::QtConceptMap::HideExamplesItem() noexcept
{
  m_examples_item->hide();
}

void ribi::cmap::QtConceptMap::RemoveConceptMap()
{
  assert(m_arrow);
  m_arrow->hide();  
  assert(m_examples_item);
  m_examples_item->hide();
  if (m_highlighter) m_highlighter->SetItem(nullptr); //Do this before destroying items
  assert(m_tools);
  m_tools->SetBuddyItem(nullptr);
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
  CheckInvariants();

  m_undo.push(command);

  CheckInvariants();

  this->UpdateConceptMap();

  CheckInvariants();

  qApp->processEvents();

  CheckInvariants();
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
  CheckInvariants();
  UpdateConceptMap();
  CheckInvariants();

  //Pass event
  switch (event->key())
  {
    case Qt::Key_F1: keyPressEventF1(); break;
    case Qt::Key_F2: keyPressEventF2(); break;
    case Qt::Key_F4: keyPressEventF4(event); break;
    case Qt::Key_Delete: keyPressEventDelete(event); break;
    #ifndef NDEBUG
    case Qt::Key_F8: MessUp(GetScene()); break;
    case Qt::Key_F9: std::exit(1); //Cause a deliberate hard crash
    #endif
    case Qt::Key_Escape: keyPressEventEscape(event); break;
    case Qt::Key_Equal: this->scale(1.1,1.1); break;
    case Qt::Key_Minus: this->scale(0.9,0.9); break;
    case Qt::Key_E: keyPressEventE(event); break;
    case Qt::Key_H: keyPressEventH(event); break;
    case Qt::Key_N: keyPressEventN(event); break;
    case Qt::Key_T: keyPressEventT(event); break;
    case Qt::Key_Z: keyPressEventZ(event); break;
    case Qt::Key_Question: keyPressEventQuestion(event); break;
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

  UpdateConceptMap();
  CheckInvariants();
}

void ribi::cmap::QtConceptMap::keyPressEventDelete(QKeyEvent *event) noexcept
{
  if (GetVerbosity()) { TRACE("Pressing delete"); }
  try
  {
    DoCommand(new CommandDeleteSelected(m_conceptmap, GetScene(), m_tools));
  }
  catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
  event->setAccepted(true);
}

void ribi::cmap::QtConceptMap::keyPressEventE(QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (GetVerbosity()) { TRACE("Pressing CTRL-E"); }
    try { this->DoCommand(new CommandCreateNewEdgeBetweenTwoSelectedNodes(GetConceptMap(),m_mode,GetScene(),m_tools)); }
    catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
    event->setAccepted(true);
  }
}

void ribi::cmap::QtConceptMap::keyPressEventEscape(QKeyEvent *event) noexcept
{
  if (GetVerbosity()) { TRACE("Pressing Escape"); }
  //Only remove the 'new arrow' if present
  if (m_arrow->isVisible())
  {
    if (GetVerbosity()) { TRACE("Remove the new arrow"); }
    m_arrow->hide();
    assert(!m_arrow->isVisible());
    return;
  }
  else
  {
    event->setAccepted(false); //Signal to dialog to close
  }
}

void ribi::cmap::QtConceptMap::keyPressEventF1() noexcept
{
  try
  {
    const auto items = GetScene().selectedItems();
    if (items.size() != 1) return;
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(items.front()))
    {
      OnNodeKeyDownPressed(qtnode, Qt::Key_F1);
    }
  }
  catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
}

void ribi::cmap::QtConceptMap::keyPressEventF2() noexcept
{
  try
  {
    const auto items = GetScene().selectedItems();
    if (items.size() != 1) return;
    if (QtNode * const qtnode = dynamic_cast<QtNode*>(items.front()))
    {
      OnNodeKeyDownPressed(qtnode, Qt::Key_F2);
    }
  }
  catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
}

void ribi::cmap::QtConceptMap::keyPressEventF4(QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::AltModifier)
  {
    if (GetVerbosity()) { TRACE("Pressing Alt-F4"); }
    event->setAccepted(false); //Signal to dialog to close
  }
}


void ribi::cmap::QtConceptMap::keyPressEventH(QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (GetVerbosity()) { TRACE("Pressing CTRL-H"); }
    try
    {
      const auto cmd = new CommandToggleArrowHead(GetConceptMap(), GetScene());
      this->DoCommand(cmd);
    }
    catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
    event->setAccepted(true);
  }
}

void ribi::cmap::QtConceptMap::keyPressEventN(QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (GetVerbosity()) { TRACE("Pressing CTRL-N"); }
    try { this->DoCommand(new CommandCreateNewNode(m_conceptmap,m_mode,GetScene(),m_tools,0.0,0.0)); }
    catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
  }
}

void ribi::cmap::QtConceptMap::keyPressEventQuestion(QKeyEvent *) noexcept
{
  if (GetVerbosity()) { TRACE("Pressing Qt::Key_Question"); }
  UpdateConceptMap();
}

void ribi::cmap::QtConceptMap::keyPressEventT(QKeyEvent *event) noexcept
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    if (GetVerbosity()) { TRACE("Pressing CTRL-T"); }
    try
    {
      const auto cmd = new CommandToggleArrowTail(GetConceptMap(), GetScene());
      this->DoCommand(cmd);
    }
    catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
    event->setAccepted(true);
  }
}

void ribi::cmap::QtConceptMap::keyPressEventZ(QKeyEvent *event) noexcept
{
  try
  {
    if (event->modifiers() & Qt::ControlModifier)
    {
      if (event->modifiers() & Qt::ShiftModifier)
      {
        this->m_undo.redo();
      }
      else
      {
        this->m_undo.undo();
      }
    }
  }
  catch (std::exception& e) { if (GetVerbosity()) { TRACE(e.what()); } }
}

void ribi::cmap::QtConceptMap::mouseDoubleClickEvent(QMouseEvent *event)
{
  CheckInvariants();

  const QPointF pos = mapToScene(event->pos());

  //Do not create new nodes when double-clicking on an existing one
  if (GetScene().itemAt(pos, QTransform())) return;

  //Create new node at the mouse cursor its position
  try {
    this->DoCommand(
      new CommandCreateNewNode(
        m_conceptmap,
        m_mode,
        GetScene(),
        m_tools,
        pos.x(),
        pos.y()
      )
    );
  }
  catch (std::logic_error& ) {}
  UpdateExamplesItem();

  CheckInvariants();
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
  UpdateConceptMap();
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
          m_tools
        );
        DoCommand(command);
        UpdateConceptMap();
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
  UpdateExamplesItem();
  assert(!m_arrow->isSelected());
}

void ribi::cmap::QtConceptMap::onCheckCollision()
{
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
}

void ribi::cmap::QtConceptMap::onFocusItemChanged(
  QGraphicsItem * newFocus, QGraphicsItem */*oldFocus*/, Qt::FocusReason reason
)
{
  //Focus on QtNode
  if (QtNode * const qtnode = dynamic_cast<QtNode*>(newFocus)) {
    if (!IsOnEdge(qtnode, GetScene()))
    {
      m_tools->SetBuddyItem(qtnode);
      onSelectionChanged();
      m_arrow->hide();
    }
    else
    {
      m_tools->SetBuddyItem(nullptr);
      onSelectionChanged();
    }
    return;
  }
  //Focus on m_tools
  if (newFocus == m_tools && !m_arrow->isVisible() && m_tools->GetBuddyItem() && reason == Qt::MouseFocusReason) {
    m_arrow->Start(m_tools->GetBuddyItem()); //Also sets visibility
    m_tools->setSelected(false);
    m_tools->GetBuddyItem()->setSelected(true); //Will tigger onSelectionChanged and hide the arrow
    m_tools->GetBuddyItem()->setFocus();
    m_arrow->setVisible(true);
  }
}

void ribi::cmap::QtConceptMap::OnNodeKeyDownPressed(QtNode* const item, const int key)
{
  //Note: item can also be the QtNode on a QtEdge
  assert(item);
  if (m_mode == Mode::edit && key == Qt::Key_F2)
  {
    //Edit concept
    QtScopedDisable<QtConceptMap> disable(this);
    QtConceptMapConceptEditDialog d(item->GetNode().GetConcept());
    d.exec();
    //Find the original Node or Edge
    if (::has_custom_vertex_with_my_vertex(item->GetNode(), m_conceptmap))
    {
      assert(::has_custom_vertex_with_my_vertex(item->GetNode(), m_conceptmap));
      const auto vd = ::find_first_custom_vertex_with_my_vertex(item->GetNode(), m_conceptmap);
      //Update the node here
      auto node = item->GetNode();
      node.SetConcept(d.GetConcept());
      //Update the node in the concept map
      set_my_custom_vertex(node, vd, m_conceptmap);
    }
    else
    {
      //It is a node on an edge
      //Find the first (and hopefully only) edge with the node on it
      Node node = item->GetNode();
      const auto ed = ::find_first_custom_edge(
        [node](const Edge& e) { return e.GetNode() == node; },
        m_conceptmap
      );
      //Get hold of the Edge
      Edge edge = ::get_my_custom_edge(ed, m_conceptmap);
      //Update the Edge here
      node.SetConcept(d.GetConcept());
      edge.SetNode(node);
      //Update the node in the concept map
      set_my_custom_edge(edge, ed, m_conceptmap);
    }
    //Update the QtNode
    item->GetNode().SetConcept(d.GetConcept());
    //Set the word-wrapped text
    item->SetText(Wordwrap(d.GetConcept().GetName(), GetWordWrapLength()));

  }
  else if (m_mode == Mode::rate && key == Qt::Key_F1)
  {
    //Rate concept
    QtScopedDisable<QtConceptMap> disable(this);
    const auto vd = ::find_first_custom_vertex_with_my_vertex(item->GetNode(), m_conceptmap);
    const auto subgraph = create_direct_neighbour_custom_and_selectable_edges_and_vertices_subgraph(vd, m_conceptmap);
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
      set_my_custom_vertex(node, vd, m_conceptmap);
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
  else if (m_mode == Mode::rate && key == Qt::Key_F2)
  {
    //Rate examples
    if (item->GetNode().GetConcept().GetExamples().Get().empty()) return;
    QtScopedDisable<QtConceptMap> disable(this);
    ribi::cmap::QtRateExamplesDialog d(item->GetNode().GetConcept());
    d.exec();
    //Find the original Node
    const auto vd = ::find_first_custom_vertex_with_my_vertex(item->GetNode(), m_conceptmap);
    //Update the node here
    auto node = item->GetNode();
    node.GetConcept().SetExamples(d.GetRatedExamples());
    //Update the node in the concept map
    set_my_custom_vertex(node, vd, m_conceptmap);
    //Update the QtNode
    item->GetNode().GetConcept().SetExamples(d.GetRatedExamples());
  }

  this->show();
  this->setFocus();
  GetScene().setFocusItem(item);
  item->setSelected(true);
  GetScene().update();
}



void ribi::cmap::QtConceptMap::onSelectionChanged()
{
  ConceptMap& g = m_conceptmap;

  //Selectness of vertices
  const auto vip = vertices(g);
  std::for_each(vip.first,vip.second,
    [this, &g](const VertexDescriptor vd) {
      const auto vertex_map = get(boost::vertex_custom_type,g);
      const auto is_selected_map = get(boost::vertex_is_selected,g);
      const auto qtnode = FindQtNode(get(vertex_map,vd).GetId(),GetScene());
      //qtnode can be nullptr when onSelectionChanged is called while building up the QtConceptMap
      if (qtnode) { put(is_selected_map,vd,qtnode->isSelected()); }
    }
  );

  //Selectness of edges
  const auto eip = edges(g);
  std::for_each(eip.first,eip.second,
    [this, &g](const EdgeDescriptor ed) {
      const auto edge_map = get(boost::edge_custom_type,g);
      const auto is_selected_map = get(boost::edge_is_selected,g);
      const auto qtedge = FindQtEdge(get(edge_map,ed).GetId(),GetScene());
      if (qtedge) { put(is_selected_map,ed,qtedge->isSelected()); }
    }
  );
  GetScene().update();
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

  CheckInvariants();
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

void ribi::cmap::QtConceptMap::UpdateConceptMap()
{
  UpdateExamplesItem();
  for (const auto item: GetScene().items()) { item->update(); }
  onSelectionChanged();
}

void ribi::cmap::QtConceptMap::UpdateExamplesItem()
{
  //If nothing is selected, hide the Examples
  m_examples_item->SetBuddyItem(nullptr); //Handles visibility
  qDebug() << "CountSelectedQtNodes(GetScene()): " << CountSelectedQtNodes(GetScene());
  if (CountSelectedQtNodes(GetScene()) == 1)
  {
    const auto selected_qtnodes = GetSelectedQtNodes(GetScene());
    assert(selected_qtnodes.size() == 1);
    const auto selected_qtnode = selected_qtnodes[0];
    m_examples_item->SetBuddyItem(selected_qtnode); //Handles visibility
  }
}

void ribi::cmap::QtConceptMap::wheelEvent(QWheelEvent *event)
{
  if (event->delta() > 0)
  {
    this->scale(1.1,1.1);
  }
  else if (event->delta() < 0)
  {
    this->scale(0.9,0.9);
  }

}
