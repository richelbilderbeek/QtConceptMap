# Qt does not go well with -Weffc++
# Qwt does not go well with -Weffc++
# apfloat does not go well with -Weffc++
# RInside does not go well with -Weffc++

win32 {
  # Windows only
  message("Desktop application, no effc++, built for Windows")
  greaterThan(QT_MAJOR_VERSION, 4): QT += svg
  QMAKE_CXXFLAGS += -std=c++1y -Wall -Wextra #-Weffc++
}

macx {
  # Mac only
  message("Desktop application, no effc++, built for Mac")
  QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+
  CONFIG +=c++1y
}

unix:!macx{
  # Linux only
  message("Desktop application, no effc++, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++11

  equals(QT_MAJOR_VERSION, 4): LIBS +=  -lQtSvg
  greaterThan(QT_MAJOR_VERSION, 4): QT +=  concurrent opengl printsupport svg
}

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

CONFIG(release, debug|release) {
  message(Release mode)
  DEFINES += NDEBUG
}

include(../RibiClasses/CppAbout/CppAbout.pri)
include(../RibiClasses/CppFileIo/CppFileIo.pri)
include(../RibiClasses/CppHelp/CppHelp.pri)
include(../RibiClasses/CppMenuDialog/CppMenuDialog.pri)
#include(../RibiClasses/CppTestTimer/CppTestTimer.pri)
#include(../RibiClasses/CppTrace/CppTrace.pri)

#include(../RibiLibraries/GeneralConsole.pri)
#include(../RibiLibraries/Apfloat.pri)
#include(../RibiLibraries/BoostAll.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

# Do not test console classes
include(../RibiClasses/CppContainer/CppContainer.pri)
#include(../RibiClasses/CppCounter/CppCounter.pri)
include(../RibiClasses/CppConceptMap/CppConceptMap.pri)
#include(../RibiClasses/CppDnaSequence/CppDnaSequence.pri)
#include(../RibiClasses/CppGeometry/CppGeometry.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)
#include(../RibiClasses/CppPlane/CppPlane.pri)
#include(../RibiClasses/CppRibiRandom/CppRibiRandom.pri)
include(../RibiClasses/CppRibiRegex/CppRibiRegex.pri)
#include(../RibiClasses/CppRibiSystem/CppRibiSystem.pri)
#include(../RibiClasses/CppRibiTime/CppRibiTime.pri)
#include(../RibiClasses/CppStopwatch/CppStopwatch.pri)
include(../RibiClasses/CppXml/CppXml.pri)

# Desktop classes
include(../RibiClasses/CppGrabber/CppGrabber.pri)
include(../RibiClasses/CppGrabber/CppGrabberTest.pri)

include(../RibiClasses/CppQtArrowItem/CppQtArrowItem.pri)
include(../RibiClasses/CppQtArrowItem/CppQtArrowItemTest.pri)

include(QtConceptMap.pri)
include(QtConceptMapTest.pri)

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
