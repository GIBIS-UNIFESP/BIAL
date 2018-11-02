#include <AdjacencyRound.hpp>
#include <BrainInhomogeneity.hpp>
#include <BrainSkullStripping.hpp>
#include <BrainTissueSegmentation.hpp>
#include <ColorMedianFeature.hpp>
#include <ConnPathFunction.hpp>
#include <DiffusionFunction.hpp>
#include <DrawBox.hpp>
#include <Feature.hpp>
#include <FileImage.hpp>
#include <FilteringAnisotropicDiffusion.hpp>
#include <FilteringGaussian.hpp>
#include <FilteringMean.hpp>
#include <FilteringMedian.hpp>
#include <FilteringOptimalAnisotropicDiffusion.hpp>
#include <FuzzyCMeans.hpp>
#include <GradientCanny.hpp>
#include <GradientGabor.hpp>
#include <GradientMorphological.hpp>
#include <GradientPosOperations.hpp>
#include <GradientScaleCanny.hpp>
#include <GradientSobel.hpp>
#include <HeartCOG.hpp>
#include <HeartSegmentation.hpp>
#include <Image.hpp>
#include <ImageIFT.hpp>
#include <ImageMerge.hpp>
#include <ImageSplit.hpp>
#include <MRIModality.hpp>
#include <MedianFeature.hpp>
#include <MedianFeature.hpp>
#include <MorphologyDilation.hpp>
#include <MorphologyErosion.hpp>
#include <NiftiHeader.hpp>
#include <PathFunction.hpp>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTimer>
#include <SegmentationBorder.hpp>
#include <SegmentationMST.hpp>
#include <SegmentationWatershed.hpp>
#include <SpatialFeature.hpp>
#include <Table.hpp>
#include <TransformEuclDist.hpp>

#include "dragdrop.h"
#include "guiimage.h"
#include "label.h"
#include "ui_dragdrop.h"

using namespace std;
using namespace Bial;

DragDrop::DragDrop( QWidget *parent ) : QWidget( parent ), ui( new Ui::DragDrop ) {
  ui->setupUi( this );

  ui->graphicsViewBefore->setScene( new QGraphicsScene( this ) );
  ui->graphicsViewAfter->setScene( new QGraphicsScene( this ) );
  ui->graphicsViewInput->setScene( new QGraphicsScene( this ) );
  ui->graphicsViewOutput->setScene( new QGraphicsScene( this ) );

  ui->graphicsViewBefore->setFit( true );
  ui->graphicsViewAfter->setFit( true );

  model = new QFileSystemModel( this );
  model->setRootPath( "" );
  ui->treeView->setModel( model );
  ui->treeView->setAnimated( false );
  ui->treeView->setIndentation( 20 );
  ui->treeView->setSortingEnabled( true );
  const QSize availableSize = QApplication::desktop( )->availableGeometry( ui->treeView ).size( );
  ui->treeView->resize( availableSize / 2 );
  ui->treeView->setColumnWidth( 0, ui->treeView->width( ) / 3 );
  ui->treeView->setWindowTitle( "Pastas" );
  ui->treeView->sortByColumn( 0, Qt::AscendingOrder );

  /*
   *  ui->scrollAreaLeft->hide();
   *  ui->scrollAreaRight->hide();
   */

  /*
   *  QList<int> list;
   *  list << 40 << 400;
   *  ui->splitter->setSizes(list);
   */

  connect( ui->pushButtonBW, &PushButton::item, this, &DragDrop::processBW );
  connect( ui->pushButtonInvert, &PushButton::item, this, &DragDrop::processInvert );

  connect( ui->pushButtonBatchBW, &PushButton::file, this, &DragDrop::processBatchBW );

  connect( ui->pushButtonInputFolder, &PushButton::file, this, &DragDrop::setInputFolder );
  connect( ui->pushButtonOutputFolder, &PushButton::file, this, &DragDrop::setOutputFolder );

  connect( ui->graphicsViewOutput, &GraphicsView::saveFile, this, &DragDrop::saveImage );

  /*  connect(ui->scrollAreaRight, &ScrollArea::item, this, &DragDrop::saveImage); */

  ui->pushButtonBatchBW->hide( );
  ui->pushButtonBatchInvert->hide( );
  ui->pushButtonBW->hide( );
  ui->pushButtonInvert->hide( );
}

DragDrop::~DragDrop( ) {
  delete ui;
}

void DragDrop::processBW( const QString &text ) {
  qDebug( ) << "text: " << text;
  if( text == "item" ) {
    on_pushButtonBW_clicked( );
  }
  if( text == "item2" ) {
    showImage( item2->pixmap( ), item2->fileInfo( ) );
    on_pushButtonBW_clicked( );
  }
}

void DragDrop::processInvert( const QString &text ) {
  qDebug( ) << "text: " << text;
  if( text == "item" ) {
    on_pushButtonInvert_clicked( );
  }
  if( text == "item2" ) {
    showImage( item2->pixmap( ), item2->fileInfo( ) );
    on_pushButtonInvert_clicked( );
  }
}

void DragDrop::processBatchBW( const QString &folder ) {
  QFileInfo info( folder );
  if( not info.isDir( ) ) {
    QMessageBox::critical( this, "Erro!", "Não é pasta!" );
    return;
  }
  outputFolder = folder;

  setOutputFolder( folder );

  on_pushButtonBatchBW_clicked( );
}

void DragDrop::setInputFolder( const QString &path ) {
  inputFolder = path;

  ui->groupBoxEntrada->setTitle( "Pasta Entrada: " + path );

  loadInputFolderThumbs( );
}

void DragDrop::setOutputFolder( const QString &path ) {
  outputFolder = path;

  ui->groupBoxSaida->setTitle( "Pasta Saída: " + path );

  loadOutputFolderThumbs( );
  /*  loadFolderThumbs(path, ui->scrollAreaRightWidgetContents); */
}

void DragDrop::saveImage( const QString &text ) { /* TODO: convert text to enum? */
  if( outputFolder.isEmpty( ) ) {
    QMessageBox::critical( this, "Erro!", "Não tem pasta de saída!" );
    return;
  }
  QString filePath = text == "item" ? item->fileInfo( ).absoluteFilePath( ) : item2->fileInfo( ).absoluteFilePath( );

  /* TODO: se nome já existir perguntar se sobrescrever ou renomear */

  Bial::Write( text == "item" ? m_img->getClrImage( ) : m_img2->getClrImage( ), filePath.toStdString( ) );

  loadOutputFolderThumbs( );
}

void DragDrop::on_pushButtonBW_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  QImage temp = item->pixmap( ).toImage( );
  temp = temp.convertToFormat( QImage::Format_Grayscale8 );
  QPixmap pix( QPixmap::fromImage( temp ) );

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::showImage( const QPixmap &pix, const QFileInfo &fileInfo ) {
  if( item ) {
    ui->graphicsViewBefore->scene( )->removeItem( item );
    delete item;
  }
  if( m_img ) {
    delete m_img;
  }
  item = new GraphicsItem( "item", pix, fileInfo );

  ui->graphicsViewBefore->scene( )->setSceneRect( item->pixmap( ).rect( ) );

  ui->graphicsViewBefore->scene( )->addItem( item );
  ui->graphicsViewBefore->fitInView( ui->graphicsViewBefore->sceneRect( ), Qt::KeepAspectRatio );

  /* ------------------------------------------------------ */

  /* TODO: verify extension before constructing GuiImage (create a static GuiImage function to verify?) */
  m_img = new GuiImage( fileInfo.absoluteFilePath( ), this );
  /*
   *  if (img->getSlice(0).isNull()) return;
   *  qDebug() << "bial size: " << img->getSlice(0).size();
   *  item2 = new GraphicsItem("item2", img->getSlice(0), fileName);
   *  ui->graphicsViewAfter->scene()->setSceneRect(item2->pixmap().rect());
   *  ui->graphicsViewAfter->fitInView(ui->graphicsViewAfter->sceneRect(), Qt::KeepAspectRatio);
   */
}

