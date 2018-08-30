#ifndef QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
#define QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H

#include <QtTest/QtTest>

namespace ribi {
namespace cmap {

class QtRatedConceptDialogTest : public QObject
{
    Q_OBJECT //!OCLINT

private slots:
  void AssessorHasRatings() const noexcept;
  void LonelyNodeHasNoRelations() const noexcept;
  void NodeWithRelationsIndicatesThis() const noexcept;
  void StudentHasNoRatings() const noexcept;
  void ToHtml() const noexcept;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEDCONCEPTDIALOG_TEST_H
