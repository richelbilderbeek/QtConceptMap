#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapbrushfactory.h"
#include <cassert>
#include <QLinearGradient>
#pragma GCC diagnostic pop

const int ribi::cmap::QtBrushFactory::sm_bottom = 10;
const int ribi::cmap::QtBrushFactory::sm_left = -100;
const int ribi::cmap::QtBrushFactory::sm_right = 100;
const int ribi::cmap::QtBrushFactory::sm_top = -10;
const int ribi::cmap::QtBrushFactory::sm_color_low_value  =  64;
const int ribi::cmap::QtBrushFactory::sm_color_mid_value  = 128;

std::string ribi::cmap::QtBrushFactory::BrushToStr(const QBrush& brush)
{
  if (brush == CreateBlueGradientBrush()) return "blue";
  if (brush == CreateGoldGradientBrush()) return "gold";
  if (brush == CreateGrayGradientBrush()) return "gray";
  if (brush == CreateGreenGradientBrush()) return "green";
  if (brush == CreateRedGradientBrush()) return "red";
  if (brush == CreateYellowGradientBrush()) return "yellow";
  if (brush == CreateWhiteGradientBrush()) return "white";
  return "unknown";
}

QBrush ribi::cmap::QtBrushFactory::CreateBlueGradientBrush() noexcept
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,GetBlue());
  linearGradient.setColorAt(1.0,GetWhite());
  QBrush b(linearGradient);
  assert(b.gradient()->stops().front().first == 0.0);
  assert(b.gradient()->stops().front().second == GetBlue());
  assert(b.gradient()->stops().back().first == 1.0);
  assert(b.gradient()->stops().back().second == GetWhite());
  assert(b.gradient()->type() == QGradient::LinearGradient);
  return b;
}

QBrush ribi::cmap::QtBrushFactory::CreateGoldGradientBrush() noexcept
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  //USC gold: 255, 204, 0
  linearGradient.setColorAt(0.0,QColor(255, 204, 0));
  //UCLA gold: 255, 232, 0
  linearGradient.setColorAt(1.0,QColor(255, 232, 0));
  return linearGradient;
}

QBrush ribi::cmap::QtBrushFactory::CreateGrayGradientBrush() noexcept
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor(196,196,196));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

QBrush ribi::cmap::QtBrushFactory::CreateGreenGradientBrush() noexcept
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor( sm_color_mid_value,255, sm_color_mid_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

QBrush ribi::cmap::QtBrushFactory::CreateRedGradientBrush() noexcept
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor(255, sm_color_mid_value, sm_color_mid_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

QBrush ribi::cmap::QtBrushFactory::CreateYellowGradientBrush() noexcept
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor(255,255, sm_color_mid_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

QBrush ribi::cmap::QtBrushFactory::CreateWhiteGradientBrush() noexcept
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  const int low_value = 250;
  linearGradient.setColorAt(0.0,QColor(low_value,low_value,low_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

QColor ribi::cmap::QtBrushFactory::GetBlue() const noexcept
{
  return QColor(
    sm_color_mid_value, sm_color_mid_value,255
  );
}

QColor ribi::cmap::QtBrushFactory::GetWhite() const noexcept
{
  return QColor(255,255,255);
}
