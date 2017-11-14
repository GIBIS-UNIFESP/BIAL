#include "Color.hpp"
#include "controller.h"
#include "controlswidget.h"
#include "defaulttool.h"
#include "dicomdir.h"
#include "imagewidget.h"
#include "mainwindow.h"
#include "thumbswidget.h"
#include "ui_mainwindow.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfoList>
#include <QGraphicsPixmapItem>
#include <QImageReader>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

#include <thirdParty/qcustomplot.h>


//#undef BIAL_WARNING
//#define BIAL_WARNING( exp ) \
//     std::stringstream ss; \
//     ss << __FILE__ << ": " << __LINE__ << ": " << __FUNCTION__ << " Warning: " << exp;\
//     QMessageBox::critical( this, "Erro", "Erro carregando tradução!" );

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ),
  m_controller( new Controller( 4, this ) ) {
  ui->setupUi( this );
  setDockOptions( DockOption::AllowNestedDocks | DockOption::AllowTabbedDocks );

  thumbsWidget = new ThumbsWidget( m_controller, this );
  thumbsWidget->setFixedWidth( 100 );
  thumbsDock = new QDockWidget( tr( "Thumbnails" ), this );
  thumbsDock->setWidget( thumbsWidget );
  addDockWidget( Qt::RightDockWidgetArea, thumbsDock );

  histogramWidget = new QCustomPlot( this );
  histogramDock = new QDockWidget( tr( "Histogram" ), this );
  histogramDock->setWidget( histogramWidget );
  histogramDock->hide( );
  addDockWidget( Qt::LeftDockWidgetArea, histogramDock, Qt::Vertical );

  labelsWidget = new LabelsWidget( this );
  labelsDock = new QDockWidget( tr( "Labels" ), this );
  labelsDock->setWidget( labelsWidget );
  labelsDock->hide( );
  addDockWidget( Qt::LeftDockWidgetArea, labelsDock, Qt::Vertical );

  controlsWidget = new ControlsWidget( m_controller, this );
  controlsDock = new QDockWidget( tr( "Controls" ), this );
  controlsDock->setWidget( controlsWidget );
  controlsDock->hide( );
  addDockWidget( Qt::LeftDockWidgetArea, controlsDock, Qt::Vertical );

  ui->imageViewer->setController( m_controller );
  ui->actionPrint->setEnabled( false );


  segmentationDock = new QDockWidget( tr( "Segmentation" ), this );
  segmentationWidget = new SegmentationWidget( this );
  segmentationWidget->adjustSize( );
  segmentationDock->setWidget( segmentationWidget );

  segmentationDock->hide( );
  segmentationDock->adjustSize( );
  connect( ui->actionSegmentation_dock, &QAction::toggled, segmentationDock, &QDockWidget::setVisible );
  connect( segmentationDock, &QDockWidget::visibilityChanged, ui->actionSegmentation_dock,
           &QAction::setChecked );


  livewireDock = new QDockWidget( tr( "LiveWire" ), this );
  livewireWidget = new ActiveContourWidget( this->ui->imageViewer, this );
  livewireWidget->setController( m_controller );
  livewireWidget->adjustSize( );
  livewireDock->setWidget( livewireWidget );
  livewireDock->hide( );
  livewireDock->adjustSize( );


  connect( ui->actionLiveWire_dock, &QAction::toggled, livewireDock, &QDockWidget::setVisible );
  connect( livewireDock, &QDockWidget::visibilityChanged, ui->actionLiveWire_dock,
           &QAction::setChecked );

  /*
   *  ui->dockWidgetFunctional->hide( );
   *  ui->widgetDragDrop->hide( );
   */

  setupLogoview( );
  createConnections( );
  currentImageChanged( );
  readSettings( );
  createActions( );
  loadQss( );
  containerUpdated( );

  ui->toolBar->setVisible( false );

  QActionGroup *group = new QActionGroup( this );
  actionDefaultTool = group->addAction( "Default Tool" );
  actionSegmentationTool = group->addAction( "Segmentation Tool" );
  actionLiveWireTool = group->addAction( "LiveWire Tool" );

  actionDefaultTool->setCheckable( true );

  actionLiveWireTool->setCheckable( true );

  actionSegmentationTool->setCheckable( true );


  connect( actionDefaultTool, &QAction::triggered, this, &MainWindow::actionDefaultTool_triggered );
  connect( actionSegmentationTool, &QAction::triggered, this, &MainWindow::actionSegmentationTool_triggered );
  connect( actionLiveWireTool, &QAction::triggered, this, &MainWindow::actionLiveWireTool_triggered );

  ui->toolBar->addActions( group->actions( ) );

#ifndef LIBGDCM
  ui->actionOpen_DicomDir->setVisible( false );
#endif

//  tabifyDockWidget( controlsDock, segmentationDock );
//  tabifyDockWidget( controlsDock, livewireDock );
  segmentationDock->setFloating( true );
  livewireDock->setFloating( true );

}

