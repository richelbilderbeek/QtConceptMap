
/*














*/



#ifndef QTCONCEPTMAPCOMPETENCY_H
#define QTCONCEPTMAPCOMPETENCY_H

#include <map>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <QColor>
#include <QIcon>
#include "conceptmapcompetency.h"

#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace ribi {
namespace cmap {

///Class to convert Competency to QColor and vice versa
struct QtCompetency
{
  QtCompetency();

  ///Convert a QColor to a Competency
  Competency ColorToCompetency(const QColor& color) const;

  ///Convert a Competency to a QColor
  QColor CompetencyToColor(const Competency competency) const;

  ///Convert a Competency to a QIcon
  QIcon CompetencyToIcon(const Competency competency) const;

  ///Convert a QIcon to a Competency
  Competency IconToCompetency(const QIcon& icon) const;

  private:
  ///The map between Competency and QColor
  const std::map<Competency,QColor> m_color_map;

  ///The map between Competency and QIcon
  const std::map<Competency,QIcon> m_icon_map;

};

///Create map between Competency and QColor
std::map<Competency,QColor> CreateColorMap() noexcept;

///Create map between Competency and QIcon
std::map<Competency,QIcon> CreateIconMap() noexcept;


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPCOMPETENCY_H
