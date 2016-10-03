#include "controller.h"
#include "controlswidget.h"
#include "imageviewer.h"
#include "ui_controlswidget.h"
#include <QTimer>

ControlsWidget::ControlsWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::ControlsWidget ) {
  ui->setupUi( this );

  timer = new QTimer( this );
  timer->setInterval( 1000 / ui->spinBoxSpeed->value( ) );
  controller = nullptr;
  ui->pushButton4Views->hide( );
  ui->pushButton_3RGB->hide( );
  /*  ui->groupBoxEditor->hide( ); */

  ui->horizontalSliderZoom->setEnabled( false );
  ui->horizontalSliderBrightness->setEnabled( false );
  ui->horizontalSliderContrast->setEnabled( false );

  /*
   *  ui->horizontalSliderZoom->setEnabled(false);
   *  ui->horizontalSliderBrightness->setEnabled(false);
   *  ui->horizontalSliderContrast->setEnabled(false);
   */

  /*
   *  ui->labelBrightness->setVisible(false);
   *  ui->labelContrast->setVisible(false);
   *  ui->labelZoom->setVisible(false);
   */
}

ControlsWidget::~ControlsWidget( ) {
  delete ui;
}

void ControlsWidget::setController( Controller *value ) {
  controller = value;
  connect( controller, &Controller::currentImageChanged, this, &ControlsWidget::imageChanged );
  connect( controller, &Controller::imageUpdated, this, &ControlsWidget::imageUpdated );
  connect( controller, &Controller::containerUpdated, this, &ControlsWidget::updateRange );
  connect( ui->folderHorizontalSlider, &QAbstractSlider::valueChanged, controller, &Controller::setCurrentImagePos );
  connect( ui->folderSpinBox, SIGNAL(valueChanged(int)), controller, SLOT(setCurrentImagePos(int)) );
  connect( timer, &QTimer::timeout, controller, &Controller::loadNextImage );
  updateRange( );
}

void ControlsWidget::imageChanged( ) {
  GuiImage *img = controller->currentImage( );
  if( img == nullptr ) {
    return;
  }
  ui->pushButtonHistogramNormalization->setChecked( img->getEqualizeHistogram( ) );
  ui->folderHorizontalSlider->setValue( controller->currentImagePos( ) );
  ui->folderSpinBox->setValue( controller->currentImagePos( ) );
  DisplayFormat *format = controller->currentFormat( );
  ui->pushButton1View->setChecked( format->getNumberOfViews( ) == 1 );
  ui->pushButton3Views->setChecked( format->getNumberOfViews( ) == 3 );
  ui->pushButton4Views->setChecked( format->getNumberOfViews( ) == 4 );
  ui->pushButton_1RGB->setChecked( format->getNumberOfViews( ) == 1 );
  ui->pushButton_3RGB->setChecked( format->getNumberOfViews( ) == 3 );
  ui->pushButton_4RGB->setChecked( format->getNumberOfViews( ) == 4 );
  switch( format->currentLayout( ) ) {
      case Layout::GRID:
      ui->pushButtonGrid->setChecked( true );
      break;
      case Layout::HORIZONTAL:
      ui->pushButtonHorizontal->setChecked( true );
      break;
      case Layout::VERTICAL:
      ui->pushButtonVertical->setChecked( true );
      break;
  }
  switch( format->currentViews( ) ) {
      case Views::SHOW0:
      ui->pushButton1View->setChecked( true );
      ui->pushButtonAxial->setChecked( true );
      ui->pushButton_RGB->setChecked( true );
      break;
      case Views::SHOW1:
      ui->pushButton1View->setChecked( true );
      ui->pushButtonCoronal->setChecked( true );
      ui->pushButton_R->setChecked( true );
      break;
      case Views::SHOW2:
      ui->pushButton1View->setChecked( true );
      ui->pushButtonSagittal->setChecked( true );
      ui->pushButton_G->setChecked( true );
      break;
      case Views::SHOW3:
      ui->pushButton1View->setChecked( true );
      ui->pushButton_B->setChecked( true );
      break;
      case Views::SHOW012:
      ui->pushButton3Views->setChecked( true );
      break;
      case Views::SHOW123:
      ui->pushButton3Views->setChecked( true );
      break;
      case Views::SHOW0123:
      ui->pushButton4Views->setChecked( true );
      break;
  }
  ui->groupBoxNiftiViews->setVisible( format->showNiftiViews( ) );
  ui->groupBoxNiftiAxis->setVisible( format->showNiftiAxis( ) );
  ui->groupBoxOrientation->setVisible( format->showOrientation( ) );
  ui->groupBoxPpmViews->setVisible( format->showPpmViews( ) );
  ui->groupBoxPpmChannels->setVisible( format->showPpmChannels( ) );
  ui->rotate->setVisible( format->rotateSingle( ) );
  ui->groupBoxLabels->setVisible( false );

  /* TODO Continue ... */
}