void MainWindow::createConnections( ) {
  /* Show/Hide docks. */
  connect( ui->actionShow_controls_dock, &QAction::toggled, controlsDock, &QDockWidget::setVisible );
  connect( ui->actionHistogram_dock, &QAction::toggled, histogramDock, &QDockWidget::setVisible );
  connect( ui->actionShow_images_dock, &QAction::toggled, thumbsDock, &QDockWidget::setVisible );
  connect( ui->actionLabels_dock, &QAction::toggled, labelsDock, &QDockWidget::setVisible );
  connect( controlsDock, &QDockWidget::visibilityChanged, ui->actionShow_controls_dock, &QAction::setChecked );
  connect( thumbsDock, &QDockWidget::visibilityChanged, ui->actionShow_images_dock, &QAction::setChecked );
  connect( histogramDock, &QDockWidget::visibilityChanged, ui->actionHistogram_dock, &QAction::setChecked );
  connect( labelsDock, &QDockWidget::visibilityChanged, ui->actionLabels_dock, &QAction::setChecked );


  /* Controller. */
  connect( m_controller, &Controller::currentImageChanged, this, &MainWindow::currentImageChanged );
  connect( m_controller, &Controller::imageUpdated, this, &MainWindow::imageUpdated );
  connect( m_controller, &Controller::containerUpdated, this, &MainWindow::containerUpdated );
  connect( m_controller, &Controller::recentFilesUpdated, this, &MainWindow::updateRecentFileActions );

  /* ImageViewer */
  connect( ui->imageViewer, &ImageViewer::mouseClicked, this, &MainWindow::updateIntensity );
  connect( ui->imageViewer, &ImageViewer::mouseReleased, this, &MainWindow::updateIntensity );
  connect( ui->imageViewer, &ImageViewer::mouseDragged, this, &MainWindow::updateIntensity );

  connect( ui->logoView, &GraphicsView::dropImage, this, &MainWindow::loadFile );
  connect( ui->logoView, &GraphicsView::dropFolder, this, &MainWindow::loadFolder );
  /*
   *  connect( ui->imageViewer, &ImageViewer::dropImage, this, &MainWindow::loadFile );
   *  connect( ui->imageViewer, &ImageViewer::dropFolder, this, &MainWindow::loadFolder );
   */
}

void MainWindow::setupLogoview( ) {
  QGraphicsScene *scn = new QGraphicsScene( this );
  QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem( QPixmap( ":/icons/logo_shadow.png" ) );
  scn->addItem( pixmapItem );
  scn->setSceneRect( 0, 0, pixmapItem->pixmap( ).width( ), pixmapItem->pixmap( ).height( ) );
  ui->logoView->setScene( scn );
}

MainWindow::~MainWindow( ) {
  delete ui;
}

void MainWindow::on_actionRed_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::red );
}

void MainWindow::on_actionGreen_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::green );
}

void MainWindow::on_actionBlue_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::blue );
}

void MainWindow::on_actionBlack_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::black );
}

void MainWindow::on_actionWhite_background_triggered( ) {
  ui->imageViewer->setViewBgColor( Qt::white );
}

