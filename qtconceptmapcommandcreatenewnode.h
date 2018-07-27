#ifndef CONCEPTMAPCOMMANDCREATENEWNODE_H
#define CONCEPTMAPCOMMANDCREATENEWNODE_H

#include "qtconceptmapcommand.h"
#include "conceptmapnodetype.h"

namespace ribi {
namespace cmap {

///Start a new node
///-Can be used only when there is an existing concept map
class CommandCreateNewNode final : public Command
{
  public:

  CommandCreateNewNode(
    QtConceptMap& qtconceptmap,
    const std::string& text = "",
    const NodeType type = NodeType::normal,
    const double x = 0.0,
    const double y = 0.0
  );
  CommandCreateNewNode(const CommandCreateNewNode&) = delete;
  CommandCreateNewNode& operator=(const CommandCreateNewNode&) = delete;
  ~CommandCreateNewNode() noexcept;

  double GetX() const noexcept;
  double GetY() const noexcept;

  ///Get the Node its text
  std::string GetText() const noexcept;
  void Redo() override;
  void Undo() override;

  private:
  QtNode * m_added_qtnode;

  ///Type of the added QtNode
  const NodeType m_type;

  ///Text on the added QtNode
  const std::string m_text;

  QtTool * m_tool_item;
  QtNode * m_tool_item_old_buddy;

  ///X coordinat of (center of) added QtNode
  const double m_x;

  ///Y coordinat of (center of) added QtNode
  const double m_y;
};

/// Works on, for example  'create_new_node(0, 0, from)'
CommandCreateNewNode * ParseCommandCreateNewNode(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDCREATENEWNODE_H
