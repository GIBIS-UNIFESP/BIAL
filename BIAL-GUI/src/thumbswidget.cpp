#include "controller.h"
#include "thumbswidget.h"
#include "ui_thumbswidget.h"

ThumbsWidget::ThumbsWidget( QWidget *parent ) : QWidget( parent ), ui( new Ui::ThumbsWidget ), controller(nullptr) {
  ui->setupUi( this );
}

ThumbsWidget::~ThumbsWidget( ) {
  delete ui;
}

void ThumbsWidget::clear( ) {
  COMMENT( "Reseting thumbnails.", 1 );
  qDeleteAll( thumbs );
  thumbs.clear( );
}

void ThumbsWidget::addThumbnail( GuiImage *image ) {
  COMMENT( "Loading new thumbnail", 1 );
  int num = thumbs.size( );
  int size = width( ) - 40;
  Thumbnail *thumbnail = new Thumbnail( image, num, size );
  thumbs.append( thumbnail );
  ui->thumbsLayout->addWidget( thumbnail, num, 0, Qt::AlignHCenter );
  ui->thumbsLayout->setMargin( 0 );
  ui->thumbsLayout->setVerticalSpacing( 1 );
  /*  thumbnail->show(); */
  connect( thumbnail, &Thumbnail::changeImage, controller, &Controller::setCurrentImagePos );
}

void ThumbsWidget::setController( Controller *value ) {
  controller = value;
}

void ThumbsWidget::removeAt( int pos ) {
  COMMENT( "Removing thumbnail at " << pos << ".", 1 );
  delete thumbs[ pos ];
  thumbs.removeAt( pos );
  for( int i = 0; i < thumbs.size( ); ++i ) {
    thumbs[ i ]->setObjectName( QString::number( i ) );
  }
}