void MainWindow::currentImageChanged( ) {
  ui->actionNext->setEnabled( m_controller->size( ) > 1 );
  ui->actionPrevious->setEnabled( m_controller->size( ) > 1 );
  if( m_controller->currentImage( ) ) {
    DisplayFormat *format = m_controller->currentFormat( );
    actionDefaultTool->setVisible( DefaultTool::supportedFormats & ( int ) format->modality( ) );
    actionSegmentationTool->setVisible( SegmentationTool::supportedFormats & ( int ) format->modality( ) );

    ui->menuLayout->setEnabled( format->modality( ) != Modality::BW2D );
    ui->menuOverlay->setEnabled( format->hasOverlay( ) );

    ui->actionGrid->setVisible( format->showOrientation( ) );
    ui->actionHorizontal->setVisible( format->showOrientation( ) );
    ui->actionVertical->setVisible( format->showOrientation( ) );

    ui->actionAxial->setVisible( format->modality( ) == Modality::BW3D );
    ui->actionCoronal->setVisible( format->modality( ) == Modality::BW3D );
    ui->actionSagittal->setVisible( format->modality( ) == Modality::BW3D );

    ui->actionRed_channel->setVisible( format->modality( ) == Modality::RGB2D );
    ui->actionGreen_channel->setVisible( format->modality( ) == Modality::RGB2D );
    ui->actionBlue_channel->setVisible( format->modality( ) == Modality::RGB2D );
    ui->actionAll_channels->setVisible( format->modality( ) == Modality::RGB2D );

    ui->action3_Views->setVisible( format->has3Views( ) );
    ui->action4_Views->setVisible( format->has4Views( ) );
    if( m_controller->currentImage( )->tools.empty( ) ) {
      actionDefaultTool->setChecked( true );
    }
    else if( m_controller->currentImage( )->currentTool( )->type( ) == DefaultTool::Type ) {
      actionDefaultTool->setChecked( true );
    }
    else if( m_controller->currentImage( )->currentTool( )->type( ) == SegmentationTool::Type ) {
      actionSegmentationTool->setChecked( true );
    }
/* actionLiveWireTool->setChecked( true ); */
//    actionLiveWireTool_triggered( );
    segmentationWidget->setTool( m_controller->currentImage( )->currentTool( ) );
    labelsWidget->setTool( m_controller->currentImage( )->currentTool( ) );
    if( m_controller->currentImage( ) ) {
      QFileInfo finfo = m_controller->currentImage( )->fileName( );
      setWindowTitle( QString( "%1 - BIAL" ).arg( finfo.fileName( ) ) );
    }
    else {
      setWindowTitle( QString( "BIAL" ) );
    }
  }
}

void MainWindow::imageUpdated( ) {
  const Bial::Signal &hist = m_controller->currentImage( )->getHistogram( );
  QCustomPlot *plot = histogramWidget;
  QVector< double > x( hist.size( ) ), y( hist.size( ) );
  for( size_t bin = 0; bin < hist.size( ); ++bin ) {
    x[ bin ] = hist.Data( bin );
    y[ bin ] = hist[ bin ];
  }
  plot->addGraph( );
  plot->setInteraction( QCP::iRangeDrag, true );
  plot->setInteraction( QCP::iRangeZoom, true );
  plot->graph( 0 )->clearData( );
  plot->graph( 0 )->setData( x, y );
  plot->axisRect( 0 )->setRangeDrag( Qt::Vertical );
  plot->axisRect( 0 )->setRangeZoom( Qt::Vertical );
  plot->axisRect( 0 )->setRangeZoomAxes( plot->xAxis, plot->yAxis );
  plot->graph( 0 )->setLineStyle( QCPGraph::lsImpulse );
  plot->xAxis->setLabel( "Intensity" );
  plot->yAxis->setLabel( "Frequency" );
  plot->rescaleAxes( true );
  plot->replot( );
  if( m_controller->currentImage( ) ) {
    segmentationWidget->setTool( m_controller->currentImage( )->currentTool( ) );
    labelsWidget->setTool( m_controller->currentImage( )->currentTool( ) );
  }
}

void MainWindow::containerUpdated( ) {
  COMMENT( "MainWindow::containerUpdated( )", 0 );
  if( m_controller->size( ) <= 1 ) {
    thumbsDock->hide( );
  }
  else {
    thumbsDock->show( );
  }
  bool hasImage = ( m_controller->currentImage( ) != nullptr );
  ui->toolBar->setVisible( hasImage );
  COMMENT( "Has Image = " << hasImage, 0 );
  ui->menuWindow->setEnabled( hasImage );
  controlsDock->setVisible( hasImage );
  ui->logoView->setVisible( !hasImage );
  ui->imageViewer->setVisible( hasImage );
  ui->menuLayout->setEnabled( hasImage );
  ui->menuOverlay->setEnabled( hasImage );
  ui->actionRemove_current_image->setEnabled( hasImage );
  ui->actionAddLabel->setEnabled( hasImage );
  if( !hasImage ) {
    ui->actionRemove_current_label->setEnabled( false );
    histogramDock->hide( );
  }
  ui->actionShow_controls_dock->setEnabled( hasImage );
  ui->actionShow_images_dock->setEnabled( hasImage );
  currentImageChanged( );
}

void MainWindow::on_actionOpen_image_triggered( ) {
  QString fileName = getFileDialog( );
  if( fileName.isEmpty( ) ) {
    return;
  }
  if( !loadFile( fileName ) ) {
    QMessageBox::warning( this, "Warning", tr( "Could not open file!" ) );
    return;
  }
}

