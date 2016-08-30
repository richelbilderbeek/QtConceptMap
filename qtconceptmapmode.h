#ifndef QTCONCEPTMAPMODE_H
#define QTCONCEPTMAPMODE_H

namespace ribi {
namespace cmap {

///The mode of the concept map:
/// * edit: the concept map is edited by the student
/// * rate: the edited concept map is rated by the assessor
enum class Mode { uninitialized, edit, rate };

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPMODE_H
