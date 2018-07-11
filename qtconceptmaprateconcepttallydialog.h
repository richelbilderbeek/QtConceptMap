#ifndef QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H
#define QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H





#include <QDialog>
//#include "qthideandshowdialog.h"
#include "qtconceptmapfwd.h"
#include "conceptmap.h"


struct QTableWidgetItem;
namespace Ui { class QtRateConceptTallyDialog; }

namespace ribi {
namespace cmap {

class QtRateConceptTallyDialog : public QDialog
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

  ///
  Concept GetConcept() const noexcept;

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

  ///Will throw if col is absent
  void ChangeConceptExample(
    Concept& concept, const int index, const QTableWidgetItem& item, const int col
  );

  ///Will throw if col is absent
  void ChangeConceptName(
    Concept& concept, const QTableWidgetItem& item, const int col
  );


  std::vector<Row> CreateData(const ConceptMap& map);

  ///Put uneditable nothing in the table its cell
  void PutNothing(const int row, const int col) noexcept;

  void ShowDebugLabel() const noexcept;

  ///Will throw if table has not exactly the right number of columns
  void ShowExample(
    const Concept& concept,
    const int example_index,
    const int row_index
  ) const;

  void ShowNoExample(
    const Concept& concept,
    const int row_index,
    const Row& row,
    const ConceptMap& conceptmap
  ) noexcept;
};


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H