QString MainWindow::getFileDialog( ) {
  return( QFileDialog::getOpenFileName(
            this, tr( "Open" ), defaultFolder,
            tr( "All images (*.pbm *.pbm.gz *.pgm *.pgm.gz *.ppm *.ppm.gz *.pnm *.pnm.gz *.dcm *.dcm.gz *.nii *.nii.gz "
                "*.scn *.scn.gz *.bmat *.bmat.gz *.jpeg *.jpg *.png);; PBM images (*.pbm *.pbm.gz);;"
                "PGM images (*.pgm *.pgm.gz);; PPM images (*.ppm *.ppm.gz);;"
                "PNM images (*.pnm *.pnm.gz);; DICOM images (*.dcm *.dcm.gz);;"
                "NIfTI images (*.nii *.nii.gz);;"
                "SCN Files (*.scn *.scn.gz);;"
                "BMAT images (*.bmat *.bmat.gz);;"
                "JPG images (*.jpg *.jpeg);;"
                "PNG images (*.png);;"
                "All files (*)" ) ) );
}

bool MainWindow::loadFile( QString filename ) {
  COMMENT( "Loading file: " << filename.toStdString( ), 0 );
  m_controller->clear( );
  return( m_controller->addImage( filename ) );
}

bool MainWindow::loadFolder( QString dirname ) {
  QDir folder( dirname );
  COMMENT( "Reding folder: " << folder.absolutePath( ).toStdString( ) << ".", 1 );
  QFileInfoList list = folder.entryInfoList( QDir::NoDotAndDotDot | QDir::Files, QDir::DirsFirst | QDir::Name );
  bool value = false;
  /*  qDebug() << "list size: " << list.size(); */
  CursorChanger c( Qt::WaitCursor );

  QProgressDialog progress( "Reading files...", "Abort", 0, list.size( ), this );
  progress.setWindowModality( Qt::WindowModal );
  int size = list.size( );
  for( int i = 0; i < size; ++i ) {
    progress.setValue( i );
    if( progress.wasCanceled( ) ) {
      break;
    }
    QFileInfo fileInfo = list.at( i );
    if( fileInfo.isFile( ) and checkExtension( fileInfo ) ) {
      QString fileName = fileInfo.absoluteFilePath( );
      if( m_controller->addImage( fileName ) ) {
        value = true;
      }
      else {
        BIAL_WARNING( std::string( "Could not open file!" ) );
        statusBar( )->showMessage( tr( "Could not open file!" ), 2000 );
        continue;
      }
    }
    qApp->processEvents( );
  }
  progress.setValue( list.size( ) );

  return( value );
}

bool MainWindow::checkExtension( const QFileInfo &fileInfo ) { /* receive to lower */
  QStringList list;
  list << "scn" << "scn.gz" << "img" << "img.gz" << "hdr"
       << "hdr.gz" << "nii" << "nii.gz" << "pnm" << "pnm.gz"
       << "pgm" << "pgm.gz" << "pbm" << "pbm.gz" << "dcm"
       << "dcm.gz" << "pnm" << "pnm.gz" << "bmat" << "bmat.gz";

  QString suffix = fileInfo.completeSuffix( ).toLower( );
  if( list.contains( suffix ) ) {
    return( true );
  }
  else {
    QImageReader reader( fileInfo.absoluteFilePath( ) );
    return( !reader.format( ).isEmpty( ) );
  }
}

void MainWindow::readSettings( ) {
  COMMENT( "Reading QSettings", 1 );
  QSettings settings;

  settings.beginGroup( "MainWindow" );
  defaultFolder = settings.value( "defaultFolder" ).toString( );
  if( defaultFolder.isEmpty( ) ) {
    defaultFolder = QDir::homePath( );
  }
}

