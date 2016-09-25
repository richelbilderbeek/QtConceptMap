#include "qtconceptmapbrushfactory_test.h"

#include "qtconceptmapbrushfactory.h"

void ribi::cmap::qtconceptmapbrushfactory_test::all_tests()
{
  QtBrushFactory b;
  QVERIFY(b.BrushToStr(b.CreateBlueGradientBrush()) == "blue");
  QVERIFY(b.BrushToStr(b.CreateGoldGradientBrush()) == "gold");
  QVERIFY(b.BrushToStr(b.CreateGrayGradientBrush()) == "gray");
  QVERIFY(b.BrushToStr(b.CreateGreenGradientBrush()) == "green");
  QVERIFY(b.BrushToStr(b.CreateRedGradientBrush()) == "red");
  QVERIFY(b.BrushToStr(b.CreateYellowGradientBrush()) == "yellow");
  QVERIFY(b.BrushToStr(b.CreateWhiteGradientBrush()) == "white");
}
