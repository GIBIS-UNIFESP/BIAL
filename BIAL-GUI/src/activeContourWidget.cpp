#include "activeContourWidget.h"
#include "controller.h"
#include "ui_livewirewidget.h"
#ifdef OPENCV
#include <opencv2/ml.hpp>
#endif
#include "FileImage.hpp"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

Controller* ActiveContourWidget::controller( ) const {
  return( m_controller );
}

void ActiveContourWidget::setController( Controller *controller ) {
  m_controller = controller;
}

ActiveContourWidget::ActiveContourWidget( ImageViewer *viewer, QWidget *parent ) :
  m_viewer( viewer ),
  QWidget( parent ),
  ui( new Ui::LiveWireWidget ) {
  ui->setupUi( this );

}

ActiveContourWidget::~ActiveContourWidget( ) {
  delete ui;
}

void ActiveContourWidget::setTool( Tool *sTool ) {
  m_tool = dynamic_cast< ActiveContourTool* >( sTool );
  setEnabled( m_tool != nullptr );
}

void ActiveContourWidget::on_pushButtonClear_clicked( ) {
  m_tool->clear( );
}

void ActiveContourWidget::on_pushButtonRobotUser_clicked( ) {
  try {
    RobotUser mrRoboto( *m_tool );
    mrRoboto.run( );
    qApp->processEvents( );
  }
  catch( std::bad_alloc &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Memory allocation error." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( std::runtime_error &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Runtime error." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( const std::out_of_range &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Out of range exception." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( const std::logic_error &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR(
      "Image, window end, and/or window size dimensions do not match." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
}

void ActiveContourWidget::on_pushButtonSave_clicked( ) {
  QFileInfo finfo( m_tool->getGuiImage( )->fileName( ) );
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
      Bial::Write( m_tool->getResult( ), outFname.toStdString( ), finfo.absoluteFilePath( ).toStdString( ) );
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


void ActiveContourWidget::on_pushButtonProcessAll_clicked( ) {
  try {
    for( int img = 0; img < m_controller->size( ); ++img ) {
      m_controller->setCurrentImagePos( img );
      m_tool->clear( );
      RobotUser mrRoboto( *m_tool );
      for( auto method : m_tool->getMethods( ) ) {
        mrRoboto.runSingle( method );
      }
      mrRoboto.run( );
      qApp->processEvents( );
    }
  }
  catch( std::bad_alloc &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Memory allocation error." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( std::runtime_error &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Runtime error." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( const std::out_of_range &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Out of range exception." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( const std::logic_error &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR(
      "Image, window end, and/or window size dimensions do not match." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
}

void ActiveContourWidget::on_pushButtonClassifier_clicked( ) {
  try {
#ifdef OPENCV
    cv::Ptr< cv::ml::SVM > m_svm = cv::ml::SVM::create( );
    m_svm->setType( cv::ml::SVM::C_SVC );
    m_svm->setKernel( cv::ml::SVM::LINEAR );
    m_svm->setTermCriteria( cv::TermCriteria( cv::TermCriteria::MAX_ITER, 100, 1e-6 ) );
    for( int img = 0; img < m_controller->size( ); ++img ) {
      m_controller->setCurrentImagePos( img );
      GuiImage *guiImg = m_controller->currentImage( );
      Tool::setImageTool( ActiveContourTool::Type, guiImg, m_viewer );
      m_tool = dynamic_cast< ActiveContourTool* >( guiImg->currentTool( ) );
      m_tool->clear( );

      qDebug( ) << m_controller->currentImage( )->fileName( );

      RobotUser mrRoboto( *m_tool );
//      mrRoboto.train( );
      qApp->processEvents( );
    }
    return;
    auto selectedMethods = m_tool->getSelectedMethods( );
    auto methods = m_tool->getMethods( );

    int num_samples = selectedMethods.size( );
    std::vector< FeatureData > trainingData;
    std::vector< int > labels;
    trainingData.reserve( num_samples );
    labels.reserve( num_samples );
    for( int sample = 0; sample < num_samples; ++sample ) {
      for( auto method : methods ) {
        auto features = m_tool->pathDescription( method->m_paths[ sample ], method.get( ) );
        trainingData.push_back( features );
        labels.push_back( ( method->type( ) == selectedMethods[ sample ] ) ? 1 : -1 );
      }
    }
    cv::Mat trainingDataMat( num_samples, NUM_FTR, cv::DataType< float >::type, &trainingData[ 0 ][ 0 ] );
    cv::Mat labelsMat( num_samples, 1, cv::DataType< int >::type, &labels[ 0 ] );
    std::cout << "Train result: " << m_svm->train( trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat ) << std::endl;
#endif
  }
  catch( std::bad_alloc &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Memory allocation error." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( std::runtime_error &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Runtime error." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( const std::out_of_range &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR( "Out of range exception." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
  catch( const std::logic_error &e ) {
    std::string error = std::string( e.what( ) ) + "<br>" + BIAL_ERROR(
      "Image, window end, and/or window size dimensions do not match." );
    QMessageBox::warning( this, tr( "Error" ), QString::fromStdString( error ) );
  }
}
