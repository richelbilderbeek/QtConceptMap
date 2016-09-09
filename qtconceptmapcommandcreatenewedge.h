//---------------------------------------------------------------------------
/*
ConceptMap, concept map classes
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppConceptMap.htm
//---------------------------------------------------------------------------
#ifndef CONCEPTMAPCOMMANDCREATENEWEDGE_H
#define CONCEPTMAPCOMMANDCREATENEWEDGE_H

#include "conceptmap.h"
#include "conceptmapnode.h"
#include "conceptmapedge.h"
#include "qtconceptmapcommand.h"
#include "qtconceptmapmode.h"

struct QGraphicsItem;
struct QGraphicsScene;

namespace ribi {
namespace cmap {

struct QtEdge;
struct QtNode;
struct QtTool;

class CommandCreateNewEdgeBetweenTwoSelectedNodes final : public Command
{
  public:

  CommandCreateNewEdgeBetweenTwoSelectedNodes(
    ConceptMap& conceptmap,
    const Mode mode,
    QGraphicsScene& scene,
    QtTool& tool_item
  );
  CommandCreateNewEdgeBetweenTwoSelectedNodes(
    const CommandCreateNewEdgeBetweenTwoSelectedNodes&
  ) = delete;
  CommandCreateNewEdgeBetweenTwoSelectedNodes& operator=(
    const CommandCreateNewEdgeBetweenTwoSelectedNodes&
  ) = delete;
  ~CommandCreateNewEdgeBetweenTwoSelectedNodes() noexcept;

  void redo() override;
  void undo() override;

  private:
  ConceptMap& m_conceptmap;
  Edge m_added_edge; //The Edge being added or removed
  QtEdge * m_added_qtedge; //The QtEdge being added or removed
  QtNode * m_added_qtnode; //The QtNode being at the center of m_qtedge
  ConceptMap m_after;
  const ConceptMap m_before;
  const Mode m_mode;
  QGraphicsScene& m_scene;
  const QList<QGraphicsItem *> m_selected_before;
  QtTool& m_tool_item;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWEDGE_H
