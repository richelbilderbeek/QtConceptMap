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
#ifndef QTCONCEPTMAPEDGEDIALOG_H
#define QTCONCEPTMAPEDGEDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <memory>
#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#include "conceptmapedge.h"
#pragma GCC diagnostic pop

namespace Ui { class QtEdgeDialog; }

namespace ribi {
namespace cmap {

///Displays and modifies an Edge
class QtEdgeDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtEdgeDialog(
    const Edge& edge,
    QWidget *parent = 0
  );
  QtEdgeDialog(const QtEdgeDialog&) = delete;
  QtEdgeDialog& operator=(const QtEdgeDialog&) = delete;
  ~QtEdgeDialog() noexcept;

  Edge GetEdge() const noexcept { return m_edge; }
  static int GetMinimumHeight(const Edge& edge) noexcept;

  ///Get if there is an arrow at the head of the edge directly from the GUI
  bool GetUiHasHeadArrow() const noexcept;
  ///Get if there is an arrow at the tail of the edge directly from the GUI
  bool GetUiHasTailArrow() const noexcept;
  ///Get the X coordinat directly from the GUI
  double GetUiX() const noexcept;
  ///Get the Y coordinat directly from the GUI
  double GetUiY() const noexcept;

  void SetEdge(const Edge& edge);

  ///Set if there is an arrow at the head of the edge directly to the GUI
  void SetUiHasHeadArrow(const bool has_head) noexcept;
  ///Set if there is an arrow at the tail of the edge directly to the GUI
  void SetUiHasTailArrow(const bool has_tail) noexcept;
  ///Set the X coordinat directly to the GUI
  void SetUiX(const double x ) noexcept;
  ///Set the Y coordinat directly to the GUI
  void SetUiY(const double y) noexcept;

private:
  Ui::QtEdgeDialog *ui;

  ///The Edge to work on
  Edge m_edge;

  std::shared_ptr<QtNodeDialog> m_qtnodedialog; //The center node
  std::shared_ptr<QtNodeDialog> m_qtnodedialog_from;
  std::shared_ptr<QtNodeDialog> m_qtnodedialog_to;

};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPEDGEDIALOG_H