void MainWindow::commandLineOpen( const QCommandLineParser &parser,
                                  const QCommandLineOption &dicomdir,
                                  const QCommandLineOption &folder,
                                  const QCommandLineOption &label,
                                  const QCommandLineOption &liveWire ) {
  COMMENT( "Command Line Open", 0 );
  const QStringList args = parser.positionalArguments( );
  QString errorMsg;
  if( !args.isEmpty( ) ) {
    for( QString arg : args ) {
      QFileInfo file( arg );
      if( file.exists( ) ) {
        if( file.isFile( ) ) {
          if( m_controller->addImage( file.absoluteFilePath( ) ) ) {
            ui->statusBar->showMessage( file.baseName( ) + " loaded succesfully.", 2000 );
          }
          else {
            errorMsg = "Error loading " + file.absolutePath( ) + ".";
            BIAL_WARNING( errorMsg.toStdString( ) );
          }
        }
        else {
          errorMsg = file.absoluteFilePath( ) + " is not a file.";
          BIAL_WARNING( errorMsg.toStdString( ) );
        }
      }
      else {
        errorMsg = file.absoluteFilePath( ) + " does not exist.";
        BIAL_WARNING( errorMsg.toStdString( ) );
      }
      if( m_controller->size( ) > 1 ) {
        thumbsDock->show( );
      }
    }
    if( parser.isSet( label ) ) {
      QFileInfo file( parser.value( label ) );
      if( file.exists( ) ) {
        if( file.isFile( ) ) {
          if( loadLabel( file.absoluteFilePath( ) ) ) {
            ui->statusBar->showMessage( file.baseName( ) + " loaded succesfully.", 2000 );
          }
          else {
            errorMsg = "Error loading " + file.absoluteFilePath( ) + ".";
            BIAL_WARNING( errorMsg.toStdString( ) );
          }
        }
        else {
          errorMsg = file.absoluteFilePath( ) + " is not a file.";
          BIAL_WARNING( errorMsg.toStdString( ) );
        }
      }
      else {
        errorMsg = file.absoluteFilePath( ) + " does not exist.";
        BIAL_WARNING( errorMsg.toStdString( ) );
      }
    }
  }
  else if( parser.isSet( dicomdir ) ) {
    QFileInfo file( parser.value( dicomdir ) );
    if( file.exists( ) ) {
      if( loadDicomdir( file.absoluteFilePath( ) ) ) {
        ui->statusBar->showMessage( file.baseName( ) + " loaded succesfully.", 2000 );
      }
      else {
        errorMsg = "Error loading " + file.absoluteFilePath( ) + ".";
        BIAL_WARNING( errorMsg.toStdString( ) );
      }
    }
    else {
      errorMsg = file.absoluteFilePath( ) + " does not exist.";
      BIAL_WARNING( errorMsg.toStdString( ) );
    }
  }
  else if( parser.isSet( folder ) ) {
    QFileInfo file( parser.value( folder ) );
    if( file.exists( ) ) {
      if( file.isDir( ) ) {
        if( loadFolder( file.absoluteFilePath( ) ) ) {
          ui->statusBar->showMessage( file.baseName( ) + " loaded succesfully.", 2000 );
        }
        else {
          errorMsg = "Error loading " + file.absoluteFilePath( ) + ".";
          BIAL_WARNING( errorMsg.toStdString( ) );
        }
      }
      else {
        errorMsg = file.absoluteFilePath( ) + " is not a directory.";
        BIAL_WARNING( errorMsg.toStdString( ) );
      }
    }
    else {
      errorMsg = file.absoluteFilePath( ) + " does not exist.";
      BIAL_WARNING( errorMsg.toStdString( ) );
    }
  }
  if( !errorMsg.isEmpty( ) ) {
    QMessageBox::warning( this, "Warning", errorMsg );
    ui->statusBar->showMessage( errorMsg, 5000 );
  }
  if( parser.isSet( liveWire ) ) {
    setTool( ActiveContourTool::Type );
  }
}

void MainWindow::on_actionQuit_triggered( ) {
  close( );
}

void MainWindow::openRecentFile( ) {
  QAction *action = qobject_cast< QAction* >( sender( ) );
  if( action ) {
    QString fileName = action->data( ).toString( );
    loadFile( fileName );
  }
}

void MainWindow::updateRecentFileActions( ) {
  QSettings settings;
  QStringList files = settings.value( "recentFileList" ).toStringList( );

  int numRecentFiles = qMin( files.size( ), ( int ) Controller::MaxRecentFiles );
  if( numRecentFiles > 0 ) {
    ui->menuRecent_files->setEnabled( true );
  }
  for( int i = 0; i < numRecentFiles; ++i ) {
    QString text = QString( "&%1 %2" ).arg( i + 1 ).arg( QFileInfo( files[ i ] ).fileName( ) );
    recentFileActs[ i ]->setText( text );
    recentFileActs[ i ]->setData( files[ i ] );
    recentFileActs[ i ]->setVisible( true );
  }
  for( int i = numRecentFiles; i < Controller::MaxRecentFiles; ++i ) {
    recentFileActs[ i ]->setVisible( false );
  }
}

void MainWindow::createActions( ) {
  for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
    recentFileActs[ i ] = new QAction( this );
    recentFileActs[ i ]->setVisible( false );
    connect( recentFileActs[ i ], &QAction::triggered, this, &MainWindow::openRecentFile );
    ui->menuRecent_files->addAction( recentFileActs[ i ] );
  }
  updateRecentFileActions( );
  for( int i = 0; i < Controller::MaxRecentFiles; ++i ) {
    ui->menuRecent_files->addAction( recentFileActs[ i ] );
  }
}

void MainWindow::loadQss( ) {
  QFile file( ":/qss/stylesheet.qss" );
  file.open( QFile::ReadOnly );
  QString StyleSheet = QLatin1String( file.readAll( ) );
  setStyleSheet( StyleSheet );
}

