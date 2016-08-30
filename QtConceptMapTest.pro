# Qt does not go well with -Weffc++
include(../RibiLibraries/DesktopApplicationNoWeffcpp.pri)

include(../RibiLibraries/GeneralConsole.pri)
include(../RibiLibraries/Apfloat.pri)
#include(../RibiLibraries/BoostAll.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

# Do not test console classes
include(../RibiClasses/CppContainer/CppContainer.pri)
include(../RibiClasses/CppCounter/CppCounter.pri)
include(../RibiClasses/CppConceptMap/CppConceptMap.pri)
include(../RibiClasses/CppDnaSequence/CppDnaSequence.pri)
include(../RibiClasses/CppGeometry/CppGeometry.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
include(../RibiClasses/CppPlane/CppPlane.pri)
include(../RibiClasses/CppRibiRandom/CppRibiRandom.pri)
include(../RibiClasses/CppRibiRegex/CppRibiRegex.pri)
include(../RibiClasses/CppRibiSystem/CppRibiSystem.pri)
include(../RibiClasses/CppRibiTime/CppRibiTime.pri)
include(../RibiClasses/CppStopwatch/CppStopwatch.pri)
include(../RibiClasses/CppXml/CppXml.pri)

# Desktop classes
include(../RibiClasses/CppGrabber/CppGrabber.pri)
include(../RibiClasses/CppGrabber/CppGrabberTest.pri)

include(../RibiClasses/CppQtArrowItem/CppQtArrowItem.pri)
include(../RibiClasses/CppQtArrowItem/CppQtArrowItemTest.pri)

include(../RibiClasses/CppQtConceptMap/CppQtConceptMap.pri)
include(../RibiClasses/CppQtConceptMap/CppQtConceptMapTest.pri)

include(../RibiClasses/CppQtDisplayPosItem/CppQtDisplayPosItem.pri)
include(../RibiClasses/CppQtDisplayPosItem/CppQtDisplayPosItemTest.pri)

include(../RibiClasses/CppQtHideAndShowDialog/CppQtHideAndShowDialog.pri)
include(../RibiClasses/CppQtHideAndShowDialog/CppQtHideAndShowDialogTest.pri)

include(../RibiClasses/CppQtGraphics/CppQtGraphics.pri)
include(../RibiClasses/CppQtGraphics/CppQtGraphicsTest.pri)

include(../RibiClasses/CppQtKeyboardFriendlyGraphicsView/CppQtKeyboardFriendlyGraphicsView.pri)
include(../RibiClasses/CppQtKeyboardFriendlyGraphicsView/CppQtKeyboardFriendlyGraphicsViewTest.pri)

include(../RibiClasses/CppQtQuadBezierArrowItem/CppQtQuadBezierArrowItem.pri)
include(../RibiClasses/CppQtQuadBezierArrowItem/CppQtQuadBezierArrowItemTest.pri)

include(../RibiClasses/CppQtImage/CppQtImage.pri)
include(../RibiClasses/CppQtImage/CppQtImageTest.pri)

include(../RibiClasses/CppQtRoundedEditRectItem/CppQtRoundedEditRectItem.pri)
include(../RibiClasses/CppQtRoundedEditRectItem/CppQtRoundedEditRectItemTest.pri)

include(../RibiClasses/CppQtRoundedRectItem/CppQtRoundedRectItem.pri)
include(../RibiClasses/CppQtRoundedRectItem/CppQtRoundedRectItemTest.pri)

include(../RibiClasses/CppQtScopedDisable/CppQtScopedDisable.pri)
include(../RibiClasses/CppQtScopedDisable/CppQtScopedDisableTest.pri)

SOURCES += \
    main_test.cpp

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# Boost.Graph
LIBS += \
  -lboost_date_time \
  -lboost_graph \
  -lboost_regex

# QResources give this error
QMAKE_CXXFLAGS += -Wno-unused-variable

# Qt:
# QtConcurrent::filterInternal(Sequence&, KeepFunctor, ReduceFunctor)’:
# /usr/include/qt4/QtCore/qtconcurrentfilter.h:108:47: error: typedef ‘Iterator’ locally defined but not used [-Werror=unused-local-typedefs]
# typedef typename Sequence::const_iterator Iterator;
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs

# QTest
QT += testlib
