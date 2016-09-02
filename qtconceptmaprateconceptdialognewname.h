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
#ifndef QTCONCEPTMAPRATECONCEPTDIALOG_H
#define QTCONCEPTMAPRATECONCEPTDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"

#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"
#include "conceptmap.h"
#pragma GCC diagnostic pop

namespace Ui { class QtRateConceptDialog; }
namespace ribi {
namespace cmap {

///Rate the focal concept of a sub-ConceptMap.
class QtRateConceptDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT
    
  public:
  ///concept is the center node
  ///conceptmap[0] is the same as concept and might be changed
  ///conceptmap is non-const, as GetRatedConcept will produce a new concept
  explicit QtRateConceptDialog(const ConceptMap conceptmap,
    QWidget* parent = 0);
  QtRateConceptDialog(const QtRateConceptDialog&) = delete;
  QtRateConceptDialog& operator=(const QtRateConceptDialog&) = delete;
  ~QtRateConceptDialog() noexcept;

  ///To distinguish between closing the dialog by clicking OK, or by ALT-F4
  bool GetOkClicked() const noexcept { return m_button_ok_clicked; }

  int GetComplexity() const noexcept;
  int GetConcreteness() const noexcept;
  int GetSpecificity() const noexcept;


protected:
  void keyPressEvent(QKeyEvent *);

private slots:
  void on_button_ok_clicked();
  void on_button_tally_relevancies_clicked();


private:
  Ui::QtRateConceptDialog *ui;

  ///To distinguish between closing the dialog by clicking OK, or by ALT-F4
  bool m_button_ok_clicked;

  ///The center concept, may be changed when the user clicks OK
  Concept m_concept;

  ///Cannot be const, only used in calculating the suggestions
  const ConceptMap m_conceptmap;

  const boost::shared_ptr<QtConceptMap> m_widget;

  void DisplaySuggestions() noexcept;

};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTDIALOG_H
