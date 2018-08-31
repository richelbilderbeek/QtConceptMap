#include "qtconceptmap.h"

#include <QKeyEvent>

#include "qtconceptmapcommandsetconcept.h"
#include "qtconceptmapeditconceptdialog.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"
#include "qtconceptmaprateconceptdialog.h"
#include "qtconceptmapclassifyexamplesdialog.h"

void ribi::cmap::OnNodeKeyDownPressedEditF2(
  QtConceptMap& q,
  QtNode& qtnode,
  QKeyEvent * const event
)
{
  if (event->modifiers() != Qt::NoModifier) return;

  //Cannot edit relations connected to center node
  if (IsOnEdge(qtnode))
  {
    assert(qtnode.parentItem());
    assert(qgraphicsitem_cast<QtEdge*>(qtnode.parentItem()));
    if (
      IsConnectedToCenterNode(
        *qgraphicsitem_cast<QtEdge*>(qtnode.parentItem())
      )
    )
    {
      return;
    }
  }
  assert(event->modifiers() == Qt::NoModifier);

  event->accept();

  //Edit concept
  QtEditConceptDialog d(
    GetConcept(qtnode),
    IsOnEdge(qtnode)
    ? QtEditConceptDialog::EditType::relation
    : QtEditConceptDialog::EditType::concept
  );
  q.setEnabled(false);
  d.exec();
  q.setEnabled(true);

  assert(GetSelectedQtNodesAlsoOnQtEdge(q).size() == 1);
  assert(GetSelectedQtNodesAlsoOnQtEdge(q)[0] == &qtnode);
  if (d.HasUserClickedOk())
  {
    q.DoCommand(new CommandSetConcept(q, d.ToConcept()));
  }
  q.setFocus();
  CheckInvariants(q);
}

void ribi::cmap::OnNodeKeyDownPressedRateF1(
  QtConceptMap& q,
  QtNode& qtnode,
  QKeyEvent * const event
)
{
  if (event->modifiers() != Qt::NoModifier) return;

  event->accept();

  assert(!IsOnEdge(qtnode));
  ribi::cmap::QtRateConceptDialog d(q, qtnode);
  q.setEnabled(false);
  d.exec();
  q.setEnabled(true);
  if (d.HasUserClickedOk())
  {
    d.Write(q, qtnode);
    q.update();
    qtnode.update();
  }
  q.setFocus();
}

void ribi::cmap::OnNodeKeyDownPressedRateF2(
  QtConceptMap& q,
  QtNode& qtnode,
  QKeyEvent * const event
)
{
  if (event->modifiers() != Qt::NoModifier) return;

  //Relation's examples are not rated
  if (IsOnEdge(qtnode)) return;

  //Without examples, there is nothing to rate
  if (!HasExamples(qtnode)) return;

  event->accept();

  QtClassifyExamplesDialog d(GetConcept(qtnode));
  q.setEnabled(false);
  d.exec();
  q.setEnabled(true);
  if (d.HasClickedOk())
  {
    SetExamples(qtnode, d.GetRatedExamples());
  }
  q.setFocus();
}
