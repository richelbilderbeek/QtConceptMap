//---------------------------------------------------------------------------
/*
QtConceptMap, Qt classes for display and interaction with ConceptMap
Copyright (C) 2013-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppQtConceptMap.htm
//---------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapexamplesitem.h"

#include <boost/lambda/lambda.hpp>

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

#include "trace.h"
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

void ribi::cmap::QtExamplesItem::paint(
  QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget
) noexcept
{
  //this->SetExamples(this->m_item->GetNode().GetConcept().GetExamples());
  const auto qtnode = m_qtedge ? m_qtedge->GetQtNode() : m_qtnode;
  assert(qtnode);
  const QPointF p = qtnode->GetCenterPos();
  const auto w = qtnode->GetOuterWidth();
  const auto h = qtnode->GetOuterHeight();
  const QRectF r(-0.5 * w,-0.5 * h,0.5 * w,0.5 * h);
  this->SetCenterPos(
    p.x() + (0.5 * r.width() ) + 4.0 + (0.5 * GetInnerWidth() ),
    p.y() + (0.5 * r.height()) + 4.0 + (0.5 * GetInnerHeight())
  );

  QtRoundedEditRectItem::paint(painter,option,widget);
}

void ribi::cmap::QtExamplesItem::SetBuddyItem(const QGraphicsItem * const item)
{
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
