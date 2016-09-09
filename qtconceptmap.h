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
#ifndef QTCONCEPTMAPCONCEPTMAP_H
#define QTCONCEPTMAPCONCEPTMAP_H

#include <functional>
#include <QUndoStack>
#include "qtkeyboardfriendlygraphicsview.h"
#include "qtconceptmapfwd.h"
#include "qtconceptmapmode.h"
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
class QtConceptMap : public ribi::QtKeyboardFriendlyGraphicsView
{
  Q_OBJECT //!OCLINT

public:

  explicit QtConceptMap(QWidget* parent = 0);
  QtConceptMap(const QtConceptMap&) = delete;
  QtConceptMap& operator=(const QtConceptMap&) = delete;
  ~QtConceptMap() noexcept;

  ///Raw pointer, because ConceptMap its QUndoStack will take over ownership of pointer
  void DoCommand(Command * const command) noexcept;

  ///Obtain the concept map
  const ConceptMap& GetConceptMap() const noexcept { return m_conceptmap; }
        ConceptMap& GetConceptMap()       noexcept { return m_conceptmap; }

  Mode GetMode() const noexcept { return m_mode; }

  ///The square showing the examples
  const QtExamplesItem& GetQtExamplesItem() const noexcept;
        QtExamplesItem& GetQtExamplesItem() noexcept;

  ///The arrow that must be clicked to add a new edge
  const QtNewArrow& GetQtNewArrow() const noexcept;
        QtNewArrow& GetQtNewArrow()       noexcept;

  ///The arrow that must be clicked to add a new edge
  const QtTool& GetQtToolItem() const noexcept;
        QtTool& GetQtToolItem()       noexcept;

  ///Obtain the QGraphicsScene
  const QGraphicsScene& GetScene() const noexcept;
        QGraphicsScene& GetScene() noexcept;

  const QUndoStack& GetUndo() const noexcept { return m_undo; }
        QUndoStack& GetUndo()       noexcept { return m_undo; }


  void Redo() noexcept;

  void SetConceptMap(const ConceptMap& conceptmap);

  void SetMode(const Mode mode) noexcept;

  void Undo() noexcept;

public slots:

  void keyPressEvent(QKeyEvent* event);
  void mouseMoveEvent(QMouseEvent * event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);


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

  ///The item showing the tools
  QtTool * const m_tools;

  QUndoStack m_undo;

  ///The function how a QtEdge determines it is colored
  std::function<QBrush(const QtEdge&)> GetEdgeBrushFunction(const Mode mode);

  ///Obtain the first QtNode under the cursor
  ///Returns nullptr if none is present
  QtNode* GetItemBelowCursor(const QPointF& pos) const;


  ///Remove all Qt and non-Qt items
  void RemoveConceptMap();

  ///Set the rectangle with text showing the examples
  void SetExamplesItem(QtExamplesItem * const item);

private slots:

  ///A timed event to check for collisions, update QtExamplesItem and QtToolItem
  void Respond();

  void onFocusItemChanged(QGraphicsItem*,QGraphicsItem*,Qt::FocusReason);
};

///Checks if the QtConceptMap is in a valid state
void CheckInvariants(const QtConceptMap& q) noexcept;

///All QtEdges must have a QScene
void CheckInvariantAllQtEdgesHaveAscene(const QtConceptMap& q) noexcept;

///All QtNodes must have a QScene
void CheckInvariantAllQtNodesHaveAscene(const QtConceptMap& q) noexcept;

///If one QtNode with examples is selected, the ExamplesItem must be visible and close
void CheckInvariantOneQtNodeWithExamplesHasExamplesItem(const QtConceptMap& q) noexcept;

///Hide the QtExamplesItem
void HideExamplesItem(QtConceptMap& q) noexcept;

void keyPressEventDelete(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventE(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventEscape(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventF1(QtConceptMap& q) noexcept;
void keyPressEventF2(QtConceptMap& q) noexcept;
void keyPressEventF4(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventH(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventN(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventQuestion(QtConceptMap& q, QKeyEvent *) noexcept;
void keyPressEventT(QtConceptMap& q, QKeyEvent *event) noexcept;
void keyPressEventZ(QtConceptMap& q, QKeyEvent *event) noexcept;

///Called when an item wants to be edited
void OnEdgeKeyDownPressed(QtConceptMap& q, QtEdge * const item, const int key);

///Called when an item wants to be edited
void OnNodeKeyDownPressed(QtConceptMap& q, QtNode* const item, const int key);

void onSelectionChanged(QtConceptMap& q);

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