void DragDrop::showImage2( const QPixmap &pix, const QFileInfo &fileInfo ) {
  if( item2 ) {
    ui->graphicsViewAfter->scene( )->removeItem( item2 );
    delete item2;
  }
  item2 = new GraphicsItem( "item2", pix, fileInfo );

  ui->graphicsViewAfter->scene( )->setSceneRect( item2->pixmap( ).rect( ) );

  ui->graphicsViewAfter->scene( )->addItem( item2 );
  ui->graphicsViewAfter->fitInView( ui->graphicsViewAfter->sceneRect( ), Qt::KeepAspectRatio );
}

Image< Color > DragDrop::rotateBial( ) {
  size_t x_size = m_img->getClrImage( ).size( 0 );
  size_t y_size = m_img->getClrImage( ).size( 1 );
  Image< Color > res( y_size, x_size );
  for( size_t y = 0; y < y_size; ++y ) {
    for( size_t x = 0; x < x_size; ++x ) {
      res( y, x_size - 1 - x ) = m_img->getClrImage( )( x, y );
    }
  }
  return( res );
}

Image< int > DragDrop::AnisotropicBial( ) {
  Image< int > src = m_img->getIntImage( );

  size_t iterations = QInputDialog::getInt( this, "Iterations", "How many?", 3 ); /* 30 */
  float kappa = QInputDialog::getInt( this, "Kappa", "How much?", 10 ); /* 10 */
  float radius = QInputDialog::getInt( this, "Radius", "How much?", 1.01 ); /* 1.01 */
  DiffusionFunction *diff_func = new GaussianDiffusionFunction;

  Image< int > res( Filtering::AnisotropicDiffusion( src, diff_func, kappa, iterations, radius ) );

  return( res );
}

Image< int > DragDrop::CannyBial( ) {
  Image< int > img = m_img->getIntImage( );
  float low_thres = QInputDialog::getDouble( this, "Low threshold", "Low: ", 0.7 );
  float hig_thres = QInputDialog::getDouble( this, "High threshold", "High: ", 0.9 );
  float sigma = QInputDialog::getDouble( this, "Sigma", "Sigma: ", 1.0 );

  Image< int > res( Gradient::Canny( img, low_thres, hig_thres, sigma ) );

  return( res );
}

void DragDrop::on_pushButtonInvert_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< Color > res = rotateBial( );

  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) );

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::loadInputFolderThumbs( ) {
  ui->graphicsViewInput->scene( )->clear( );
  int x = 0;
  int y = 0;

  QFileInfoList list = QDir( inputFolder ).entryInfoList( QDir::NoDotAndDotDot | QDir::AllEntries );

  QProgressDialog progress( "Reading files...", "Abort", 0, list.size( ), this );
  progress.setWindowModality( Qt::WindowModal );
  for( auto file : list ) {

    qDebug( ) << "file: " << file.fileName( );
/*
 *    if( not GuiImage::isSupported( file.absoluteFilePath( ) ) ) {
 *      continue;
 *    }
 */
    GuiImage *img = new GuiImage( file.absoluteFilePath( ), this );
    if( img->getSlice( 0 ).isNull( ) ) {
      continue;
    }
    QPixmap *pix = new QPixmap( img->getSlice( 0 ) );

    delete img;

    /*
     *    QPixmap *pix = new QPixmap(file.absoluteFilePath());
     *    if (pix->isNull()) continue;
     */
    QPixmap *pixScaled = new QPixmap( pix->scaled( 120, ui->graphicsViewInput->viewport( )->height( ) - 30,
                                                   Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation ) );

    GraphicsItem *item = new GraphicsItem( "item", *pixScaled, file );
    item->setPix2( *pix );
    item->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable );
    item->setPos( x, y );

    connect( item, &GraphicsItem::Image, this, &DragDrop::showImage );

    x += pixScaled->width( ) + 20;

    ui->graphicsViewInput->scene( )->addItem( item );

    qApp->processEvents( );
  }
}

void DragDrop::loadOutputFolderThumbs( ) {
  ui->graphicsViewOutput->scene( )->clear( );
  int x = 0;
  int y = 0;

  QFileInfoList list = QDir( outputFolder ).entryInfoList( QDir::NoDotAndDotDot | QDir::AllEntries );

  QProgressDialog progress( "Reading files...", "Abort", 0, list.size( ), this );
  progress.setWindowModality( Qt::WindowModal );
  for( auto file : list ) {

    qDebug( ) << "file: " << file.fileName( );
/*
 *    if( not GuiImage::isSupported( file.absoluteFilePath( ) ) ) {
 *      continue;
 *    }
 */
    GuiImage *img = new GuiImage( file.absoluteFilePath( ), this );
    if( img->getSlice( 0 ).isNull( ) ) {
      continue;
    }
    QPixmap *pix = new QPixmap( img->getSlice( 0 ) );

    delete img;

    /*    QPixmap *pix = new QPixmap(file.absoluteFilePath()); */
    QPixmap *pixScaled = new QPixmap( pix->scaled( 120, ui->graphicsViewOutput->viewport( )->height( ) - 30,
                                                   Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation ) );

    GraphicsItem *item = new GraphicsItem( "item", *pixScaled, file.fileName( ) );
    item->setPix2( *pix );
    item->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable );
    item->setPos( x, y );

    connect( item, &GraphicsItem::Image, this, &DragDrop::showImage );

    x += pixScaled->width( ) + 20;

    ui->graphicsViewOutput->scene( )->addItem( item );
  }
}

void DragDrop::loadFolderThumbs( QString folder, QWidget *widget ) {
  /* TODO: separate into two functions:  loadInputFolder/loadOutputFolder */
  qDebug( ) << "name: " << widget->objectName( );
  if( widget->layout( ) != nullptr ) {
    QLayoutItem *item;
    while( ( item = widget->layout( )->takeAt( 0 ) ) != nullptr ) {
      delete item->widget( );
      delete item;
    }
  }
  /*  */
  if( widget->objectName( ) == "scrollAreaLeftWidgetContents" ) {
    ui->graphicsViewInput->scene( )->clear( );
    int x = 0;
    int y = 0;
    /*  */

    QFileInfoList list = QDir( folder ).entryInfoList( QDir::NoDotAndDotDot | QDir::AllEntries );

    QProgressDialog progress( "Reading files...", "Abort", 0, list.size( ), this );
    progress.setWindowModality( Qt::WindowModal );
    for( auto file : list ) {

      QPixmap *pix = new QPixmap( file.absoluteFilePath( ) );
      /*    QPixmap *pixScaled = new QPixmap(pix->scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation)); */
      QPixmap *pixScaled = new QPixmap( pix->scaled( 120, ui->graphicsViewInput->viewport( )->height( ) - 30,
                                                     Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation ) );

      GraphicsItem *item = new GraphicsItem( "item", *pixScaled, file.fileName( ) );
      item->setPix2( *pix );
      item->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable );
      item->setPos( x, y );

      connect( item, &GraphicsItem::Image, this, &DragDrop::showImage );

      x += pixScaled->width( ) + 20;

      ui->graphicsViewInput->scene( )->addItem( item );
    }
  }
  /*  */

  QFileInfoList list = QDir( folder ).entryInfoList( QDir::NoDotAndDotDot | QDir::AllEntries );

  QProgressDialog progress( "Reading files...", "Abort", 0, list.size( ), this );
  progress.setWindowModality( Qt::WindowModal );
  for( auto file : list ) {
    if( progress.wasCanceled( ) ) {
      break;
    }
    progress.setValue( progress.value( ) + 1 );
    if( file.isFile( ) ) {
      createLabel( file, widget );
    }
  }
  progress.cancel( );

  widget->layout( )->addItem( new QSpacerItem( 40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding ) );
}

