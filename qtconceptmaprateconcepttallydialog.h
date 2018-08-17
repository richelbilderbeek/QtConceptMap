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

class QtConceptMapRateConceptDialogTest;
class QtConceptMapRateConceptTallyDialogTest;
class QtRateConceptTallyDialogCloser;

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


  ///Count the number of checked complex
  ///examples in the table
  int GetNumberOfCheckedComplexExamples() const;

  ///Count the number of checked complex items
  ///(examples and relations) in the table
  int GetNumberOfCheckedComplexItems() const;


  ///Count the number of checked concrete examples in the table
  int GetNumberOfCheckedConcreteExamples() const;

  ///Count the number of checked specific examples in the table
  int GetNumberOfCheckedSpecificExamples() const;

  ///Count the number of complex relations in the table. That is,
  ///are checked to be complex by the assessor
  int GetNumberOfComplexRelations() const;

  ///Count the number of examples in the table
  int GetNumberOfExamples() const;

  ///Count the number of relations in the table
  int GetNumberOfRelations() const;

  ///Obtain the suggested complexity, calculated from this dialog
  int GetSuggestedComplexity() const;

  ///Obtain the suggested concreteness, calculated from this dialog
  int GetSuggestedConcreteness() const;

  ///Obtain the suggested specificity, calculated from this dialog
  int GetSuggestedSpecificity() const;

  bool HasUserClickedOk() const noexcept { return m_has_user_clicked_ok; }

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
  const Data m_data;

  ///Has the user clicked OK to close this dialog?
  bool m_has_user_clicked_ok{false};

  ///The way the examples are rated
  const ribi::cmap::Rating m_rating;

  ///Extract all information to convert the UI's table's state
  ///to a ConceptMap
  Data CreateData(const ConceptMap& map);

  ///Display the data extracted from the ConceptMap in the table
  void DisplayData();

  ///Put uneditable nothing in the table its cell
  void PutNothing(const int row, const int col) noexcept;


  ///Will throw if table has not exactly the right number of columns
  void ShowExample(
    const int row_index,
    const Row& row
  ) const;

  void ShowNoExample(
    const int row_index,
    const Row& row
  ) noexcept;

  ///Update the suggest XCS on the rating label
  void UpdateRatingLabel() const noexcept;

  friend class QtConceptMapRateConceptDialogTest;
  friend class QtConceptMapRateConceptTallyDialogTest;
  friend class QtRateConceptTallyDialogCloser;
};


} //~namespace cmap
} //~namespace ribi

#endif // QTCONCEPTMAPRATECONCEPTTALLYDIALOG_H
