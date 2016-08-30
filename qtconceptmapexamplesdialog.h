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
#ifndef QTCONCEPTMAPEXAMPLESDIALOG_H
#define QTCONCEPTMAPEXAMPLESDIALOG_H

#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "conceptmapexamples.h"
#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace Ui { class QtExamplesDialog; }

namespace ribi {
namespace cmap {

///Displays and modifies Examples
class QtExamplesDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtExamplesDialog(QWidget *parent = 0);
  QtExamplesDialog(const QtExamplesDialog&) = delete;
  QtExamplesDialog& operator=(const QtExamplesDialog&) = delete;
  ~QtExamplesDialog() noexcept;

  void SetExamples(const Examples& examples);
  const Examples& GetExamples() const noexcept { return m_examples; }
        Examples& GetExamples()       noexcept { return m_examples; }

  static int GetMinimumHeight(const Examples& examples) noexcept;

private:
  Ui::QtExamplesDialog *ui;
  std::vector<boost::shared_ptr<QtExampleDialog>> m_dialogs;
  Examples m_examples;

  void OnExamplesChanged(Examples& examples) noexcept;
};

} //~namespace ribi
} //~namespace cmap

#endif // QTCONCEPTMAPEXAMPLESDIALOG_H