void DragDrop::on_pushButtonBatchBW_clicked( ) {
  if( outputFolder.isEmpty( ) ) {
    QMessageBox::critical( this, "Erro!", "Não há pasta de saída definida!" );
    return;
  }
  auto list = ui->graphicsViewInput->scene( )->items( Qt::AscendingOrder );
  for( auto object : list ) {
    GraphicsItem *item = static_cast< GraphicsItem* >( object );
    if( item ) {
      showImage( item->getPix2( ), item->fileInfo( ) );
      on_pushButtonBW_clicked( );
      saveImage( "item2" );

      QTimer timer;
      timer.setSingleShot( true );
      timer.setInterval( 1000 );
      timer.start( );
      while( timer.remainingTime( ) > 0 ) {
        /* wait */
      }
      qApp->processEvents( );
    }
  }
}

void DragDrop::createLabel( const QFileInfo &file, const QWidget *widget ) {
  Label *label = new Label( file.absoluteFilePath( ) );
  QPixmap *pixmap = new QPixmap( file.absoluteFilePath( ) );
  if( pixmap->isNull( ) ) {
    delete label;
    delete pixmap;
    return;
  }
  *pixmap = pixmap->scaled( 100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation );
  label->setPixmap( *pixmap );
  widget->layout( )->addWidget( label );

  connect( label, &Label::Image, this, &DragDrop::showImage );
}

/*
 * void DragDrop::on_actionAbrir_imagem_triggered() {
 *  QString path = QFileDialog::getOpenFileName(this, "Abrir imagem", QDir::currentPath(), "Imagens (*.jpg) ");
 */

/*
 *  if (path.isNull()) return;
 * }
 */

void DragDrop::on_pushButtonBatchInvert_clicked( ) {
  /*  auto list = ui->scrollAreaLeftWidgetContents->children(); */
  /*
   *  for (auto object : list) {
   *    Label *label = qobject_cast<Label *>(object);
   */
  /*
   *    if (label) {
   *      QFileInfo info(label->filePath);
   */
  /*
   *      showImage(label->filePath, info.fileName());
   *      on_pushButtonInvert_clicked();
   *    }
   *    QTimer timer;
   *    timer.setSingleShot(true);
   *    timer.setInterval(1000);
   *    timer.start();
   *    while (timer.remainingTime() > 0) {
   *      // wait
   *    }
   *    qApp->processEvents();
   *  }
   *  loadFolderThumbs(outputFolder, ui->scrollAreaRightWidgetContents);
   */
  if( outputFolder.isEmpty( ) ) {
    QMessageBox::critical( this, "Erro!", "Não há pasta de saída definida!" );
    return;
  }
  auto list = ui->graphicsViewInput->scene( )->items( Qt::AscendingOrder );
  for( auto object : list ) {
    GraphicsItem *item = static_cast< GraphicsItem* >( object );
    if( item ) {
      showImage( item->getPix2( ), item->fileInfo( ) );
      on_pushButtonInvert_clicked( );
      saveImage( "item2" );

      QTimer timer;
      timer.setSingleShot( true );
      timer.setInterval( 1000 );
      timer.start( );
      while( timer.remainingTime( ) > 0 ) {
        /* wait */
      }
      qApp->processEvents( );
    }
  }
}

void DragDrop::on_pushButtonInputFolder_clicked( ) {
  QString path = QFileDialog::getExistingDirectory( this, "Pasta de entrada", QDir::currentPath( ) );
  if( path.isNull( ) ) {
    return;
  }
  setInputFolder( path );
}

void DragDrop::on_pushButtonOutputFolder_clicked( ) {
  outputFolder = QFileDialog::getExistingDirectory( this, "Pasta de saída", QDir::currentPath( ) );
  if( outputFolder.isNull( ) ) {
    return;
  }
  setOutputFolder( outputFolder );
}

void DragDrop::on_treeView_doubleClicked( const QModelIndex &index ) {
  Q_UNUSED( index )
  /*  QString path = model->filePath(index); */

  /*  if (path.isNull()) return; */

  /*  setInputFolder(path); */

  /*  loadFolderThumbs(path, ui->scrollAreaLeftWidgetContents); */
}

void DragDrop::on_treeView_clicked( const QModelIndex &index ) {
  QString path = model->filePath( index );
  if( path.isNull( ) ) {
    return;
  }
  setInputFolder( path );
}

/*
 * -----------------------------------------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------
 */

Bial::Image< int > DragDrop::brain_cmeansclustering( ) {
  COMMENT( "Reading input data.", 0 );
  Image< int > scn = m_img->getIntImage( );
  QString file = QFileDialog::getOpenFileName( this );
  Image< int > mask( Read< int >( file.toStdString( ) ) );

  float m = QInputDialog::getDouble( this, "m", "m: ", 2.0, 0 ); /* 2.0 */
  float epsilon = QInputDialog::getDouble( this, "epsilon", "epsilon: ", 0.1, 0 ); /* 0.1 */
  size_t clusters = QInputDialog::getInt( this, "clusters", "clusters: ", 3, 0 ); /* 3 */

  COMMENT( "Running cmeans.", 0 );
  Feature< int > feats = MedianFeature< int >( scn, mask, AdjacencyType::HyperSpheric( 1.5, scn.Dims( ) ), 0.34
                                               );

  FuzzyCMeans< int > fcm( feats, clusters, m, epsilon );
  Matrix< double > membership( fcm.Run( false ) );
  Image< int > result( mask );


  COMMENT( "Assigning final label.", 0 );
  for( size_t elm = 0; elm < membership.size( 1 ); ++elm ) {
    size_t best_clt = 0;
    for( size_t clt = 1; clt < clusters; ++clt ) {
      cout << "clt: " << clt << ", elm: " << elm << "." << endl;
      if( membership( best_clt, elm ) < membership( clt, elm ) ) {
        best_clt = clt;
      }
    }
    /*    result[ feats.Index( elm ) ] = best_clt + 1; */
  }
  COMMENT( "Writing result.", 0 );
  return( result );
}

Bial::Image< int > DragDrop::brain_inhomogeneity_corretion( ) {
  Image< int > scn = m_img->getIntImage( );
  QString file = QFileDialog::getOpenFileName( this );
  Image< int > msk( Read< int >( file.toStdString( ) ) );

  float radius = QInputDialog::getDouble( this, "radius", "radius: ", 15.5, 7, 28 ); /* 15.5 */

  Image< int > res( Brain::InhomogeneityCorrection( scn, msk, radius ) );

  return( res );
}

