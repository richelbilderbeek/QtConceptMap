
/*














*/



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
