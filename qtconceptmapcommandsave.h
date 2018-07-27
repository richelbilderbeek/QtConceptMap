#ifndef CONCEPTMAPCOMMANDSAVE_H
#define CONCEPTMAPCOMMANDSAVE_H





#include "qtconceptmapcommand.h"


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
  ~CommandSave() noexcept;

  ///Get the filename
  std::string GetFilename() const noexcept { return m_filename; }
  void Redo() override;
  void Undo() override;

  private:

  const std::string m_filename;
};

/// Works on, for example  'save(my_file.cmp)'
CommandSave * ParseCommandSave(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDSAVE_H
