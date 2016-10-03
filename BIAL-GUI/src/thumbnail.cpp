#include "thumbnail.hpp"
#include <QDebug>
#include <QFileInfo>
#include <qboxlayout.h>
#include <qlabel.h>

Thumbnail::Thumbnail( GuiImage *image, int number, int size, QWidget *parent ) : QFrame( parent ),
  m_imageNumber( number ) {
  if( !image ) {
    throw std::invalid_argument( "Image does not exists." );
  }
  COMMENT( "Creating thumbnail for image " << image->fileName( ).toStdString( ), 0 );
  setFrameStyle( QFrame::Raised );
  setFrameShape( QFrame::StyledPanel );
  setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
  QPixmap pix;
  pix = image->getSlice( 0 );
  pix = pix.scaled( size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation );

  QLabel *imageLabel = new QLabel;
  imageLabel->setPixmap( pix );

  QFileInfo info( image->fileName( ) );
  /*
   *  QLabel *textLabel = new QLabel;
   *  textLabel->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
   *  textLabel->setWordWrap( true );
   */
  QString fname = info.fileName( );
  setToolTip( fname );
  setAttribute( Qt::WA_AlwaysShowToolTips );
  /*
   *  fname.truncate( 15 );
   *  textLabel->setText( fname );
   */
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget( imageLabel, 0, Qt::AlignHCenter );
  /*  layout->addWidget( textLabel, 0, Qt::AlignCenter ); */
  layout->addStretch( 1 );
  layout->setMargin( 2 );
  setLayout( layout );
  /*  setFrameShape(QFrame::Panel); */
}

void Thumbnail::mousePressEvent( QMouseEvent* ) {
  COMMENT( "Thumbnail clicked: " << imageNumber( ), 0 )
  emit changeImage( imageNumber( ) );
}

int Thumbnail::imageNumber( ) const {
  return( m_imageNumber );
}

void Thumbnail::setImageNumber( int imageNumber ) {
  m_imageNumber = imageNumber;
}
