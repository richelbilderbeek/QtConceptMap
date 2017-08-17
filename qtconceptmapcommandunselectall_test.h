#ifndef QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H
#define QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H

#ifdef FIRST_FINISH_COMMAND_SELECT_ALL

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtConceptMapCommandUnselectAllTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void Parse() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // FIRST_FINISH_COMMAND_SELECT_ALL


#endif // QTCONCEPTMAP_COMMANDUNSELECTALL_TEST_H