Bial::Image< int > DragDrop::brain_opfclustering( ) {
  /*
   *  if ((argc < 5) || (argc == 6) || (argc == 8)) {
   *    cout << "Usage: " << argv[0] << " <input image> <brain mask> <modality> <output image> [<csf_min_scl>, "
   *                                    "<csf_max_scl>] [<gm_min_scl>, <gm_max_scl>]"
   *         << endl;
   *    cout << "\tmodality: T1, T2, or PD." << endl;
   *    cout << "\tcsf_min_scl, csf_max_cls: Default values = 0.4, 0.5. Suggested values: 0.1 <= csf_min_scl <= "
   *            "csf_max_scl <= 0.9."
   *         << endl;
   *    cout << "\tgm_min_scl, gm_max_scl: Default values = 0.4, 0.5. Suggested values: 0.1 <= gm_min_scl <= gm_max_scl
   *    <= "
   *            "0.9."
   *         << endl;
   *    exit(1);
   *  }
   */
  /* Fixed number sequence */
  Common::Randomize( false );

  /* Validating parameters. */
  QString q_modality = QInputDialog::getText( this, "Modality", "Modality: " );
  string modality_str( q_modality.toStdString( ) );
  MRIModality modality = MRIModality::T1;
  if( modality_str.compare( "T2" ) == 0 ) {
    modality = MRIModality::T2;
  }
  else if( modality_str.compare( "PD" ) == 0 ) {
    modality = MRIModality::PD;
  }
  else if( modality_str.compare( "T1" ) != 0 ) {
    cout << "Error: Modality must be T1, T2, or PD. Given \"" << modality_str << "\"." << endl;
    exit( 1 );
  }
  float csf_min_scl = QInputDialog::getDouble( this, "csf_min_scl", "csf_min_scl: ", 0.2 ); /* 0.2 */
  float csf_max_scl = QInputDialog::getDouble( this, "csf_max_scl", "csf_max_scl: ", 0.3 ); /*
                                                                                             * 0.3
                                                                                             *  if (argc > 5) {
                                                                                             */
  /*
   *    csf_min_scl = atof(argv[5]);
   *    csf_max_scl = atof(argv[6]);
   *    if (csf_min_scl > csf_max_scl) {
   *      cout << "Error: csf_min_scl can not be greater than csf_max_scl. Given: csf_min_scl = " << csf_min_scl
   *           << ", csf_max_scl = " << csf_max_scl << "." << endl;
   *      exit(1);
   *    }
   *  }
   */
  float gm_min_scl = QInputDialog::getDouble( this, "gm_min_scl", "gm_min_scl: ", 0.1 ); /* 0.1 */
  float gm_max_scl = QInputDialog::getDouble( this, "gm_max_scl", "gm_max_scl: ", 0.2 ); /* 0.2 */
  /*
   *  if (argc > 7) {
   *    gm_min_scl = atof(argv[7]);
   *    gm_max_scl = atof(argv[8]);
   *    if (gm_min_scl > gm_max_scl) {
   *      cout << "Error: gm_min_scl can not be greater than gm_max_scl. Given: gm_min_scl = " << gm_min_scl
   *           << ", gm_max_scl = " << gm_max_scl << "." << endl;
   *      exit(1);
   *    }
   * }
   */
  /* Reading input data */
  QString q_scn = QFileDialog::getOpenFileName( this, "scn" );
  Image< int > scn( Read< int >( q_scn.toStdString( ) ) );
  QString q_mask = QFileDialog::getOpenFileName( this, "mask" );
  Image< int > mask( Read< int >( q_mask.toStdString( ) ) );

  /* Running tissue segmentation */

  Image< int > tissues( Brain::TissueSegmentation( scn,
                                                   mask,
                                                   modality,
                                                   csf_min_scl,
                                                   csf_max_scl,
                                                   gm_min_scl,
                                                   gm_max_scl ) );


  /* Writing result */

  return( tissues );
}

Bial::Image< float > DragDrop::brain_segmentation( ) {
  /*
   *    if (argc != 3) {
   *      cout << "usage: " << argv[0] << " <input> <output>" << endl;
   *      cout << "input: .............. original image." << endl;
   *      cout << "output: .............. segmented heart binary image." << endl;
   *      return (0);
   *    }
   */
  /* Reading data contents */
  /*  Image< float > scn( Read< float >( argv[ 1 ] ) ); */
  Image< float > scn = m_img->getFltImage( );
/*  NiftiHeader nii( argv[ 1 ] ); */
  NiftiHeader nii( item->fileInfo( ).fileName( ).toStdString( ) );

  /* Getting image orientation */
  string orientation = nii.Orientation( );

  /* Executing and writing results. */

  Image< float > res( Brain::SkullStripping( scn, orientation ) );
  return( res );

/*    Write(res, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::color_cmeans_clustering( ) {
  /*
   *    if (argc < 3) {
   *      cout << "Usage: " << argv[0] << " <input color image> <output image> [<m>] [<epsilon>] [<clusters>]" << endl;
   *      cout << "\tm: Exponent of cmeans membership function. Default: 2" << endl;
   *      cout << "\tepsilon: Acceptable threshold of minimization function. Default: 0.1" << endl;
   *      cout << "\tclusters: Number of clusters. Default: 3" << endl;
   *      exit(-1);
   *    }
   */
  Image< Color > img = m_img->getClrImage( );

  Adjacency adj_rel( AdjacencyType::Circular( 1.5 ) );
  Feature< int > feats = Bial::ColorMedianFeature< int >( img, adj_rel, 0.5 );


  float m = QInputDialog::getDouble( this, "m", "m:" ); /*
                                                         * 2.0
                                                         *  if (argc > 3) {
                                                         *    m = atoi(argv[3]);
                                                         *  }
                                                         */
  float epsilon = QInputDialog::getDouble( this, "epsilon", "epsilon:" ); /*
                                                                           * 0.1
                                                                           *  if (argc > 4) {
                                                                           *    epsilon = atof(argv[4]);
                                                                           *  }
                                                                           */
  size_t clusters = QInputDialog::getInt( this, "clusters", "clusters:" ); /*
                                                                            * 3
                                                                            *  if (argc > 5) {
                                                                            *    clusters = atoi(argv[5]);
                                                                            *  }
                                                                            */

  FuzzyCMeans< int > fcm( feats, clusters, m, epsilon );
  Matrix< double > membership( fcm.Run( false ) );

  Image< int > result( img.Dim( ), img.PixelSize( ) );
  for( size_t elm = 0; elm < membership.size( 1 ); ++elm ) {
    size_t best_clt = 0;
    for( size_t clt = 1; clt < clusters; ++clt ) {
      cout << "clt: " << clt << ", elm: " << elm << "." << endl;
      if( membership( best_clt, elm ) < membership( clt, elm ) ) {
        best_clt = clt;
      }
    }
    result[ feats.Index( elm ) ] = best_clt + 1;
  }
  return( result );
  /*  Write(result, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::filtering_adaptive_anisotropic( ) {
  /*
   *    if ((argc < 4) || (argc > 6)) {
   *      cout << "Usage: " << argv[0] << " <Input image> <output image> <initial_kappa> [<diffusion function> "
   *           << "[<adjacency radius>] ]" << endl;
   *      cout << "\t\t<conservativeness>: (conserve all edges) 0.0 to 1.0 (remove all noise)." << endl;
   *      cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust(0.5); 4: Robust(1.0)."
   *           << "Default: 4." << endl;
   *      cout << "\t\t<initial_kappa>: Kappa for adaptive filtering. Suggested: 50.0 to 2000.0." << endl;
   *      return (0);
   *    }
   */

  /* Reading inputs: */
  Image< int > src = m_img->getIntImage( );
  float kappa = QInputDialog::getDouble( this, "kappa", "kappa: ", 0, 0, 1 );
  /*
   *  if ((kappa < 50.0) || (kappa > 2000.0)) {
   *    cout << "Error: Invalid kappa parameter. Expected: 50.0 to 2000.0. Found: " << kappa << endl;
   *    return (0);
   *  }
   */
  float radius = QInputDialog::getDouble( this, "radius", "radius: ", 1.01, 0, 1 ); /*
                                                                                     * 1.01
                                                                                     *  if (argc > 5) {
                                                                                     *    radius = atof(argv[5]);
                                                                                     */
  /*
   *    if ((radius < 1.0) || (radius > 3.0)) {
   *      cout << "Error: Invalid adjacency radius. Expected: 1.0 to 3.0. Found: " << radius << endl;
   *      return (0);
   *    }
   *  }
   */
  DiffusionFunction *diff_func = new RobustDiffusionFunction;

  QStringList options;
  options << "RobustDiffusion(0.5)"
          << "RobustDiffusion(1.0)"
          << "PowerDiffusion(1.0)"
          << "PowerDiffusion(2.0)"
          << "GaussianDiffusion";

  QString option = QInputDialog::getItem( this, "Function", "Function: ", options );
  if( option == "RobustDiffusion(0.5)" ) {
    diff_func = new RobustDiffusionFunction;
  }
  if( option == "RobustDiffusion(1.0)" ) {
    diff_func = new RobustDiffusionFunction;
  }
  if( option == "PowerDiffusion(1.0)" ) {
    diff_func = new PowerDiffusionFunction( 1.0 );
  }
  if( option == "PowerDiffusion(2.0)" ) {
    diff_func = new PowerDiffusionFunction( 2.0 );
  }
  if( option == "GaussianDiffusion" ) {
    diff_func = new GaussianDiffusionFunction;
  }
  Image< int > res( Filtering::AdaptiveAnisotropicDiffusion( src, diff_func, kappa, radius ) );

  return( res );
/*  Write(res, argv[2]); */
}

