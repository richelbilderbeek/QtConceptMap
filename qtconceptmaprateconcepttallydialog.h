#ifndef QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H
#define QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H

#include <QDialog>
#include "qtconceptmapfwd.h"
#include "conceptmap.h"
#include "conceptmaprating.h"

struct QTableWidgetItem;
namespace Ui { class QtRateConceptTallyDialog; }

namespace ribi {
namespace cmap {

class QtRateConceptTallyDialog final : public QDialog
{
  Q_OBJECT //!OCLINT
  
public:
  ///Sub concept map is modified by this dialog
  explicit QtRateConceptTallyDialog(
    const ribi::cmap::ConceptMap& sub_conceptmap,
    const ribi::cmap::Rating& rating,
    QWidget *parent = 0
  );
  QtRateConceptTallyDialog(const QtRateConceptTallyDialog&) = delete;
  QtRateConceptTallyDialog& operator=(const QtRateConceptTallyDialog&) = delete;
  ~QtRateConceptTallyDialog() noexcept;

  ///
  Concept GetConcept() const noexcept;

  ///Get the modified sub-concept map
  //const ConceptMap& GetConceptMap() const noexcept { return m_conceptmap; }

  ///Obtain the suggested complexity, calculated from this dialog
  int GetSuggestedComplexity() const;

  ///Obtain the suggested concreteness, calculated from this dialog
  int GetSuggestedConcreteness() const;

  ///Obtain the suggested specificity, calculated from this dialog
  int GetSuggestedSpecificity() const;

  const Ui::QtRateConceptTallyDialog * GetUi() const noexcept { return ui; }
        Ui::QtRateConceptTallyDialog * GetUi()       noexcept { return ui; }

  ///Apply the UI to a concept map.
  void Write(ConceptMap& conceptmap) const;

protected:
  void keyPressEvent(QKeyEvent *);
  void resizeEvent(QResizeEvent *);
private slots:
  void on_button_ok_clicked();
  void OnCellChanged(int row, int col);

private:
  Ui::QtRateConceptTallyDialog *ui;

  ///The concept map is converted to this data type
  ///The std::vector index equals the row
  ///Every row is a pair of a Concept and an integer
  ///The Concept is the concept being judged,
  ///  which might be the concept on the focal node and the concept
  /// on the edges connected to the focal node
  ///The index is the index of the example being judged, or -1,
  /// denoting it is the concept (name) itself is being judged
  using Row = std::tuple<
    VertexDescriptor, EdgeDescriptor, //find it in concept map
    Concept,                          //boolean states
    int,                              // example index, -1 if not an example
    QString                           // text, read-only
  >;
  using Data = std::vector<Row>;
  Data m_data;

  ///The name of this concept, for example 'my own development'
  const QString m_focus_name;

  ///The way the examples are rated
  const ribi::cmap::Rating m_rating;

  #ifdef REALLY_NEED_THIS_20180702
  ///Will throw if col is absent
  void ChangeConceptExample(
    Concept& concept, const int index, const QTableWidgetItem& item, const int col
  );

  ///Will throw if col is absent
  void ChangeConceptName(
    Concept& concept, const QTableWidgetItem& item, const int col
  );
  #endif // REALLY_NEED_THIS_20180702

  ///Extract all information to convert the UI's table's state
  ///to a ConceptMap
  Data CreateData(const ConceptMap& map);

  ///Display the data extracted from the ConceptMap in the table
  void DisplayData();

  ///Put uneditable nothing in the table its cell
  void PutNothing(const int row, const int col) noexcept;


  ///Will throw if table has not exactly the right number of columns
  void ShowExample(
    const Concept& concept,
    const int example_index,
    const int row_index,
    const QString& text
  ) const;

  void ShowNoExample(
    const int row_index,
    const Row& row
  ) noexcept;

  ///Update the suggest XCS on the rating label
  void UpdateRatingLabel() const noexcept;
};


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H
