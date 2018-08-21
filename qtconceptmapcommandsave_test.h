#ifndef QTCONCEPTMAP_COMMANDSAVE_TEST_H
#define QTCONCEPTMAP_COMMANDSAVE_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtCommandSaveTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void Parse() const noexcept;
  void ParseNonsense() const noexcept;
  void Saves() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAP_COMMANDSAVE_TEST_H

