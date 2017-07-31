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
    QtConceptMap& qtconceptmap,
    const double x = 0.0,
    const double y = 0.0,
    const std::string& text = ""
  );
  CommandCreateNewNode(const CommandCreateNewNode&) = delete;
  CommandCreateNewNode& operator=(const CommandCreateNewNode&) = delete;
  ~CommandCreateNewNode() noexcept {}

  double GetX() const noexcept;
  double GetY() const noexcept;

  ///Get the Node its text
  std::string GetText() const noexcept;
  void redo() override;
  void undo() override;


  private:
  QtNode * m_added_qtnode;
  const std::string m_text; //Text on the added QtNode
  QtTool * m_tool_item;
  QtNode * m_tool_item_old_buddy;
  const double m_x;
  const double m_y;
};

/// Works on, for example  'create_new_node(0, 0, from)'
CommandCreateNewNode * parse_command_create_new_node(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWNODE_H
