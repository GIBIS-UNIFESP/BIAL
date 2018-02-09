#include "labelswidget.h"
#include "ui_labelswidget.h"

LabelsWidget::LabelsWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::LabelsWidget ) {
  ui->setupUi( this );
}

LabelsWidget::~LabelsWidget( ) {
  delete ui;
}

void LabelsWidget::setTool( Tool *sTool ) {
  tool = dynamic_cast< DefaultTool* >( sTool );
  if( tool && tool->hasLabel( ) ) {
    setEnabled( true );
    switch( tool->getLabelType( ) ) {
        case LabelType::none:
        ui->none->setChecked( true );
        break;
        case LabelType::solid:
        ui->solid->setChecked( true );
        break;
        case LabelType::translucent:
        ui->translucent->setChecked( true );
        break;
        case LabelType::multilabel: {
        ui->multilabel->setChecked( true );
        break;
      }
    }
    int tf = static_cast< int >( tool->getFactor( ) );
    ui->translucentSlider->setValue( tf );
  }
  else {
    setEnabled( false );
  }
}

void LabelsWidget::on_none_clicked( ) {
  tool->setLabelType( LabelType::none );
}

void LabelsWidget::on_solid_clicked( ) {
  tool->setLabelType( LabelType::solid );
}

void LabelsWidget::on_translucent_clicked( ) {
  tool->setLabelType( LabelType::translucent );
}

void LabelsWidget::on_multilabel_clicked( ) {
  tool->setLabelType( LabelType::multilabel );
}


void LabelsWidget::on_translucentSlider_valueChanged( int value ) {
  tool->setTranslucentFactor( ui->translucentSlider->value( ) );
}

void LabelsWidget::on_pushButton_clicked( ) {
  tool->removeLabel( );
}
