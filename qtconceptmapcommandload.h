#ifndef CONCEPTMAPCOMMANDLOAD_H
#define CONCEPTMAPCOMMANDLOAD_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "conceptmap.h"
#include "qtconceptmapcommand.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Loads the QtConceptMap to file
///Will throw if file already exists
///Will delete the file with undo
class CommandLoad final : public Command
{
  public:

  CommandLoad(
    QtConceptMap& qtconceptmap,
    const std::string& filename
  );
  CommandLoad(const CommandLoad&) = delete;
  CommandLoad& operator=(const CommandLoad&) = delete;
  ~CommandLoad() noexcept {}

  ///Get the filename
  std::string GetFilename() const noexcept { return m_filename; }
  void redo() override;
  void undo() override;

  private:

  ///ConceptMap before loading
  ConceptMap m_before;
  const std::string m_filename;
};

/// Works on, for example  'load(my_file.cmp)'
CommandLoad * ParseCommandLoad(
  QtConceptMap& qtconceptmap, std::string s);

} //~namespace cmap
} //~namespace ribi

#endif // CONCEPTMAPCOMMANDLOAD_H