void ControlsWidget::imageUpdated( ) {
}

void ControlsWidget::updateRange( ) {
  size_t images = controller->size( );
  if( images >= 1 ) {
    ui->folderSpinBox->setMaximum( images );
    ui->folderHorizontalSlider->setMaximum( images - 1 );
    ui->folderHorizontalSlider->setMinimum( 0 );
  }
  else if( images == 0 ) {
    ui->folderSpinBox->setMaximum( 0 );
    ui->folderHorizontalSlider->setMaximum( 0 );
    ui->folderHorizontalSlider->setMinimum( 0 );
  }
  ui->folderSpinBox->setEnabled( images > 1 );
  ui->folderHorizontalSlider->setEnabled( images > 1 );
  ui->groupBoxFolderControls->setVisible( images > 1 );
  setVisible( images > 0 );
}

void ControlsWidget::on_spinBoxSpeed_valueChanged( int value ) {
  timer->setInterval( 1000 / value );
}

void ControlsWidget::on_buttonPlay_clicked( bool checked ) {
  if( checked ) {
    timer->start( );
  }
  else {
    timer->stop( );
  }
}

void ControlsWidget::on_pushButton1View_clicked( ) {
  controller->currentFormat( )->setNumberOfViews( 1 );
}

void ControlsWidget::on_pushButton3Views_clicked( ) {
  controller->currentFormat( )->setNumberOfViews( 3 );
}

void ControlsWidget::on_pushButton4Views_clicked( ) {
  controller->currentFormat( )->setNumberOfViews( 4 );
}

void ControlsWidget::on_pushButton_1RGB_clicked( ) {
  controller->currentFormat( )->setNumberOfViews( 1 );
}

void ControlsWidget::on_pushButton_3RGB_clicked( ) {
  controller->currentFormat( )->setNumberOfViews( 3 );
}

void ControlsWidget::on_pushButton_4RGB_clicked( ) {
  controller->currentFormat( )->setNumberOfViews( 4 );
}

void ControlsWidget::on_pushButtonGrid_clicked( ) {
  controller->currentFormat( )->setCurrentLayout( Layout::GRID );
}

void ControlsWidget::on_pushButtonHorizontal_clicked( ) {
  controller->currentFormat( )->setCurrentLayout( Layout::HORIZONTAL );
}

void ControlsWidget::on_pushButtonVertical_clicked( ) {
  controller->currentFormat( )->setCurrentLayout( Layout::VERTICAL );
}

void ControlsWidget::on_pushButtonAxial_clicked( ) {
  controller->currentFormat( )->setCurrentViews( Views::SHOW0 );
}

void ControlsWidget::on_pushButtonCoronal_clicked( ) {
  controller->currentFormat( )->setCurrentViews( Views::SHOW1 );
}

void ControlsWidget::on_pushButtonSagittal_clicked( ) {
  controller->currentFormat( )->setCurrentViews( Views::SHOW2 );
}

void ControlsWidget::on_pushButton_RGB_clicked( ) {
  controller->currentFormat( )->setCurrentViews( Views::SHOW0 );
}

void ControlsWidget::on_pushButton_R_clicked( ) {
  controller->currentFormat( )->setCurrentViews( Views::SHOW1 );
}

void ControlsWidget::on_pushButton_G_clicked( ) {
  controller->currentFormat( )->setCurrentViews( Views::SHOW2 );
}

void ControlsWidget::on_pushButton_B_clicked( ) {
  controller->currentFormat( )->setCurrentViews( Views::SHOW3 );
}

void ControlsWidget::on_pushButtonInterpolation_clicked( ) {
  COMMENT( "Toggled interpolation button.", 1 );
  controller->setInterpolation( ui->pushButtonInterpolation->isChecked( ) );
}

void ControlsWidget::on_rotate_clicked( ) {
  controller->rotateAll90( );
}

void ControlsWidget::on_pushButtonHistogramNormalization_clicked( ) {
  if( controller->currentImage( ) ) {
    controller->currentImage( )->setEqualizeHistogram( ui->pushButtonHistogramNormalization->isChecked( ) );
  }
}

void ControlsWidget::on_pushButtonFitInView_clicked( ) {
  emit controller->currentImageChanged( );
}
