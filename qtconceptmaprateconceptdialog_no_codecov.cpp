#include "qtconceptmaprateconceptdialog.h"

#include "qtconceptmap.h"
#include "qtconceptmaprateconcepttallydialog.h"
#include "ui_qtconceptmaprateconceptdialog.h"

void ribi::cmap::QtRateConceptDialog::on_button_tally_relevancies_clicked()
{
  QtRateConceptTallyDialog d(
    m_sub_conceptmap,
    m_qtconceptmap->GetRating()
  );
  d.exec(); //Keep this dialog visible, as of 2013-08-30
  if (d.HasUserClickedOk())
  {
    ui->box_complexity->setCurrentIndex(d.GetSuggestedComplexity());
    ui->box_concreteness->setCurrentIndex(d.GetSuggestedConcreteness());
    ui->box_specificity->setCurrentIndex(d.GetSuggestedSpecificity());
    d.Write(m_sub_conceptmap);
  }
}
