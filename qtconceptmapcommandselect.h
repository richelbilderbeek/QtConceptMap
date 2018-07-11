#ifndef CONCEPTMAPCOMMANDSELECT_H
#define CONCEPTMAPCOMMANDSELECT_H





#include "qtconceptmapcommand.h"


namespace ribi {
namespace cmap {

///Start a new node
///-Can be used only when there is an existing concept map
class CommandSelect final : public Command
{
  public:

  ///@param item a QtEdge or QtNode
  ///Will throw if item is not a QtEdge nor QtNode
  CommandSelect(
    QtConceptMap& qtconceptmap,
    QGraphicsItem& item
  );
  CommandSelect(const CommandSelect&) = delete;
  CommandSelect& operator=(const CommandSelect&) = delete;
  ~CommandSelect() noexcept {}

  void Redo() override;
  void Undo() override;

  private:

  ///The actual command
  Command * m_cmd;
};

/// Works on, for example  'select(from)'
CommandSelect * ParseCommandSelect(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSELECT_H