Bial::Image< int > DragDrop::filtering_anisotropic( ) {
  /*
   *  if ((argc < 3) || (argc > 7)) {
   *    cout << "Usage: " << argv[0] << " <Input image> <output image> [<diffusion function> [<iterations> [<kappa> "
   *         << "[<adjacency radius>]]]] " << endl;
   *    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust(0.5); 4: Robust(1.0)."
   *         << "Default: 4." << endl;
   *    cout << "\t\t<iterations>: 1 to 1000. Default: 3." << endl;
   *    cout << "\t\t<kappa>: 1.0 to 10000.0. Default: 10.0." << endl;
   *    cout << "\t\t<adjacency radius>: 1.0 to 3.0. Default: 1.01." << endl;
   *    return (0);
   *  }
   */
  /* Reading inputs: */
  Image< int > src = m_img->getIntImage( );

  size_t iterations = QInputDialog::getInt( this, "iterations", "iterations: ", 3 ); /*
                                                                                      * 3
                                                                                      *  if (argc > 4) {
                                                                                      */
  /*
   *    iterations = static_cast<size_t>(atoi(argv[4]));
   *    if ((iterations < 1) || (iterations > 1000)) {
   *      cout << "Error: Invalid number of iterations. Expected: 1 to 1000. Found: " << iterations << endl;
   *      return (0);
   *    }
   *  }
   */
  float kappa = QInputDialog::getDouble( this, "kappa", "kappa: ", 10.0 ); /* 10.0 */
  /*
   *  if (argc > 5) {
   *    kappa = atof(argv[5]);
   *    if ((kappa < 1.0) || (kappa > 10000.0)) {
   *      cout << "Error: Invalid kappa value. Expected: 1.0 to 10000.0. Found: " << kappa << endl;
   *      return (0);
   *    }
   *  }
   */
  float radius = QInputDialog::getDouble( this, "radius", "radius: ", 1.01 ); /* 1.01 */
  /*
   *  if (argc > 6) {
   *    radius = atof(argv[6]);
   *    if ((radius < 1.0) || (radius > 3.0)) {
   *      cout << "Error: Invalid adjacency radius. Expected: 1.0 to 3.0. Found: " << radius << endl;
   *      return (0);
   *    }
   *  }
   */

  DiffusionFunction *diff_func = new RobustDiffusionFunction;

  QStringList options;
  options << "RobustDiffusion(0.5)"
          << "RobustDiffusion(1.0)"
          << "PowerDiffusion(1.0)"
          << "PowerDiffusion(2.0)"
          << "GaussianDiffusion";

  QString option = QInputDialog::getItem( this, "Function", "Function: ", options );
  if( option == "RobustDiffusion(0.5)" ) {
    diff_func = new RobustDiffusionFunction;
  }
  if( option == "RobustDiffusion(1.0)" ) {
    diff_func = new RobustDiffusionFunction;
  }
  if( option == "PowerDiffusion(1.0)" ) {
    diff_func = new PowerDiffusionFunction( 1.0 );
  }
  if( option == "PowerDiffusion(2.0)" ) {
    diff_func = new PowerDiffusionFunction( 2.0 );
  }
  if( option == "GaussianDiffusion" ) {
    diff_func = new GaussianDiffusionFunction;
  }
  Image< int > res( Filtering::AnisotropicDiffusion( src, diff_func, kappa, iterations, radius ) );

  return( res );
/*  Write(res, argv[2]); */
}

Bial::Image< int > DragDrop::filtering_gaussian( ) {
  /*
   *    if ((argc < 3) || (argc > 5)) {
   *      cout << "Usage: " << argv[0] << " <input image> <output image> [<radius> [ <stamdard deviation> ] ]" << endl;
   *      cout << "\t\t<radius>: [1.0, 20.0]. Default: 1.9" << endl;
   *      cout << "\t\t<standard deviation>: (0.0,+oo). Default: 2.0" << endl;
   *      return (0);
   *    }
   */

  /* reading parameters */
  float radius = QInputDialog::getDouble( this, "radius", "radius: ", 1.9, 0, 1 ); /* 1.9 */
  float std_dev = QInputDialog::getDouble( this, "std_dev", "std_dev: ", 2, 0, 1 ); /* 2.0 */
  /*
   *  if ((radius < 1.0) || (radius > 20.0)) {
   *    cout << "Error: radius range: [1.0, 20.0]. Given: " << radius << endl;
   *    return (0);
   *  }
   *  if (std_dev <= 0.0) {
   *    cout << "Error: Standard deviation range: (0.0, +oo). Given: " << std_dev << endl;
   *    return (0);
   *  }
   */
  Image< int > img = m_img->getIntImage( );
  Image< int > res( Filtering::Gaussian( img, radius, std_dev ) );

  return( res );
/*  Write(res, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::filtering_mean( ) {
  /*
   *  if (argc != 4) {
   *    cout << "Usage: " << argv[0] << " <input image> <filtering radius> <output image>" << endl;
   *    return (0);
   *  }
   */
  Image< int > img = m_img->getIntImage( );
  float radius = QInputDialog::getDouble( this, "radius", "radius:" );
  Image< int > res( Filtering::Mean( img, radius ) );

  return( res );
/*    Write(res, argv[3], argv[1]); */
}

Bial::Image< int > DragDrop::filtering_median( ) {
  /*
   *  if (argc != 4) {
   *    cout << "Usage: " << argv[0] << " <input image> <filtering radius> <output image>" << endl;
   *    return (0);
   *  }
   */
  Image< int > img = m_img->getIntImage( );
  float radius = QInputDialog::getDouble( this, "radius", "radius:" );
  Image< int > res( Filtering::Median( img, radius ) );

  return( res );
/*    Write(res, argv[3], argv[1]); */
}

