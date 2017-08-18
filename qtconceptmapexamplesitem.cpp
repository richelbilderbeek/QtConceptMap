#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapexamplesitem.h"

#include <boost/lambda/lambda.hpp>

#include <QDebug>
#include <QFont>
#include <QPainter>
#include <QGraphicsScene>

#include "conceptmapconcept.h"
#include "conceptmapexample.h"
#include "conceptmapexamples.h"
#include "conceptmaphelper.h"
#include "conceptmapnode.h"
#include "qtconceptmapbrushfactory.h"
#include "qtconceptmapqtedge.h"
#include "qtconceptmapqtnode.h"


#pragma GCC diagnostic pop

ribi::cmap::QtExamplesItem::QtExamplesItem(
  QGraphicsItem* parent)
  : QtRoundedEditRectItem(
      { "..." },
      QtRoundedEditRectItem::Padding(),
      QFont("monospace",9),
      parent
    ),
    m_item{nullptr},
    m_qtedge{nullptr},
    m_qtnode{nullptr}
{
  this->setPen(QPen(QColor(255,0,0)));
  this->setBrush(QtBrushFactory().CreateWhiteGradientBrush());
  this->setFlags(0);
  this->setZValue(2.0);
  this->setVisible(false);
  //this->SetBuddyItem(concept);
}


const ribi::cmap::QtNode * ribi::cmap::QtExamplesItem::GetBuddyItem() const noexcept
{
  if (m_item)
  {
    const QtNode * const qtnode = dynamic_cast<const QtNode*>(m_item);
    assert(qtnode);
    return qtnode;
  }
  return nullptr;
}

void ribi::cmap::QtExamplesItem::paint(
  QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget
) noexcept
{
  //Reposition();
  QtRoundedEditRectItem::paint(painter,option,widget);
}

bool ribi::cmap::IsClose(const QtExamplesItem& item, const QtNode& qtnode) noexcept
{
  /*

                        x2
                        |
   +--------------------+
   |      QtNode        |
   +--------------------+         - y2
               +----------------+ - y1
               | QtExamplesItem |
               +----------------+
                        |
                        x1
  */
  const double x1{item.GetCenterX()};
  const double x2{qtnode.GetOuterRect().right()};
  const double y1{item.GetOuterRect().top()};
  const double y2{qtnode.GetOuterRect().bottom()};
  return std::abs(x1 - x2) < 10.0  && std::abs(y1 - y2) < 10.0;
}

void ribi::cmap::QtExamplesItem::Reposition()
{
  /*

             w
   +--------------------+
   |       QtNode       |
 h |         p          |
   |                    |
   +--------------------+
                +----------------+
                | QtExamplesItem |
                |        q       |
                |                |
                +----------------+

  */
  const auto qtnode = m_qtedge ? m_qtedge->GetQtNode() : m_qtnode;
  assert(qtnode);
  const QPointF p = qtnode->GetCenterPos();
  const auto w = qtnode->GetOuterWidth();
  const auto h = qtnode->GetOuterHeight();
  const QPointF q(
    p.x() + (0.5 * w) + 4.0,
    p.y() + (0.5 * h) + 4.0 + (this->GetOuterHeight() / 2.0)
  );
  this->SetCenterPos(q);
  if (!IsClose(*this, *qtnode))
  {
    qCritical() << "Not close";
  }
}

void ribi::cmap::QtExamplesItem::SetBuddyItem(const QGraphicsItem * const item)
{
  /*

   +--------------------+
   |  QGraphicsItem     |
   +--------------------+
               +----------------+
               | QtExamplesItem |
               +----------------+

  */
  this->setVisible(false);
  m_item = item;
  if (m_item)
  {
    m_qtedge = dynamic_cast<const QtEdge*>(m_item);
    m_qtnode = dynamic_cast<const QtNode*>(m_item);
    if (m_qtedge && !m_qtedge->GetQtNode()->GetNode().GetConcept().GetExamples().Get().empty())
    {
      this->SetExamples(m_qtedge->GetQtNode()->GetNode().GetConcept().GetExamples());
      this->setVisible(true);
    }
    if (m_qtnode && !m_qtnode->GetNode().GetConcept().GetExamples().Get().empty())
    {
      this->SetExamples(m_qtnode->GetNode().GetConcept().GetExamples());
      this->setVisible(true);
    }
    Reposition();
  }
}

void ribi::cmap::QtExamplesItem::SetExamples(const Examples& examples)
{
  std::vector<std::string> v;
  for (const Example& example: examples.Get())
  {
    const std::string s {example.GetText()};
    const std::size_t wordwrap_length{40};
    const std::vector<std::string> w {Wordwrap(s,wordwrap_length)};
    std::copy(w.begin(),w.end(),std::back_inserter(v));
  }
  this->SetText(v);
}
