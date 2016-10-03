#include "graphicsview.h"

#include <QDebug>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>

GraphicsView::GraphicsView( QWidget *parent ) : QGraphicsView( parent ) {
}

void GraphicsView::dragEnterEvent( QDragEnterEvent *event ) {
  event->acceptProposedAction( );
}

void GraphicsView::dragMoveEvent( QDragMoveEvent *event ) {
  event->acceptProposedAction( );
}

void GraphicsView::dragLeaveEvent( QDragLeaveEvent *event ) {
  event->accept( );
}

void GraphicsView::dropEvent( QDropEvent *event ) {
  const QMimeData *mimeData = event->mimeData( );

  qDebug( ) << "mime: " << mimeData->text( );
  if( mimeData->hasText( ) ) {
    QString file = mimeData->text( ).remove( "file://" ).remove( "\r\n" );
    qDebug( ) << "text: " << file;
    QFileInfo info( file );
    qDebug( ) << "file: " << info.isFile( );
    qDebug( ) << "folder: " << info.isDir( );
    qDebug( ) << "suffix: " << info.completeSuffix( );
    if( objectName( ) == "graphicsViewOutput" ) {
      emit saveFile( event->mimeData( )->text( ) );
      return;
    }
    if( info.isFile( ) ) {
      emit dropImage( file );
    }
    else if( info.isDir( ) ) {
      emit dropFolder( file );
    }
  }
}

void GraphicsView::resizeEvent( QResizeEvent *event ) {
  if( fit ) {
    fitInView( scene( )->itemsBoundingRect( ), Qt::KeepAspectRatio );
  }
  QGraphicsView::resizeEvent( event );
}

bool GraphicsView::getFit( ) const {
  return( fit );
}

void GraphicsView::setFit( bool value ) {
  fit = value;
}
