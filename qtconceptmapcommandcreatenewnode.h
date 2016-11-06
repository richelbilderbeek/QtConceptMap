
/*
ConceptMap, concept map classes













*/

//From http://www.richelbilderbeek.nl/CppConceptMap.htm

#ifndef CONCEPTMAPCOMMANDCREATENEWNODE_H
#define CONCEPTMAPCOMMANDCREATENEWNODE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

#include "conceptmapnode.h"
#include "conceptmap.h"
#include "qtconceptmapqtnode.h"
#include <QGraphicsScene>
#include "qtconceptmaptoolsitem.h"
#include "qtconceptmapcommand.h"
#include "qtconceptmapmode.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Start a new node
///-Can be used only when there is an existing concept map
class CommandCreateNewNode final : public Command
{
  public:

  CommandCreateNewNode(
    ConceptMap& conceptmap,
    const Mode mode,
    QGraphicsScene& scene,
    QtTool& tool_item,
    const double x,
    const double y
  );
  CommandCreateNewNode(const CommandCreateNewNode&) = delete;
  CommandCreateNewNode& operator=(const CommandCreateNewNode&) = delete;
  ~CommandCreateNewNode() noexcept {}

  void redo() override;
  void undo() override;

  private:
  ConceptMap& m_conceptmap;
  ConceptMap m_conceptmap_after;
  const ConceptMap m_conceptmap_before;
  const Mode m_mode;
  QtNode * m_qtnode;
  QGraphicsScene& m_scene;
  QtTool& m_tool_item;
  QtNode * const m_tool_item_old_buddy;
  const double m_x;
  const double m_y;
};

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWNODE_H
