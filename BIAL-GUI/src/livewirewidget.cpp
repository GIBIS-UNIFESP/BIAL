#include "controller.h"
#include "livewirewidget.h"
#include "ui_livewirewidget.h"
#include <opencv2/ml.hpp>

#include "FileImage.hpp"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

Controller* LiveWireWidget::controller( ) const {
  return( m_controller );
}

void LiveWireWidget::setController( Controller *controller ) {
  m_controller = controller;
}

LiveWireWidget::LiveWireWidget( QWidget *parent ) :
  QWidget( parent ),
  ui( new Ui::LiveWireWidget ) {
  ui->setupUi( this );
}

LiveWireWidget::~LiveWireWidget( ) {
  delete ui;
}

void LiveWireWidget::setTool( Tool *sTool ) {
  tool = dynamic_cast< LiveWireTool* >( sTool );
  setEnabled( tool != nullptr );
}

void LiveWireWidget::on_pushButtonClear_clicked( ) {
  tool->clear( );
}

void LiveWireWidget::on_pushButtonRobotUser_clicked( ) {
  try {
    tool->roboto( );
  }
  catch( std::runtime_error &err ) {
    QMessageBox::warning( this, tr( "Error" ), QString( err.what( ) ) );
  }
}

void LiveWireWidget::on_pushButtonSave_clicked( ) {
  QFileInfo finfo( tool->getGuiImage( )->fileName( ) );
  QDir dir = finfo.dir( );
  dir.mkpath( "segmentation" );
  dir.cd( "segmentation" );
  QString outFname = dir.absoluteFilePath( finfo.baseName( ) + ".pgm" );
  Bial::Write( tool->getResult( ), outFname.toStdString( ), finfo.absoluteFilePath( ).toStdString( ) );
}

//  if( m_points.size( ) > 1 ) {
//    float testData[ m_methods.size( ) ][ NUM_FTR ];
//    for( int m = 0; m < m_methods.size( ); ++m ) {
//      auto method = m_methods[ m ];
//      auto features = pathDescription( paths[ m ], method.get( ) );
//      for( int ftr = 0; ftr < features.size( ); ++ftr ) {
//        testData[ m ][ ftr ] = features[ ftr ];
//      }
//    }
//    cv::Mat testDataMat( m_methods.size( ), NUM_FTR, cv::DataType< float >::type, testData );
//    cv::Mat responsesMat( m_methods.size( ), 1, cv::DataType< int >::type );
//    std::cout << "SVM PREDICT: " << m_svm->predict( testDataMat, responsesMat ) << std::endl;
//    for( int m = 0; m < m_methods.size( ); ++m ) {
//      std::cout << "LABEL [" << m << "] = " << responsesMat.at< int >( m ) << std::endl;
//    }
//  }


void LiveWireWidget::on_pushButtonProcessAll_clicked( ) {

  cv::Ptr< cv::ml::SVM > m_svm = cv::ml::SVM::create( );
  m_svm->setType( cv::ml::SVM::C_SVC );
  m_svm->setKernel( cv::ml::SVM::LINEAR );
  m_svm->setTermCriteria( cv::TermCriteria( cv::TermCriteria::MAX_ITER, 100, 1e-6 ) );
  for( int img = 0; img < m_controller->size( ); ++img ) {
    m_controller->setCurrentImagePos( img );
    tool->clear( );
    qDebug( ) << m_controller->currentImage( )->fileName( );
    qDebug( ) << "Comment êtes-vous, Monsieur Robot?";
    RobotUser mrRoboto( *tool );
    qDebug( ) << "Courez, Mr. Robot!";
    mrRoboto.train( );
    qApp->processEvents( );
  }
  auto selectedMethods = tool->getSelectedMethods( );
  auto methods = tool->getMethods( );

  int num_samples = selectedMethods.size( );
  std::vector< FeatureData > trainingData;
  std::vector< int > labels;
  trainingData.reserve( num_samples );
  labels.reserve( num_samples );
  for( int sample = 0; sample < num_samples; ++sample ) {
    for( auto method : methods ) {
      auto features = tool->pathDescription( method->m_paths[ sample ], method.get( ) );
      trainingData.push_back( features );
      labels.push_back( ( method->type( ) == selectedMethods[ sample ] ) ? 1 : -1 );
    }
  }
  cv::Mat trainingDataMat( num_samples, NUM_FTR, cv::DataType< float >::type, &trainingData[ 0 ][ 0 ] );
  cv::Mat labelsMat( num_samples, 1, cv::DataType< int >::type, &labels[ 0 ] );
  std::cout << "Train result: " << m_svm->train( trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat ) << std::endl;
}