Bial::Image< int > DragDrop::filtering_optimum_anisotropic( ) {
  /*
   *  if ((argc < 4) || (argc == 7) || (argc > 8)) {
   *    cout << "Usage: " << argv[0] << " <Input image> <output image> <conservativeness> [<diffusion function> "
   *         << "[<adjacency radius> [<edge_region> <flat_region>]]]" << endl;
   *    cout << "\t\t<conservativeness>: (conserve all edges) 0.0 to 1.0 (remove all noise)." << endl;
   *    cout << "\t\t<diffusion function>: 0: Power(1.0); 1: Power(2.0); 2: Gaussian; 3: Robust(0.5); 4: Robust(1.0)."
   *         << "Default: 4." << endl;
   *    cout << "\t\t<edge_region>: Canny edge detection." << endl;
   *    cout << "\t\t<flat_region>: Background segmentation." << endl;
   *    return (0);
   *  }
   */

  /* Reading inputs: */
  Image< int > src = m_img->getIntImage( );
  float conservativeness = QInputDialog::getDouble( this, "conservativeness", "conservativeness:" );
  /*
   *  if ((conservativeness < 0.0) || (conservativeness > 1.0)) {
   *    cout << "Error: Invalid conservativeness parameter. Expected: 0.0 to 1.0. Found: " << conservativeness << endl;
   *    return (0);
   *  }
   */
  float radius = QInputDialog::getDouble( this, "radius", "radius:" ); /*
                                                                        * 1.01
                                                                        *  if (argc > 5) {
                                                                        *    radius = atof(argv[5]);
                                                                        *    if ((radius < 1.0) || (radius > 3.0)) {
                                                                        */
  /*
   *      cout << "Error: Invalid adjacency radius. Expected: 1.0 to 3.0. Found: " << radius << endl;
   *      return (0);
   *    }
   *  }
   *  DiffusionFunction *diff_func;
   *  if (argc < 5) {
   *    diff_func = new RobustDiffusionFunction(1.0);
   *  } else {
   *    int func = atof(argv[4]);
   *    if (func == 0) {
   *      diff_func = new PowerDiffusionFunction(1.0);
   *    } else if (func == 1) {
   *      diff_func = new PowerDiffusionFunction(2.0);
   *    } else if (func == 2) {
   *      diff_func = new GaussianDiffusionFunction;
   *    } else if (func == 3) {
   *      diff_func = new RobustDiffusionFunction(0.5);
   *    } else if (func == 4) {
   *      diff_func = new RobustDiffusionFunction(1.0);
   *    } else {
   *      cout << "Error: Invalid diffusion function option. Expected: 0 to 4. Found: " << func << endl;
   *      return (0);
   *    }
   *  }
   */
  DiffusionFunction *diff_func = new RobustDiffusionFunction;

  QStringList options;
  options << "RobustDiffusion(0.5)"
          << "RobustDiffusion(1.0)"
          << "PowerDiffusion(1.0)"
          << "PowerDiffusion(2.0)"
          << "GaussianDiffusion";

  QString option = QInputDialog::getItem( this, "Function", "Function: ", options );
  if( option == "RobustDiffusion(0.5)" ) {
    diff_func = new RobustDiffusionFunction;
  }
  if( option == "RobustDiffusion(1.0)" ) {
    diff_func = new RobustDiffusionFunction;
  }
  if( option == "PowerDiffusion(1.0)" ) {
    diff_func = new PowerDiffusionFunction( 1.0 );
  }
  if( option == "PowerDiffusion(2.0)" ) {
    diff_func = new PowerDiffusionFunction( 2.0 );
  }
  if( option == "GaussianDiffusion" ) {
    diff_func = new GaussianDiffusionFunction;
  }
  /*
   *  if (argc > 6) {
   *    Image<int> edge_region(Read<int>(argv[6]));
   *    Image<int> flat_region(Read<int>(argv[7]));
   *    Image<int> res(
   *        Filtering::OptimumAnisotropicDiffusion(src, diff_func, radius, conservativeness, edge_region, flat_region));
   *    Write(res, argv[2]);
   *  } else {
   */

  Image< int > res( Filtering::OptimalAnisotropicDiffusion( src, diff_func, radius, conservativeness ) );
  return( res );

/*
 *    Write(res, argv[2]);
 *  }
 *  if (argc < 5) {
 *    delete (dynamic_cast<RobustDiffusionFunction *>(diff_func));
 *  } else {
 *    int func = atof(argv[4]);
 *    if (func == 0) {
 *      delete (dynamic_cast<PowerDiffusionFunction *>(diff_func));
 *    } else if (func == 1) {
 *      delete (dynamic_cast<PowerDiffusionFunction *>(diff_func));
 *    } else if (func == 2) {
 *      delete (dynamic_cast<GaussianDiffusionFunction *>(diff_func));
 *    } else if (func == 3) {
 *      delete (dynamic_cast<RobustDiffusionFunction *>(diff_func));
 *    } else { / * if( func == 4 ) * /
 *      delete (dynamic_cast<RobustDiffusionFunction *>(diff_func));
 *    }
 *  }
 */
}

Bial::Image< int > DragDrop::gradient_canny( ) {
  /*
   *  if (argc != 6) {
   *    cout << "Usage: " << argv[0] << " <input image> <output image> <lower threshold> <higher threshold> <sigma>"
   *         << endl;
   *    cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
   *    cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
   *    return (0);
   *  }
   *  try {
   */
  COMMENT( "Reading parameters.", 0 );
  Image< int > img = m_img->getIntImage( );
  float low_thres = QInputDialog::getDouble( this, "low_thres", "low_thres: ", 0, 0, 1 );
  float hig_thres = QInputDialog::getDouble( this, "high_thres", "high_thres: ", 0, 0, 1 );
  float sigma = QInputDialog::getDouble( this, "sigma", "sigma: ", 0, 0, 1 );

  COMMENT( "Running Canny.", 0 );
  Image< int > res( Gradient::Canny( img, low_thres, hig_thres, sigma ) );

  COMMENT( "Writing result.", 0 );
  /*      Write(res, argv[2], argv[1]); */
  return( res );
/*  } */
}

Bial::Image< int > DragDrop::gradient_directional_sobel( ) {
  /*
   *    if (argc != 4) {
   *      cout << "Usage: " << argv[0] << " <input image> <output image> <direction> ]" << endl;
   *      cout << "\t\t<direction>: Lower than the number of dimensions of the input image." << endl;
   *      return (0);
   *    }
   */
  /* reading parameters */
  Image< int > img = m_img->getIntImage( );
  size_t direction = QInputDialog::getInt( this, "direction", "direction:" );
  /*
   *  if (direction >= img.Dims()) {
   *    cout << "Error: Invalid direction. Given: " << direction << ". Image dimensions: " << img.Dims() << endl;
   *    return (0);
   *  }
   */
  Image< int > res( Gradient::DirectionalSobel( img, direction ) );

  /*    Write(res, argv[2], argv[1]); */

  return( res );
}

Bial::Image< int > DragDrop::gradient_gabor( ) {
  /*
   *  if ((argc < 4) || (argc > 5)) {
   *    cout << "Usage: " << argv[0] << " <input image> <standard deviation> <output magnitude> [<output direction> ]"
   *         << endl;
   *    return (0);
   *  }
   */

  /* reading parameters */
  Image< int > img = m_img->getIntImage( );
  Image< int > mag( img );
  Image< int > dir( img );
  float sigma = QInputDialog::getDouble( this, "sigma", "sigma:" );

  Gradient::Gabor( img, sigma, &mag, &dir );

  /*
   *  Write(mag, argv[3], argv[1]);
   *    if (argc == 5) {
   *      Write(dir, argv[4], argv[1]);
   *    }
   */

  return( mag );
}

Bial::Image< int > DragDrop::gradient_hole_closing( ) {
  /*
   *    if (argc != 7) {
   *      cout << "Usage: " << argv[0]
   *           << " <input image> <output image> <lower threshold> <higher threshold> <sigma> <scales>" << endl;
   *      cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
   *      cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
   *      return (0);
   *    }
   */
  COMMENT( "Reading inputs.", 0 );
  Image< int > img = m_img->getIntImage( );
  float low_thres = QInputDialog::getDouble( this, "low_thres", "low_thres:" );
  float hig_thres = QInputDialog::getDouble( this, "high_thres", "high_thres:" );
  float sigma = QInputDialog::getDouble( this, "sigma", "sigma:" );
  size_t scales = QInputDialog::getInt( this, "scales", "scales:" );

  Image< int > grad( Gradient::MultiScaleCanny( img, low_thres, hig_thres, sigma, scales ) );
  COMMENT( "Setting all canny gradient levels to 1.", 0 );
  for( size_t pxl = 0; pxl < grad.size( ); ++pxl ) {
    if( grad( pxl ) != 0 ) {
      grad( pxl ) = 1;
    }
  }
  Image< int > res( Gradient::CloseHoles( grad ) );


  return( res );
/*  Write( res, argv[ 2 ], argv[ 1 ] ); */
}

Bial::Image< int > DragDrop::gradient_morphological( ) {
  /*
   *  if (argc != 3) {
   *    cout << "Usage: " << argv[0] << " <input image> <output image>" << endl;
   *    exit(1);
   *  }
   */
  Image< int > img = m_img->getIntImage( );


  Adjacency spheric( AdjacencyType::HyperSpheric( 1.0, img.Dims( ) ) );
  Image< int > gradient( Image< int >( Gradient::Morphological( img, spheric ) ) );


  /*  Write( gradient, argv[ 2 ], argv[ 1 ] ); */

  return( gradient );
}

