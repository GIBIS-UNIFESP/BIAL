#include "FileImage.hpp"
#include "segmentationwidget.h"
#include "segmentationwidget.h"
#include "ui_segmentationwidget.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
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
    ui->pushButtonSave->setEnabled( tool->hasMask( ) );
    switch( tool->getDrawType( ) ) {
        case 0:
        ui->eraserButton->setChecked( true );
        break;
        case 1:
        case 2:
        ui->drawButton->setChecked( true );
        break;
        case 3:
        ui->LiveWirePostButton->setChecked( true );
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
    int new_val = tool->connect( pf_type, alpha, beta );
    ui->AnchorPointsspinBox->setValue( new_val );
  }
  catch( std::runtime_error &err ) {
    QMessageBox::warning( this, "ERROR", err.what( ) );
  }
}

void SegmentationWidget::on_eraserButton_clicked( ) {
  tool->setDrawType( 0 );
  tool->setSeedsVisibility( true );
  tool->setEllipsesMovable( false );
}

void SegmentationWidget::on_drawButton_clicked( ) {
  tool->setDrawType( 1 );
  tool->setSeedsVisibility( true );
  tool->setEllipsesMovable( false );
}

void SegmentationWidget::on_ClearButton_clicked( ) {
    tool->clearSeeds( );
}

void SegmentationWidget::on_LiveWirePostButton_clicked( ) {
  tool->setDrawType( 3 );
  tool->setSeedsVisibility( true );
  tool->setEllipsesMovable( true );
}

void SegmentationWidget::on_AlphaSpinBox_valueChanged( double arg1 ) {
  tool->setAlpha( arg1 );
}

void SegmentationWidget::on_BetaSpinBox_valueChanged( double arg1 ) {
  tool->setBeta( arg1 );
}

void SegmentationWidget::on_AnchorPointsspinBox_valueChanged( int arg1 ) {
  tool->setAnchors( arg1 );
  tool->LiveWirePostProcessing( arg1 );
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
  QString suffix = finfo.completeSuffix( );
  if( !QString( "pbm,pbm.gz,pgm,pgm.gz,ppm,ppm.gz,pnm,pnm.gz,dcm,dcm.gz,nii,nii.gz"
                ",scn,scn.gz,bmat,bmat.gz" ).split( ',' ).contains( suffix.toLower( ) ) ) {
    suffix = "pnm";
  }
  QString outFname = finfo.dir( ).absoluteFilePath( finfo.baseName( ) + "_seg." + suffix );
  outFname = QFileDialog::getSaveFileName( this, tr( "Save segmentation as.." ),
                                           outFname,
                                           "PBM images (*.pbm *.pbm.gz);;"
                                           "PGM images (*.pgm *.pgm.gz);; PPM images (*.ppm *.ppm.gz);;"
                                           "PNM images (*.pnm *.pnm.gz);; DICOM images (*.dcm *.dcm.gz);;"
                                           "NIfTI images (*.nii *.nii.gz);;"
                                           "SCN Files (*.scn *.scn.gz);;"
                                           "BMAT images (*.bmat *.bmat.gz);;" );
  if( !outFname.isEmpty( ) ) {
    try {
      qDebug( ) << "Saving to " << outFname;

      Bial::Image< int > result = tool->getMask( );
      Bial::Write( result, outFname.toStdString( ), finfo.absoluteFilePath( ).toStdString( ) );
    }
    catch( std::invalid_argument &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( std::bad_alloc &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( std::runtime_error &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( const std::out_of_range &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( const std::logic_error &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
  }
}

void SegmentationWidget::on_pushButtonSaveSeeds_clicked( ) {
  QFileInfo finfo( tool->getGuiImage( )->fileName( ) );
  QString suffix = finfo.completeSuffix( );
  if( !QString( "pbm,pbm.gz,pgm,pgm.gz,ppm,ppm.gz,pnm,pnm.gz,dcm,dcm.gz,nii,nii.gz"
                ",scn,scn.gz,bmat,bmat.gz" ).split( ',' ).contains( suffix.toLower( ) ) ) {
    suffix = "pnm";
  }
  QString outFname = finfo.dir( ).absoluteFilePath( finfo.baseName( ) + "_seeds." + suffix );
  outFname = QFileDialog::getSaveFileName( this, tr( "Save seeds as.." ),
                                           outFname,
                                           "PBM images (*.pbm *.pbm.gz);;"
                                           "PGM images (*.pgm *.pgm.gz);; PPM images (*.ppm *.ppm.gz);;"
                                           "PNM images (*.pnm *.pnm.gz);; DICOM images (*.dcm *.dcm.gz);;"
                                           "NIfTI images (*.nii *.nii.gz);;"
                                           "SCN Files (*.scn *.scn.gz);;"
                                           "BMAT images (*.bmat *.bmat.gz);;" );
  if( !outFname.isEmpty( ) ) {
    try {
      qDebug( ) << "Saving to " << outFname;

      Bial::Image< int > result = tool->getSeeds( );
      Bial::Write( result, outFname.toStdString( ), finfo.absoluteFilePath( ).toStdString( ) );
    }
    catch( std::invalid_argument &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( std::bad_alloc &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( std::runtime_error &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( const std::out_of_range &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
    catch( const std::logic_error &e ) {
      QMessageBox messageBox( this );
      messageBox.critical( 0,
                           "ERROR",
                           "Could not save file:\n" + QString( e.what( ) ) );
    }
  }
}
