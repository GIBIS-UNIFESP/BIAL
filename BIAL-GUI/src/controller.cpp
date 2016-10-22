#include "controller.h"
#include "thumbswidget.h"

#include <QDebug>
#include <QFile>
#include <qsettings.h>
#include "defaulttool.h"
Controller::Controller( int views, QObject *parent )
  : QObject( parent ), bw2dFormat( new BW2DFormat( this ) ), rgb2dFormat( new RGB2DFormat( this ) ),
  bw3dFormat( new BW3DFormat( this ) ) {
  for( int item = 0; item < views; ++item ) {
    m_pixmapItems.append( new QGraphicsPixmapItem( ) );
    m_labelItems.append( new QGraphicsPixmapItem( ) );
  }
  m_currentImagePos = -1;
  connect( rgb2dFormat, &DisplayFormat::updated, this, &Controller::currentImageChanged );
  connect( bw3dFormat, &DisplayFormat::updated, this, &Controller::currentImageChanged );
  connect( bw2dFormat, &DisplayFormat::updated, this, &Controller::currentImageChanged );
  connect( rgb2dFormat, &DisplayFormat::updated, this, &Controller::update );
  connect( bw3dFormat, &DisplayFormat::updated, this, &Controller::update );
  connect( bw2dFormat, &DisplayFormat::updated, this, &Controller::update );
}

GuiImage* Controller::currentImage( ) {
  return( imageAt( currentImagePos( ) ) );
}

GuiImage* Controller::imageAt( int pos ) {
  if( ( pos >= 0 ) && ( pos < m_images.size( ) ) ) {
    return( m_images.at( pos ) );
  }
  return( nullptr );
}

int Controller::currentImagePos( ) const {
  return( m_currentImagePos );
}

bool Controller::addImage( QString fname ) {
  qDebug( ) << "addimage.";
  COMMENT( "Loading file " << fname.toStdString( ), 0 );
  GuiImage *img = nullptr;
  try {
    img = new GuiImage( fname, this );
  }
  catch( std::bad_alloc e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::runtime_error e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::out_of_range e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( std::logic_error e ) {
    BIAL_WARNING( e.what( ) );
  }
  catch( ... ) {
  }
  if( img == nullptr ) {
    emit containerUpdated( );
    return( false );
  }
  m_images.append( img );

  m_thumbsWidget->addThumbnail( img );
  if( currentImagePos( ) == -1 ) {
    setCurrentImagePos( 0 );
  }
  setRecentFile( fname );

  emit containerUpdated( );
  emit currentImageChanged( );

  return( true );
}

bool Controller::removeCurrentLabel( ) {
  /* TODO Controller::removeCurrentLabel( ) */
  return( false );
}

void Controller::removeCurrentImage( ) {
  m_images.removeAt( currentImagePos( ) );
  m_thumbsWidget->removeAt( currentImagePos( ) );
  if( currentImagePos( ) == 0 ) {
    setCurrentImagePos( 0 );
  }
  else {
    setCurrentImagePos( currentImagePos( ) - 1 );
  }
  emit containerUpdated( );
}

bool Controller::isEmpty( ) {
  return( m_images.isEmpty( ) );
}

void Controller::clear( ) {
  COMMENT( "Reseting images.", 1 );
  qDeleteAll( m_images );
  m_images.clear( );
  setCurrentImagePos( -1 );
  m_thumbsWidget->clear( );
  emit containerUpdated( );
}

int Controller::size( ) {
  return( m_images.size( ) );
}

void Controller::update( ) {
  COMMENT( "UPDATING IMAGE!", 2 );

  GuiImage *img = currentImage( );
  if( img ) {
    std::array< bool, 4 > showItens = currentFormat( )->getViews( );
    for( int axis = 0; axis < 4; ++axis ) {
      if( showItens[ axis ] ) {
        m_labelItems.at( axis )->setPixmap( QPixmap( ) );
        const QPixmap &pix = img->getSlice( axis );
        m_pixmapItems.at( axis )->setPixmap( pix );
        Tool *tool = img->currentTool( );
        if( tool && tool->hasLabel( ) ) {
          m_labelItems.at( axis )->setPixmap( tool->getLabel( axis ) );
        }
      }
    }
  }
  else {
    for( int axis = 0; axis < m_pixmapItems.size( ); ++axis ) {
      m_pixmapItems[ axis ]->setPixmap( QPixmap( ) );
    }
  }
  emit imageUpdated( );
}

void Controller::setCurrentImagePos( int position ) {
  m_currentImagePos = position;
  if( currentImage( ) != nullptr ) {
    disconnect( currentImage( ), &GuiImage::imageUpdated, this, &Controller::update );
  }
  if( currentImage( ) != nullptr ) {
    emit currentImageChanged( );
    update( );
    connect( currentImage( ), &GuiImage::imageUpdated, this, &Controller::update );
  }
}

void Controller::loadNextImage( ) {
  if( currentImagePos( ) == ( m_images.count( ) - 1 ) ) {
    setCurrentImagePos( 0 );
  }
  else {
    setCurrentImagePos( currentImagePos( ) + 1 );
  }
}

void Controller::setCurrentSlice( size_t view, size_t slice ) {
  currentImage( )->setCurrentSlice( view, slice );
}

void Controller::setZoom( int value ) {
  /* FIXME Zoom doesn't work yet. */
  scale = 1.0 + value / 100.0;
  update( );
}

void Controller::setInterpolation( bool isSmooth ) {
  for( int view = 0; view < m_pixmapItems.size( ); ++view ) {
    if( isSmooth ) {
      m_pixmapItems[ view ]->setTransformationMode( Qt::SmoothTransformation );
    }
    else {
      m_pixmapItems[ view ]->setTransformationMode( Qt::FastTransformation );
    }
  }
}

void Controller::rotateAll90( ) {
  currentImage( )->rotateAll90( );
  emit currentImageChanged( );
}

void Controller::rotate90( size_t view ) {
  currentImage( )->rotate90( view );
  emit currentImageChanged( );
}

void Controller::flipH( size_t view ) {
  currentImage( )->flipH( view );
  emit currentImageChanged( );
}

void Controller::flipV( size_t view ) {
  currentImage( )->flipV( view );
  emit currentImageChanged( );
}

void Controller::setRecentFile( QString fname ) {
  COMMENT( "Setting recent file to : \"" << fname.toStdString( ) << "\"", 1 );
  if( !QFile( fname ).exists( ) ) {
    return;
  }
  QSettings settings;
  QStringList files = settings.value( "recentFileList" ).toStringList( );

  files.removeAll( fname );

  files.prepend( fname );
  while( files.size( ) > MaxRecentFiles ) {
    files.removeLast( );
  }
  settings.setValue( "recentFileList", files );

  emit recentFilesUpdated( );
}

void Controller::setThumbsWidget( ThumbsWidget *thumbsWidget ) {
  m_thumbsWidget = thumbsWidget;
  m_thumbsWidget->setController( this );
}

DisplayFormat* Controller::currentFormat( ) {
  Modality mod = currentImage( )->modality( );
  if( mod == Modality::RGB2D ) {
    return( rgb2dFormat );
  }
  else if( mod == Modality::BW3D ) {
    return( bw3dFormat );
  }
  else {
    return( bw2dFormat );
  }
}

QGraphicsPixmapItem* Controller::getPixmapItem( size_t view ) {
  return( m_pixmapItems.at( view ) );
}

QGraphicsPixmapItem* Controller::getLabelItem( size_t view ) {
  return( m_labelItems.at( view ) );
}
