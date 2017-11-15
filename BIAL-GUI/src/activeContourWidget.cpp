#include "activeContourWidget.h"
#include "controller.h"
#include "ui_livewirewidget.h"
#include "FileImage.hpp"
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <fstream>
#include <iterator>


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
  m_SVM = NULL;
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
  QDir dir = finfo.dir( );
  dir.mkpath( "segmentation" );
  dir.cd( "segmentation" );
  QString outFname = dir.absoluteFilePath( finfo.baseName( ) + ".pgm" );
  Bial::Write( m_tool->getResult( ), outFname.toStdString( ), finfo.absoluteFilePath( ).toStdString( ) );
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


    //Parametros da svm
    CvSVMParams m_params;
    m_params.svm_type = CvSVM::C_SVC;
    m_params.kernel_type = CvSVM::LINEAR;
    m_params.term_crit = cvTermCriteria( cv::TermCriteria::MAX_ITER, 100, 1e-6 );

    //Para armazenar os dados extraidos de todas as imagens
    std::vector< FeatureData > trainingData;
    std::vector< int > labels;

    //percorre todas a imagens abertas e executa o treinamento
    for( int img = 0; img < m_controller->size( ); ++img ) {
      m_controller->setCurrentImagePos( img );
      m_tool->clear( );
      qDebug( ) << m_controller->currentImage( )->fileName( );
      qDebug( ) << "Comment êtes-vous, Monsieur Robot?";
      RobotUser MrRoboto( *m_tool );
      qDebug( ) << "Entraînement, Mr.Robot!";
      MrRoboto.train( );
      //////////////////////////////////////////////////////////////////////////
      ///extrai as caracteristicas encontradas no treinamente da imagem atual///
      //////////////////////////////////////////////////////////////////////////
      auto selectedMethods = m_tool->getSelectedMethods( );
      auto methods = m_tool->getMethods( );

      int num_samples = selectedMethods.size( );
      qDebug( ) << "Extracting features";
      for( int sample = 0; sample < num_samples; ++sample ) {
        for( auto method : methods ) {
          if( method->type( ) == selectedMethods[ sample ] ){
            auto features = m_tool->pathDescription( MrRoboto.m_GTpaths[ sample ] );
            qDebug( ) << "Feature: " << features << selectedMethods[ sample ];
            trainingData.push_back( features );
            labels.push_back( selectedMethods[ sample ] );
          }
        }
      }

      qApp->processEvents( );
    }
    std::ofstream output_file("./feature.txt");
    std::ostream_iterator< Bial::Array< double, 90 > > output_iterator(output_file, "\n");
    std::copy(trainingData.begin(), trainingData.end(), output_iterator);

    std::ofstream output_file2("./label.txt");
    std::ostream_iterator< int > output_iterator2(output_file2, "\n");
    std::copy(labels.begin(), labels.end(), output_iterator2);

    qDebug( ) << "Creating Training Matrix";
    cv::Mat trainingDataMat( trainingData.size(), NUM_FTR, cv::DataType< float >::type, &trainingData[ 0 ][ 0 ] );
    cv::Mat labelsMat( labels.size(), 1, cv::DataType< int >::type, &labels[ 0 ] );

    qDebug( ) << "Training SVM with " << trainingData.size() <<" rows and " << labels.size() << " labels";
    //m_SVM = new CvSVM;
    //m_SVM->train(trainingDataMat,labelsMat,cv::Mat(),cv::Mat(),m_params);

    //    std::cout << "Train result: " << m_svm->train( trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat ) << std::endl;
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

void ActiveContourWidget::on_pushButtonTest1_clicked()
{
  try{
    //percorre todas a imagens abertas e executa o treinamento
    if(m_SVM != NULL){
      for( int img = 0; img < m_controller->size( ); ++img ) {
        m_controller->setCurrentImagePos( img );
        m_tool->clear( );
        qDebug( ) << m_controller->currentImage( )->fileName( );
        qDebug( ) << "Comment êtes-vous, Monsieur Robot?";
        RobotUser MrRobot( *m_tool );
        MrRobot.setSVM(m_SVM);
        qDebug( ) << "Courez, Mr. Robot!";
        MrRobot.runTest1();

        qApp->processEvents( );
      }
    }
    else{
      qDebug( ) << "Robo não treinado";
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

void ActiveContourWidget::on_pushButtonTest2_clicked()
{


}
