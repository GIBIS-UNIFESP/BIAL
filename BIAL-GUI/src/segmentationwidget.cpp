#include "FileImage.hpp"
#include "segmentationwidget.h"
#include "ui_segmentationwidget.h"
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

SegmentationWidget::SegmentationWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::Segmentationwidget ) {
  ui->setupUi( this );
}

SegmentationWidget::~SegmentationWidget( ) {
  delete ui;
}

void SegmentationWidget::setTool( Tool *sTool ) {
  tool = dynamic_cast< SegmentationTool* >( sTool );
  if( tool ) {
    setEnabled( true );
    /* atualiza os dados da interface */
    ui->BetaSpinBox->setValue( tool->getBeta( ) );
    ui->AlphaSpinBox->setValue( tool->getAlpha( ) );
    ui->pushButtonShowSeeds->setChecked( tool->getSeedsVisible( ) );
    ui->pushButtonShowMask->setChecked( tool->getMaskVisible( ) );
    switch( tool->getDrawType( ) ) {
        case 0:
        ui->eraserButton->setChecked( true );
        break;
        case 1:
        case 2:
        ui->drawButton->setChecked( true );
        break;
    }
  }
  else {
    setEnabled( false );
  }
}

void SegmentationWidget::on_SegmentationButton_clicked( ) {
  double alpha = ui->AlphaSpinBox->value( );
  double beta = ui->BetaSpinBox->value( );
  int pf_type = ( ui->pfmaxgeo->isChecked( ) ? 0 : ( ui->pfmax->isChecked( ) ? 1 : 2 ) );
  try {
    tool->connect( pf_type, alpha, beta );
  }
  catch( std::runtime_error &err ) {
    QMessageBox::warning( this, "ERROR", err.what( ) );
  }
}

void SegmentationWidget::on_eraserButton_clicked( ) {
  tool->setDrawType( 0 );
  tool->setSeedsVisibility( true );
}

void SegmentationWidget::on_drawButton_clicked( ) {
  tool->setDrawType( 1 );
  tool->setSeedsVisibility( true );
}

void SegmentationWidget::on_ClearButton_clicked( ) {
  tool->clearSeeds( );
}

void SegmentationWidget::on_AlphaSpinBox_valueChanged( double arg1 ) {
  tool->setAlpha( arg1 );
}

void SegmentationWidget::on_BetaSpinBox_valueChanged( double arg1 ) {
  tool->setBeta( arg1 );
}

void SegmentationWidget::on_pushButtonShowSeeds_clicked( bool checked ) {
  tool->setSeedsVisibility( checked );
}

void SegmentationWidget::on_pushButtonShowMask_clicked( bool checked ) {
  tool->setMaskVisibility( checked );
}

void SegmentationWidget::on_thickDoubleSpinBox_valueChanged( double arg1 ) {
  tool->setThickness( arg1 );
}

void SegmentationWidget::on_pfmaxgeo_toggled( bool checked ) {
//    if( tool->isInitiated( ) ) {
//      QMessageBox messageBox;
//      messageBox.critical(0,"Warining","Changing path-value propagation function after first execution will restart
// the process.");
//      messageBox.setFixedSize(500,200);
//    }
}

void SegmentationWidget::on_pfmax_toggled( bool checked ) {
//    if( tool->isInitiated( ) ) {
//      QMessageBox messageBox;
//      messageBox.critical(0,"Warining","Changing path-value propagation function after first execution will restart
// the process.");
//      messageBox.setFixedSize(500,200);
//    }
}

void SegmentationWidget::on_pfsum_toggled( bool checked ) {
//    if( tool->isInitiated( ) ) {
//      QMessageBox messageBox;
//      messageBox.critical(0,"Warining","Changing path-value propagation function after first execution will restart
// the process.");
//      messageBox.setFixedSize(500,200);
//    }
}

void SegmentationWidget::on_morphological_grad_toggled( bool checked ) {
  tool->MorphologicalGradient( );
}

void SegmentationWidget::on_sobel_grad_toggled( bool checked ) {
  tool->SobelGradient( );
}


void SegmentationWidget::on_pushButtonSave_clicked( ) {
  QFileInfo finfo( tool->getGuiImage( )->fileName( ) );
  QDir dir = finfo.dir( );
  dir.mkpath( "segmentation" );
  dir.cd( "segmentation" );
  QString outFname = dir.absoluteFilePath( finfo.baseName( ) + ".pgm" );
  Bial::Write( tool->getMask( ), outFname.toStdString( ), finfo.absoluteFilePath( ).toStdString( ) );
}
