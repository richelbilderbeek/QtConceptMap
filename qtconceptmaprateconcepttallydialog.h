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
#ifndef QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H
#define QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#include "conceptmap.h"
#pragma GCC diagnostic pop

struct QTableWidgetItem;
namespace Ui { class QtRateConceptTallyDialog; }

namespace ribi {
namespace cmap {

class QtRateConceptTallyDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT
  
public:
  ///Sub concept map is modified by this dialog
  explicit QtRateConceptTallyDialog(
    const ribi::cmap::ConceptMap& sub_conceptmap,
    QWidget *parent = 0
  );
  QtRateConceptTallyDialog(const QtRateConceptTallyDialog&) = delete;
  QtRateConceptTallyDialog& operator=(const QtRateConceptTallyDialog&) = delete;
  ~QtRateConceptTallyDialog() noexcept;

  ///Obtain the suggested complexity, calculated from this dialog
  int GetSuggestedComplexity() const;

  ///Obtain the suggested concreteness, calculated from this dialog
  int GetSuggestedConcreteness() const;

  ///Obtain the suggested specificity, calculated from this dialog
  int GetSuggestedSpecificity() const;

  const Ui::QtRateConceptTallyDialog * GetUi() const noexcept { return ui; }
        Ui::QtRateConceptTallyDialog * GetUi()       noexcept { return ui; }

protected:
  void keyPressEvent(QKeyEvent *);
  void resizeEvent(QResizeEvent *);
private slots:
  void on_button_ok_clicked();
  void OnCellChanged(int row, int col);

private:
  Ui::QtRateConceptTallyDialog *ui;
  //const ConceptMap m_map;

  ///The concept map is converted to this data type
  ///The std::vector index equals the row
  ///Every row is a pair of a Concept and an integer
  ///The Concept is the concept being judged,
  ///  which might be the concept on the focal node and the concept
  /// on the edges connected to the focal node
  ///The index is the index of the example being judged, or -1,
  /// denoting it is the concept name being judged
  using Row = std::tuple<EdgeDescriptor,Concept,int>;
  std::vector<Row> m_data;

  ///The name of this concept, for example 'my own development'
  const std::string m_focus_name;

  void ChangeConceptExample(Concept& concept, const int index, const QTableWidgetItem& item, const int col) noexcept;
  void ChangeConceptName(Concept& concept, const QTableWidgetItem& item, const int col) noexcept;


  std::vector<Row> CreateData(const ConceptMap& map);

  void ShowDebugLabel() const noexcept;

  void ShowExample(
    const Concept& concept,
    const int example_index,
    const int row_index
  ) const noexcept;

  void ShowNoExample(
    const Concept& concept,
    const int row_index,
    const Row& row,
    const ConceptMap& conceptmap
  ) const noexcept;
};


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H
