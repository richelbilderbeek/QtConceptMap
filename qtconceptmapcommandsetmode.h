#ifndef CONCEPTMAPCOMMANDSETMODE_H
#define CONCEPTMAPCOMMANDSETMODE_H

#include "qtconceptmapcommand.h"
#include "qtconceptmapmode.h"

namespace ribi {
namespace cmap {

class CommandSetMode final : public Command
{
  public:

  CommandSetMode(
    QtConceptMap& qtconceptmap,
    const Mode m
  );
  CommandSetMode(const CommandSetMode&) = delete;
  CommandSetMode& operator=(const CommandSetMode&) = delete;
  ~CommandSetMode() noexcept;

  ///Get the new Mode
  Mode GetMode() const noexcept { return m_mode; }

  void redo() override;
  void undo() override;


  private:

  ///The new mode
  const Mode m_mode;

  ///The mode before setting the new mode
  Mode m_mode_prev;
};

CommandSetMode * ParseCommandSetMode(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSETMODE_H