bool MainWindow::loadDicomdir( QString dicomFName ) {
  COMMENT( "Loading DicomDir file", 1 );
  CursorChanger c( Qt::WaitCursor );
  DicomDir dicomdir;
  if( !dicomdir.open( dicomFName ) ) {
    statusBar( )->showMessage( tr( "Could not open dicomdir" ), 2000 );
    return( false );
  }
  const QStringList files = dicomdir.getImages( );
  if( files.size( ) > 0 ) {
    m_controller->clear( );
    QProgressDialog progress( tr( "Reading dicomdir files..." ), tr( "Abort" ), 0, files.size( ), this );
    progress.setWindowModality( Qt::WindowModal );
    for( int i = 0, size = files.size( ); i < size; ++i ) {
      progress.setValue( i );
      if( progress.wasCanceled( ) ) {
        break;
      }
      m_controller->addImage( files[ i ].trimmed( ) );
    }
    progress.setValue( files.size( ) );
    if( m_controller->size( ) < 1 ) {
      statusBar( )->showMessage( tr( "Could not load any dicomdir images" ), 2000 );
      return( false );
    }
    return( true );
  }
  statusBar( )->showMessage( tr( "Empty dicomdir!" ), 2000 );
  BIAL_WARNING( "Empty dicomdir!" );

  return( false );
}

bool MainWindow::loadLabel( QString filename ) {
  actionDefaultTool_triggered( );
  DefaultTool *tool = dynamic_cast< DefaultTool* >( m_controller->currentImage( )->currentTool( ) );
  if( tool ) {
    bool status = tool->addLabel( filename );
    labelsDock->show( );
    return( status );
  }
  return( false );
}

void MainWindow::on_actionAddLabel_triggered( ) {
  QString filename = getFileDialog( );
  if( !loadLabel( filename ) ) {
    QMessageBox::warning( this, "Warning", tr( "Could not open label!" ) );
  }
}

void MainWindow::on_actionOpen_folder_triggered( ) {
  QString folderString = QFileDialog::getExistingDirectory( this, tr( "Open folder" ), defaultFolder );
  COMMENT( "Opening folder: \"" << folderString.toStdString( ) << "\"", 1 )
  if( !folderString.isEmpty( ) ) {
    m_controller->clear( );
    if( !loadFolder( folderString ) ) {
      BIAL_WARNING( "Could not read folder!" )
      QMessageBox::warning( this, "Warning", tr( "Could not read folder!" ) );
    }
  }
}

void MainWindow::on_actionOpen_DicomDir_triggered( ) {
  QString fileName = QFileDialog::getOpenFileName( this, tr( "Open" ), defaultFolder, tr( "*" ) );
  loadDicomdir( fileName );
}

void MainWindow::on_actionAdd_image_triggered( ) {
  m_controller->addImage( getFileDialog( ) );
}

void MainWindow::on_actionRemove_current_image_triggered( ) {
  m_controller->removeCurrentImage( );
}

void MainWindow::on_actionSelect_default_folder_triggered( ) {
  QString temp = QFileDialog::getExistingDirectory( this, tr( "Select default folder" ), QDir::homePath( ) );
  if( !temp.isEmpty( ) ) {
    defaultFolder = temp;
    QSettings settings;
    settings.beginGroup( "MainWindow" );
    settings.setValue( "defaultFolder", defaultFolder );
    settings.endGroup( );
  }
}

void MainWindow::on_actionRemove_current_label_triggered( ) {
  m_controller->removeCurrentLabel( );
}