Bial::Image< int > DragDrop::gradient_multiscalecanny( ) {
  /*
   *  if (argc != 7) {
   *    cout << "Usage: " << argv[0]
   *         << " <input image> <output image> <lower threshold> <higher threshold> <sigma> <scales>" << endl;
   *    cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
   *    cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
   *    return (0);
   *  }
   */
  /* reading parameters */
  Image< int > img = m_img->getIntImage( );
  float low_thres = QInputDialog::getDouble( this, "low_thres", "low_thres:" );
  float hig_thres = QInputDialog::getDouble( this, "high_thres", "high_thres:" );
  float sigma = QInputDialog::getDouble( this, "sigma", "sigma:" );
  size_t scales = QInputDialog::getInt( this, "scales", "scales:" );

  Image< int > res( Gradient::MultiScaleCanny( img, low_thres, hig_thres, sigma, scales ) );

  return( res );

  /*  Write(res, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::gradient_multi_scale_size_canny( ) {
  /*
   *    if (argc != 8) {
   *      cout << "Usage: " << argv[0]
   *           << " <input image> <output image> <lower threshold> <higher threshold> <sigma> <scales> <fraction>" <<
   *           endl;
   *      cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
   *      cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
   *      cout << "\t\t<fraction>: Fraction of the maximum edge size to be ignored. In (0.0, 1.0]." << endl;
   *      return (0);
   *    }
   */
  /* reading parameters */
  Image< int > img = m_img->getIntImage( );
  float low_thres = QInputDialog::getDouble( this, "low_thres", "low_thres: ", 0, 0, 1 );
  float hig_thres = QInputDialog::getDouble( this, "high_thres", "high_thres: ", 0, 0, 1 );
  float sigma = QInputDialog::getDouble( this, "sigma", "sigma: ", 0, 0, 1 );
  size_t scales = QInputDialog::getDouble( this, "scales", "scales: ", 0, 0, 1 );
  float fraction = QInputDialog::getDouble( this, "fraction", "fraction: ", 0, 0, 1 );

  Image< int > res( Gradient::MultiScaleSizeCanny( img, low_thres, hig_thres, sigma, scales, fraction ) );

  return( res );
/*    Write(res, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::gradient_multi_sub_scale_canny( ) {
  /*
   *    if (argc != 7) {
   *      cout << "Usage: " << argv[0]
   *           << " <input image> <output image> <lower threshold> <higher threshold> <sigma> <scales>" << endl;
   *      cout << "\t\t<lower threshold>, <higher threshold>: Hysteresis thresholds From 0.0 to 1.0." << endl;
   *      cout << "\t\t<sigma>: Standard deviation of Gaussian filter. From 0.0 to +oo." << endl;
   *      return (0);
   *    }
   */

  /* reading parameters */
  Image< int > img = m_img->getIntImage( );
  float low_thres = QInputDialog::getDouble( this, "low_thres", "low_thres: ", 0, 0, 1 );
  float hig_thres = QInputDialog::getDouble( this, "high_thres", "high_thres: ", 0, 0, 1 );
  float sigma = QInputDialog::getDouble( this, "sigma", "sigma: ", 0, 0 );
  size_t scales = QInputDialog::getInt( this, "scales", "scales: " );

  Image< int > res( Gradient::MultiSubScaleCanny( img, low_thres, hig_thres, sigma, scales ) );

  return( res );
/*  Write(res, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::gradient_scalecanny( ) {
  /*
   *    if (argc != 6) {
   *      cout << "Usage: " << argv[0] << " <input image> <output image> <window side> <lower threshold> <higher
   *      threshold>"
   *           << endl;
   *      cout << "\t\t<lower threshold>, <higher threshold>: From 0.0 to 1.0." << endl;
   *      cout << "\t\t<window side>: From 1 to window maximum side." << endl;
   *      return (0);
   *    }
   */
  /* reading parameters */
  Image< int > img = m_img->getIntImage( );
  size_t window_side = QInputDialog::getInt( this, "window_side", "window_side: ", 1, 1 );
  float low_thres = QInputDialog::getDouble( this, "low_thres", "low_thres: ", 0, 0, 1 );
  float hig_thres = QInputDialog::getDouble( this, "high_thres", "high_thres: ", 0, 0, 1 );

  Image< int > res( Gradient::ScaleCanny( img, window_side, low_thres, hig_thres ) );

  return( res );
/*  Write(res, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::gradient_sobel( ) {
  /*
   *    if ((argc < 3) || (argc > 4)) {
   *      cout << "Usage: " << argv[0] << " <input image> <output magnitude> [<output direction> ]" << endl;
   *      return (0);
   *    }
   */
  /* reading parameters */
  Image< int > img = m_img->getIntImage( );

  Image< int > mag( img );
  Image< int > dir( img );

/*  Gradient::Sobel( img, &mag, &dir ); */

  return( mag );
  /*
   *  Write(mag, argv[2], argv[1]);
   *  Write(dir, argv[3], argv[1]);
   */
}

Bial::Image< int > DragDrop::gradient_supressed_sobel( ) {
  /*
   *    if (argc != 3) {
   *      cout << "Usage: " << argv[0] << " <input image> <output magnitude>" << endl;
   *      return (0);
   *    }
   */
  /* reading parameters */
  Image< int > img = m_img->getIntImage( );

  Image< int > res( Gradient::NonMaxSobelSuppression( img ) );

  return( res );
/*    Write(res, argv[2], argv[1]); */
}

Bial::Image< float > DragDrop::heart_cog( ) {
  /*
   *    if (argc != 2) {
   *      cout << "usage: " << argv[0] << " <input>" << endl;
   *      cout << "input: .............. original image" << endl;
   *      return (0);
   *    }
   */
  /* Reading data contents */

  Image< float > scn = m_img->getFltImage( );
  NiftiHeader nii( item->fileInfo( ).absoluteFilePath( ).toStdString( ) );


  /* Getting image orientation */
  string orientation = nii.Orientation( );

  /* Initializing output files */

  Vector< Box > best_rectangles;
  Image< float > similarity( scn );


  /* Computing COG */
  Vector< size_t > COG = Heart::WeightedCOG( scn, orientation, best_rectangles, similarity );

  /* Drawing regions to an image */

  Image< int > regions( scn );
  regions.Set( 0 );

  std::cout << "Rectangles: " << std::endl;
  for( size_t rtg = 0; rtg < best_rectangles.size( ); ++rtg ) {
    std::cout << rtg << ":" << std::endl;
    best_rectangles[ rtg ].Print( std::cout );
    best_rectangles[ rtg ].Draw( regions );
  }
/* Printing COG value */
  cout << COG[ 0 ] << " " << COG[ 1 ] << " " << COG[ 2 ] << endl;

  /* Writting output images */
  return( similarity );

/*
 *     Write(similarity, "dat/similarity.nii.gz", argv[1]);
 *     Write(regions, "dat/regions.nii.gz", argv[1]);
 */

}

Bial::Image< float > DragDrop::heart_segmentation( ) {
  /* Reading data contents */
  Image< float > scn = m_img->getFltImage( );
  NiftiHeader nii( item->fileInfo( ).absoluteFilePath( ).toStdString( ) );
  /* Getting image orientation */
  string orientation = nii.Orientation( );
  /*   Executing and writing results. */
  Image< float > res( Heart::Segmentation( scn, orientation ) );
  return( res );
  /*  Write(res, argv[2], argv[1]); */
}

Bial::Image< int > DragDrop::image_binary_mask( ) {
  Image< int > scn = m_img->getIntImage( );
  for( size_t p = 0; p < scn.size( ); ++p ) {
    if( scn[ p ] != 0 ) {
      scn[ p ] = 1;
    }
  }
  return( scn );
}

Bial::Image< int > DragDrop::image_color_binary_mask( ) {
  Image< Color > scn = m_img->getClrImage( );
  Image< int > res( scn.Dim( ), scn.PixelSize( ) );
  for( size_t chl = 1; chl < 4; ++chl ) {
    for( size_t pxl = 0; pxl < scn.size( ); ++pxl ) {
      if( scn[ pxl ][ chl ] != 0 ) {
        res[ pxl ] = 1;
      }
    }
  }
  return( res );
}

