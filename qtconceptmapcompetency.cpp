



#include "qtconceptmapcompetency.h"

#include <cassert>

#include "conceptmapcompetencies.h"

#include <QImage>
#include <QPixmap>

ribi::cmap::QtCompetency::QtCompetency()
  : m_color_map{CreateColorMap()},
    m_icon_map{CreateIconMap()}
{

}

ribi::cmap::Competency ribi::cmap::QtCompetency::ColorToCompetency(const QColor& color) const
{
  assert(!m_color_map.empty());
  const auto iter = std::find_if(m_color_map.begin(),m_color_map.end(),
    [color](const std::pair<cmap::Competency,QColor>& p)
    {
      return
           color.red() == p.second.red()
        && color.green() == p.second.green()
        && color.blue() == p.second.blue();
    }
  );
  assert(iter!=m_color_map.end());
  return iter->first;
}

QColor ribi::cmap::QtCompetency::CompetencyToColor(const cmap::Competency competency) const
{
  assert(!m_color_map.empty());
  const auto iter = m_color_map.find(competency);
  assert(iter!=m_color_map.end());
  return iter->second;
}

QIcon ribi::cmap::QtCompetency::CompetencyToIcon(const cmap::Competency competency) const
{
  assert(!m_icon_map.empty());
  const auto iter = m_icon_map.find(competency);
  assert(iter!=m_icon_map.end());
  QIcon icon = iter->second;
  assert(!icon.isNull());
  return icon;
}

std::map<ribi::cmap::Competency,QColor> ribi::cmap::CreateColorMap() noexcept
{
  return
  {
    { cmap::Competency::uninitialized      , Qt::white },
    { cmap::Competency::profession         , Qt::red },
    { cmap::Competency::organisations      , QColor(255,127,0) }, //Orange
    { cmap::Competency::social_surroundings, Qt::yellow },
    { cmap::Competency::target_audience    , Qt::green },
    { cmap::Competency::ti_knowledge       , Qt::cyan },
    { cmap::Competency::prof_growth        , Qt::blue },
    { cmap::Competency::misc               , Qt::magenta }
  };
}

std::map<ribi::cmap::Competency,QIcon> ribi::cmap::CreateIconMap() noexcept
{
  return
  {
    { cmap::Competency::uninitialized      , QIcon(":/QtConceptMap/pics/White.png") },
    { cmap::Competency::profession         , QIcon(":/QtConceptMap/pics/Red.png") },
    { cmap::Competency::organisations      , QIcon(":/QtConceptMap/pics/Orange.png") },
    { cmap::Competency::social_surroundings, QIcon(":/QtConceptMap/pics/Yellow.png") },
    { cmap::Competency::target_audience    , QIcon(":/QtConceptMap/pics/Green.png") },
    { cmap::Competency::ti_knowledge       , QIcon(":/QtConceptMap/pics/Cyan.png") },
    { cmap::Competency::prof_growth        , QIcon(":/QtConceptMap/pics/Blue.png") },
    { cmap::Competency::misc               , QIcon(":/QtConceptMap/pics/Purple.png") }
  };
}

ribi::cmap::Competency ribi::cmap::QtCompetency::IconToCompetency(const QIcon& icon) const
{
  assert(!m_icon_map.empty());
  const auto iter = std::find_if(m_icon_map.begin(),m_icon_map.end(),
    [icon](const std::pair<Competency,QIcon>& p)
    {

      return icon.pixmap(14,14).toImage() == (p.second).pixmap(14,14).toImage();
      //return icon.name() == (p.second).name();
      //assert(icon.pixmap().size() == (p.second).pixmap().size());
      //return icon.pixmap() == (p.second).pixmap();
    }
  );
  assert(iter!=m_icon_map.end());
  return iter->first;
}