void MainWindow::updateIntensity( QPointF scnPos, Qt::MouseButtons buttons, size_t axis ) {
  Q_UNUSED( buttons )
  GuiImage * img = m_controller->currentImage( );
  if( img != nullptr ) {
    Bial::Point3D pt = img->getPosition( scnPos, axis );
    QString msg;
    size_t dims = img->getDims( );
    switch( img->getImageType( ) ) {
        case Bial::MultiImageType::int_img: {
        int max = img->max( );
        const Bial::Image< int > &bial_img( img->getIntImage( ) );
        if( dims == 3 ) {
          if( bial_img.ValidCoordinate( pt.x, pt.y, pt.z ) ) {
            int color = bial_img( pt.x, pt.y, pt.z );
            msg = QString( "Axis %1 : (%2, %3, %4) = %5/%6" )
                  .arg( axis )
                  .arg( ( int ) pt.x )
                  .arg( ( int ) pt.y )
                  .arg( ( int ) pt.z )
                  .arg( color )
                  .arg( max );
          }
        }
        else {
          if( bial_img.ValidCoordinate( pt.x, pt.y ) ) {
            int color = bial_img( pt.x, pt.y );
            msg = QString( "(%1, %2) = %3/%4" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg( color ).arg( max );
          }
        }
        break;
      }
        case Bial::MultiImageType::flt_img: {
        float max = img->fmax( );
        const Bial::Image< float > &bial_img( img->getFltImage( ) );
        if( dims == 3 ) {
          if( bial_img.ValidCoordinate( pt.x, pt.y, pt.z ) ) {
            int color = static_cast< int >( bial_img( pt.x, pt.y, pt.z ) );
            msg = QString( "Axis %1 : (%2, %3, %4) = %5/%6" )
                  .arg( axis )
                  .arg( ( int ) pt.x )
                  .arg( ( int ) pt.y )
                  .arg( ( int ) pt.z )
                  .arg( color )
                  .arg( max );
          }
        }
        else {
          if( bial_img.ValidCoordinate( pt.x, pt.y ) ) {
            int color = static_cast< int >( bial_img( pt.x, pt.y ) );
            msg = QString( "(%1, %2) = %3/%4" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg( color ).arg( max );
          }
        }
        break;
      }
        case Bial::MultiImageType::clr_img: {
        int max = img->max( );
        const Bial::Image< Bial::Color > &bial_img( img->getClrImage( ) );
        if( dims == 3 ) {
          if( bial_img.ValidCoordinate( pt.x, pt.y, pt.z ) ) {
            int r = static_cast< int >( bial_img( pt.x, pt.y, pt.z )[ 0 ] );
            int g = static_cast< int >( bial_img( pt.x, pt.y, pt.z )[ 1 ] );
            int b = static_cast< int >( bial_img( pt.x, pt.y, pt.z )[ 2 ] );
            msg = QString( "Axis %1 : (%2, %3, %4) = %5/%6" )
                  .arg( axis )
                  .arg( ( int ) pt.x )
                  .arg( ( int ) pt.y )
                  .arg( ( int ) pt.z )
                  .arg( r )
                  .arg( g )
                  .arg( b )
                  .arg( max );
          }
        }
        else {
          if( bial_img.ValidCoordinate( pt.x, pt.y ) ) {
            int r = static_cast< int >( bial_img( pt.x, pt.y )[ 0 ] );
            int g = static_cast< int >( bial_img( pt.x, pt.y )[ 1 ] );
            int b = static_cast< int >( bial_img( pt.x, pt.y )[ 2 ] );
            msg = QString( "(%1, %2) = (%3, %4, %5)/%6" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg( r ).arg( g ).arg(
              b ).arg( max );
          }
        }
        break;
      }
        case Bial::MultiImageType::rcl_img: {
        int max = img->fmax( );
        const Bial::Image< Bial::RealColor > &bial_img( img->getRclImage( ) );
        if( dims == 3 ) {
          if( bial_img.ValidCoordinate( pt.x, pt.y, pt.z ) ) {
            int r = static_cast< int >( bial_img( pt.x, pt.y, pt.z )[ 0 ] );
            int g = static_cast< int >( bial_img( pt.x, pt.y, pt.z )[ 1 ] );
            int b = static_cast< int >( bial_img( pt.x, pt.y, pt.z )[ 2 ] );
            msg = QString( "Axis %1 : (%2, %3, %4) = %5/%6" )
                  .arg( axis )
                  .arg( ( int ) pt.x )
                  .arg( ( int ) pt.y )
                  .arg( ( int ) pt.z )
                  .arg( r )
                  .arg( g )
                  .arg( b )
                  .arg( max );
          }
        }
        else {
          if( bial_img.ValidCoordinate( pt.x, pt.y ) ) {
            int r = static_cast< int >( bial_img( pt.x, pt.y )[ 0 ] );
            int g = static_cast< int >( bial_img( pt.x, pt.y )[ 1 ] );
            int b = static_cast< int >( bial_img( pt.x, pt.y )[ 2 ] );
            msg = QString( "(%1, %2) = (%3, %4, %5)/%6" ).arg( ( int ) pt.x ).arg( ( int ) pt.y ).arg( r ).arg( g ).arg(
              b ).arg( max );
          }
        }
      }
        default:
        std::string msg( BIAL_ERROR( "Accessing non-initialized multi-image." ) );
        throw( std::runtime_error( msg ) );
    }
    ui->statusBar->showMessage( msg, 10000 );
  }
}

void MainWindow::on_actionAxial_triggered( ) {
  m_controller->currentFormat( )->setNumberOfViews( 1 );
  m_controller->currentFormat( )->setCurrentViews( Views::SHOW0 );
}

void MainWindow::on_actionCoronal_triggered( ) {
  m_controller->currentFormat( )->setNumberOfViews( 1 );
  m_controller->currentFormat( )->setCurrentViews( Views::SHOW1 );
}

void MainWindow::on_actionSagittal_triggered( ) {
  m_controller->currentFormat( )->setNumberOfViews( 1 );
  m_controller->currentFormat( )->setCurrentViews( Views::SHOW2 );
}

void MainWindow::on_action3_Views_triggered( ) {
  m_controller->currentFormat( )->setNumberOfViews( 3 );
}

void MainWindow::on_action4_Views_triggered( ) {
  m_controller->currentFormat( )->setNumberOfViews( 4 );
}

void MainWindow::on_actionVertical_triggered( ) {
  m_controller->currentFormat( )->setCurrentLayout( Layout::VERTICAL );
}

void MainWindow::on_actionHorizontal_triggered( ) {
  m_controller->currentFormat( )->setCurrentLayout( Layout::HORIZONTAL );
}

void MainWindow::on_actionGrid_triggered( ) {
  m_controller->currentFormat( )->setCurrentLayout( Layout::GRID );
}

void MainWindow::on_actionWhitePen_triggered( ) {
  m_controller->currentFormat( )->setOverlayColor( Qt::white );
}

void MainWindow::on_actionRedPen_triggered( ) {
  m_controller->currentFormat( )->setOverlayColor( Qt::red );
}

void MainWindow::on_actionBluePen_triggered( ) {
  m_controller->currentFormat( )->setOverlayColor( Qt::blue );
}

void MainWindow::on_actionGreenPen_triggered( ) {
  m_controller->currentFormat( )->setOverlayColor( Qt::green );
}

void MainWindow::on_actionBlackPen_triggered( ) {
  m_controller->currentFormat( )->setOverlayColor( Qt::black );
}

void MainWindow::on_actionToggle_overlay_triggered( ) {
  if( m_controller->currentFormat( ) ) {
    m_controller->currentFormat( )->toggleOverlay( );
  }
}

void MainWindow::setTool( int toolType ) {
  segmentationDock->hide( );
  livewireDock->hide( );
  switch( toolType ) {
      case DefaultTool::Type:
      actionDefaultTool->setChecked( true );
      break;
      case ActiveContourTool::Type:
      actionLiveWireTool->setChecked( true );
      livewireDock->show( );
      livewireDock->raise( );
      break;
      case SegmentationTool::Type:
      actionSegmentationTool->setChecked( true );
      segmentationDock->show( );
      segmentationDock->raise( );
      break;
  }
  GuiImage *img = m_controller->currentImage( );
  if( img ) {
    Tool::setImageTool( toolType, img, this->ui->imageViewer );
    segmentationWidget->setTool( img->currentTool( ) );
    livewireWidget->setTool( img->currentTool( ) );
    labelsWidget->setTool( img->currentTool( ) );
    emit img->imageUpdated( );
  }
}

void MainWindow::actionDefaultTool_triggered( ) {
  setTool( DefaultTool::Type );
}

void MainWindow::actionSegmentationTool_triggered( ) {
  setTool( SegmentationTool::Type );
}

void MainWindow::actionLiveWireTool_triggered( ) {
  setTool( ActiveContourTool::Type );
}

void MainWindow::on_actionEnglish_triggered( ) {
  QTranslator translator;
  if( not translator.load( ":/translations/bial_en.qm" ) ) {
    QMessageBox::critical( this, "Erro", "Erro carregando tradução!" );
    return;
  }
  qApp->installTranslator( &translator );
  ui->retranslateUi( this );
}

void MainWindow::on_actionPortuguese_triggered( ) {
  QTranslator translator;
  if( not translator.load( ":/translations/bial_br.qm" ) ) {
    QMessageBox::critical( this, "Erro", "Erro carregando tradução!" );
    return;
  }
  qApp->installTranslator( &translator );
  ui->retranslateUi( this );
}

void MainWindow::actionFunctional_Tool_triggered( ) {
  /*    ui->dockWidgetFunctional->show(); */
/*  ui->widgetDragDrop->show( ); */
  ui->logoView->hide( );
}

/*
 * TODO: para a funcao drag and drop colocar uma arvore de navegacao de pasta a esquerda para facilitar, nao precisar
 * abrir uma janela do sistema para isso
 * TODO: arrumar itens na scene nao sendo carregadas no tamanho correto as vezes (precisa clicar novamente ou clicar em
 * redimensionar)
 */

void MainWindow::actionChange_default_parameters_triggered( ) {
  /* TODO: abrir janela para mostrar parametros */
}

void MainWindow::on_actionNext_triggered( ) {
  m_controller->loadNextImage( );
}

void MainWindow::on_actionPrevious_triggered( ) {
  m_controller->loadPreviousImage( );
}
