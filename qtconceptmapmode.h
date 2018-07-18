#ifndef QTCONCEPTMAPMODE_H
#define QTCONCEPTMAPMODE_H

#include <string>
#include <vector>

namespace ribi {
namespace cmap {

///The mode of the concept map:
/// * uninitialized: a plain view mode, best for printing
/// * edit: the concept map is edited by the student
/// * rate: the edited concept map is rated by the assessor
enum class Mode { uninitialized, edit, rate };

Mode str_to_mode(const std::string& s);
std::string to_str(const Mode m) noexcept;

std::vector<Mode> collect_all_modes() noexcept;

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPMODE_H
