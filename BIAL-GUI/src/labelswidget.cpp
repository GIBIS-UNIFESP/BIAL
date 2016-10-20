#include "labelswidget.h"
#include "ui_labelswidget.h"

LabelsWidget::LabelsWidget( QWidget *parent ) :
  QWidget( parent ),
  ui( new Ui::LabelsWidget ) {
  ui->setupUi( this );
}

LabelsWidget::~LabelsWidget( ) {
  delete ui;
}

void LabelsWidget::setTool( Tool *sTool ) {
  tool = dynamic_cast< DefaultTool* >( sTool );
  if( tool && !tool->labels().isEmpty() ) {
    setEnabled( true );
  }
  else {
    setEnabled( false );
  }
}

void LabelsWidget::on_labelSpinBox_editingFinished( ) {

}

void LabelsWidget::on_labelHorizontalSlider_actionTriggered( int action ) {

}

void LabelsWidget::on_solid_clicked( ) {

}

void LabelsWidget::on_translucent_clicked( ) {

}

void LabelsWidget::on_translucentSlider_actionTriggered( int action ) {

}

void LabelsWidget::on_multiLabel_clicked( ) {

}
