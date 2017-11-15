#include "graphicsscene.h"
#include "imagewidget.h"
#include "ui_imagewidget.h"
#include <Common.hpp>

ImageWidget::ImageWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::ImageWidget ) {
  ui->setupUi( this );
  m_scene = new GraphicsScene( this );
  ui->graphicsView->setScene( m_scene );
  setViewBgColor( Qt::black );
  QPalette p( palette( ) );
  p.setColor( QPalette::Background, Qt::lightGray );
  setAutoFillBackground( true );
  setPalette( p );
  connect( ui->graphicsView, &GraphicsView::dropImage, this, &ImageWidget::dropImage );
  connect( ui->graphicsView, &GraphicsView::dropFolder, this, &ImageWidget::dropFolder );
}

ImageWidget::~ImageWidget( ) {
  delete ui;
}

void ImageWidget::hideControls( ) {
  ui->frameControls->hide( );
}

void ImageWidget::showControls( ) {
  ui->frameControls->show( );
}

void ImageWidget::setViewBgColor( const QColor &color ) {
  if( ui->graphicsView->scene( ) ) {
    ui->graphicsView->scene( )->setBackgroundBrush( color );
  }
}

GraphicsScene* ImageWidget::scene( ) const {
  return( m_scene );
}

QGraphicsView* ImageWidget::graphicsView( ) {
  return( ui->graphicsView );
}

size_t ImageWidget::viewNumber( ) const {
  return( m_viewNumber );
}

void ImageWidget::setViewNumber( const size_t &viewNumber ) {
  m_viewNumber = viewNumber;
}

void ImageWidget::setRange( int start, int end ) {
  if( start > end ) {
    throw std::invalid_argument( BIAL_ERROR( "start should be smaller than end" ) );
  }
  ui->spinBox->setMinimum( start );
  ui->spinBox->setMaximum( end );

  ui->horizontalSlider->setMinimum( start );
  ui->horizontalSlider->setMaximum( end );
  if( start == end ) {
    ui->spinBox->setEnabled( false );
    ui->horizontalSlider->setEnabled( false );
  }
}

void ImageWidget::setSlice( int slice ) {
  if( ui->spinBox->value( ) != slice ) {
    ui->spinBox->setValue( slice );
  }
  if( ui->horizontalSlider->value( ) != slice ) {
    ui->horizontalSlider->setValue( slice );
  }
}

void ImageWidget::show( ) {
  QWidget::show( );
}

void ImageWidget::on_spinBox_valueChanged( int position ) {
  emit sliceChanged( m_viewNumber, position );
}

void ImageWidget::on_rotateButton_clicked( ) {
  emit rotate( m_viewNumber );
}

void ImageWidget::on_horizontalSlider_valueChanged( int position ) {
  emit sliceChanged( m_viewNumber, position );
}

void ImageWidget::on_flip_h_button_clicked( ) {
  emit fliph( m_viewNumber );
}

void ImageWidget::on_flip_v_button_clicked( ) {
  emit flipv( m_viewNumber );
}
