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
#ifndef QTCONCEPTMAPNODEDIALOG_H
#define QTCONCEPTMAPNODEDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <boost/shared_ptr.hpp>
#include "qthideandshowdialog.h"
#include "conceptmapnode.h"
#include "qtconceptmapfwd.h"
#pragma GCC diagnostic pop

namespace Ui { class QtNodeDialog; }

namespace ribi {
namespace cmap {

///Displays and modifies a Node
class QtNodeDialog : public ribi::QtHideAndShowDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit QtNodeDialog(QWidget *parent = 0);
  QtNodeDialog(const QtNodeDialog&) = delete;
  QtNodeDialog& operator=(const QtNodeDialog&) = delete;
  ~QtNodeDialog() noexcept;

  static int GetMinimumHeight(const Node& node) noexcept;
  const Node& GetNode() const noexcept { return m_node; }
        Node& GetNode()       noexcept { return m_node; }
  std::string GetUiName() const noexcept;
  ///Read the X value directly from GUI
  double GetUiX() const noexcept;
  ///Read the Y value directly from GUI
  double GetUiY() const noexcept;

  void SetNode(const Node& node) noexcept;

  ///Set the name directly to GUI
  void SetUiName(const std::string& name) noexcept;
  ///Set the X value directly to GUI
  void SetUiX(const double x) noexcept;
  ///Set the Y value directly to GUI
  void SetUiY(const double y) noexcept;


private slots:
  void on_box_x_valueChanged(double arg1);
  void on_box_y_valueChanged(double arg1);

private:
  Ui::QtNodeDialog *ui;

  ///The Node to work on
  Node m_node;

  boost::shared_ptr<QtConceptDialog> m_qtconceptdialog;
};

} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPNODEDIALOG_H
