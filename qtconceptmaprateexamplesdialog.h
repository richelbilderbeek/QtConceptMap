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
#ifndef QTCONCEPTMAPRATEEXAMPLESDIALOG_H
#define QTCONCEPTMAPRATEEXAMPLESDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"

#include "qtconceptmapfwd.h"
#include "conceptmapconcept.h"
#pragma GCC diagnostic pop

namespace Ui { class QtRateExamplesDialog; }

namespace ribi {
namespace cmap {

///Allows the user to rate the examples of a concept
class QtRateExamplesDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT
  
public:
  explicit QtRateExamplesDialog(
    const Concept& concept,
    QWidget* parent = 0
  );
  QtRateExamplesDialog(const QtRateExamplesDialog&) = delete;
  QtRateExamplesDialog& operator=(const QtRateExamplesDialog&) = delete;
  ~QtRateExamplesDialog() noexcept;

  ///Obtain the rated examples
  Examples GetRatedExamples() const;

protected:
  void keyPressEvent(QKeyEvent *);

public slots:
  void on_button_prof_clicked();
  void on_button_organisations_clicked();
  void on_button_social_clicked();
  void on_button_target_audience_clicked();
  void on_button_prof_development_clicked();
  void on_button_misc_clicked();
  void on_button_ti_knowledge_clicked();
  void on_button_ok_clicked();

private:
  Ui::QtRateExamplesDialog *ui;

  ///The concept, which is modified when clicking OK, but remains unmodified when
  ///the user clicks cancel
  Concept m_concept;

  const Concept m_concept_at_start;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATEEXAMPLESDIALOG_H
