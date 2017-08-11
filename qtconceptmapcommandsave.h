#ifndef CONCEPTMAPCOMMANDSAVE_H
#define CONCEPTMAPCOMMANDSAVE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapcommand.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Saves the QtConceptMap to file
///Will throw if file already exists
///Will delete the file with undo
class CommandSave final : public Command
{
  public:

  CommandSave(
    QtConceptMap& qtconceptmap,
    const std::string& filename
  );
  CommandSave(const CommandSave&) = delete;
  CommandSave& operator=(const CommandSave&) = delete;
  ~CommandSave() noexcept {}

  ///Get the filename
  std::string GetFilename() const noexcept { return m_filename; }
  void redo() override;
  void undo() override;

  private:

  const std::string m_filename;
};

/// Works on, for example  'save(my_file.cmp)'
CommandSave * ParseCommandSave(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSAVE_H