Bial::Image< int > DragDrop::image_dilate( ) {
  Image< int > img = m_img->getIntImage( );
  float radius = QInputDialog::getDouble( this, "radius", "radius: " ); /* no default */

  Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
  Image< int > res( Morphology::Dilate( img, adj ) );


  return( res );
}

Bial::Image< int > DragDrop::image_equalize( ) {
  Image< int > img = m_img->getIntImage( );
/*  Signal eql = Signal::EqualizedHistogram( img ); */
  Image< int > res( img );
/*
 *  for( size_t val = 0; val < eql.size( ); ++val ) {
 *    eql[ val ] = round( eql[ val ] );
 *  }
 *  for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
 *    res[ pxl ] = static_cast< int >( eql[ img[ pxl ] ] );
 *  }
 */
  return( res );
}

Bial::Image< int > DragDrop::image_erode( ) {
  Image< int > img = m_img->getIntImage( );
  float radius = QInputDialog::getDouble( this, "radius", "radius: " ); /* no default */

  Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
  Image< int > res( Morphology::Erode( img, adj ) );


  return( res );
}

void DragDrop::image_getslice( ) {
  /*
   *  if (argc != 5) {
   *    cout << "usage: " << argv[0] << " <input 3D image> <output 2D image> <slice number> <direction>" << endl;
   *    cout << "\t<direction>: 0, 1, or 2." << endl;
   *    return (0);
   *  }
   *  Image<int> img(Read<int>(argv[1]));
   *  size_t slice = atoi(argv[3]);
   *  size_t dir = atoi(argv[4]);
   *  size_t x_size = img.size(0);
   *  size_t y_size = img.size(1);
   *  size_t z_size = img.size(2);
   *  if (dir == 0) {
   *    Image<int> res(y_size, z_size);
   *    for (size_t z = 0; z < z_size; ++z) {
   *      for (size_t y = 0; y < y_size; ++y) {
   *        res(y, z) = img(slice, y, z);
   *      }
   *    }
   *    Write(res, argv[2], argv[1]);
   *  } else if (dir == 1) {
   *    Image<int> res(x_size, z_size);
   *    for (size_t z = 0; z < z_size; ++z) {
   *      for (size_t x = 0; x < x_size; ++x) {
   *        res(x, z) = img(x, slice, z);
   *      }
   *    }
   *    Write(res, argv[2], argv[1]);
   *  } else {
   *    Image<int> res(x_size, y_size);
   *    for (size_t y = 0; y < y_size; ++y) {
   *      for (size_t x = 0; x < x_size; ++x) {
   *        res(x, y) = img(x, y, slice);
   *      }
   *    }
   *    Write(res, argv[2], argv[1]);
   *  }
   */
}

Bial::Image< int > DragDrop::image_ift_watershed( ) {
  Image< int > gradient = m_img->getIntImage( );
  Image< int > label( Segmentation::Watershed( gradient ) );

  return( label );
}

Bial::Image< int > DragDrop::image_merge( ) {
  QString folder = QFileDialog::getExistingDirectory( this, "Folder" ); /* dicom folder */

  Vector< Image< int > > slices( ReadDir< int >( folder.toStdString( ) ) );
  Image< int > merge( ImageOp::Merge( slices ) );


  return( merge );
}

Bial::Image< int > DragDrop::image_minimum_spanning_forest( ) {
  Image< int > img = m_img->getIntImage( );
  size_t classes = QInputDialog::getInt( this, "classes", "classes: " ); /* no default */
  Feature< int > feature( img.size( ), 1 );
  for( size_t elm = 0; elm < feature.Elements( ); ++elm ) {
    feature( elm, 0 ) = img[ elm ];
  }
  Image< int > label( Segmentation::MinimumSpanningForest( img, feature, classes ) );

  return( label );
}

Bial::Image< int > DragDrop::image_multiply( ) {
  Image< int > img1 = m_img->getIntImage( );
  float scalar = QInputDialog::getDouble( this, "scalar", "scalar: " ); /* no default */

  Image< int > res( img1 * scalar );

  return( res );
}

Bial::Image< Bial::Color > DragDrop::image_multiply_mask( ) {
  /*
   *    if (argc < 4) {
   *      cout << "Usage: " << argv[0] << " <input image> <binary mask image> <output image>" << endl;
   *      exit(-1);
   *    }
   */
  Image< Color > img = m_img->getClrImage( );
  QString q_mask = QFileDialog::getOpenFileName( this, "mask" );
  Image< int > mask( Read< int >( q_mask.toStdString( ) ) );
/*
 *  if( img.Dims( ) != mask.Dims( ) ) {
 *    cout << "Error: Input images must have the same dimensions." << endl;
 *    return( 0 );
 *  }
 */
  for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
    if( mask[ pxl ] == 0 ) {
      for( size_t ch = 1; ch < 4; ++ch ) {
        img[ pxl ][ ch ] = 0;
      }
    }
  }
  return( img );
}

Bial::Image< int > DragDrop::transform_euclidean( ) {
  Image< int > scn = m_img->getIntImage( );

  Image< int > bdr( Segmentation::BorderPixels( scn, AdjacencyType::HyperSpheric( 1.1, scn.Dims( ) ) ) );
  Image< int > res( Transform::EDT( bdr ) );


  return( res );
}

void DragDrop::on_pushButtonBW_11_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = transform_euclidean( );

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_13_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< Color > res = image_multiply_mask( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_7_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_multiply( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_10_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_minimum_spanning_forest( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_9_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_merge( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_8_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_ift_watershed( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_5_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_erode( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_4_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_equalize( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_3_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_dilate( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_14_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_color_binary_mask( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_15_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = image_binary_mask( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_16_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< float > res = heart_segmentation( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_17_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< float > res = heart_cog( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_18_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_supressed_sobel( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_19_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_sobel( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_20_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_scalecanny( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_21_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_multiscalecanny( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_22_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_multi_scale_size_canny( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_23_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_multiscalecanny( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_24_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_morphological( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_25_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_hole_closing( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_26_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_gabor( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_27_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_directional_sobel( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_28_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = gradient_canny( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_29_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = filtering_optimum_anisotropic( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_30_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = filtering_median( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_31_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = filtering_mean( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_32_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = filtering_gaussian( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_33_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = filtering_anisotropic( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_34_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = filtering_adaptive_anisotropic( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_35_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = color_cmeans_clustering( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_37_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< float > res = brain_segmentation( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_38_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = brain_opfclustering( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_39_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = brain_inhomogeneity_corretion( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_pushButtonBW_40_clicked( ) {
  if( not item or item->pixmap( ).isNull( ) ) {
    return;
  }
  if( item->fileInfo( ).suffix( ) == "dcm" ) {
    QMessageBox::critical( this, "Erro!", "Suporte a escrita de DICOM ainda não implementado!" );
    return;
  }
  /*
   *  QImage temp = item->pixmap().toImage();
   *  temp.invertPixels();
   *  QPixmap pix(QPixmap::fromImage(temp));
   */

  /*  */

  Image< int > res = brain_cmeansclustering( );
  /*
   *  Image<int> res = AnisotropicBial();
   *  Image<int> res = CannyBial();
   */

/*  Write( res, QString( outputFolder + "/" + item->fileInfo( ).fileName( ) ).toStdString( ) ); */

  delete m_img2;
  m_img2 = new GuiImage( outputFolder + "/" + item->fileInfo( ).fileName( ), this );

  QPixmap pix( m_img2->getSlice( 0 ) );

  qDebug( ) << "x: " << m_img2->getSlice( 0 ).width( );
  qDebug( ) << "y: " << m_img2->getSlice( 0 ).height( );
  if( pix.isNull( ) ) {
    qDebug( ) << "NULL!";
  }
  /*  */

  showImage2( pix, item->fileInfo( ) );
}

void DragDrop::on_groupBoxSaida_toggled( bool checked ) {
  ui->graphicsViewOutput->setVisible( checked );
  ui->pushButtonOutputFolder->setVisible( checked );
}
