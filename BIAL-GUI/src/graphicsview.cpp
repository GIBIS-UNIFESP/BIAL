#include "graphicsview.h"
#include "Common.hpp"

#include <QApplication>
#include <QDebug>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QScrollBar>

GraphicsView::GraphicsView( QWidget *parent ) : QGraphicsView( parent ) {
  COMMENT( "Here", 0 );
  _pan = false;
  _space = false;
  _panStartX = 0;
  _panStartY = 0;
  m_gvzoom = new GraphicsViewZoom( this );
}


void GraphicsView::mousePressEvent( QMouseEvent *e ) {
  if( e->button( ) == Qt::MiddleButton ) {
    _pan = true;
    _panStartX = e->x( );
    _panStartY = e->y( );
    QApplication::setOverrideCursor( Qt::ClosedHandCursor );
    e->accept( );
    return;
  }
  QGraphicsView::mousePressEvent( e );
}

void GraphicsView::mouseReleaseEvent( QMouseEvent *e ) {
  if( e->button( ) == Qt::MiddleButton ) {
    _pan = false;
    QApplication::restoreOverrideCursor( );
    e->accept( );
    return;
  }
  QGraphicsView::mouseReleaseEvent( e );
}

void GraphicsView::mouseMoveEvent( QMouseEvent *e ) {
  if( _pan || _space ) {
    horizontalScrollBar( )->setValue( horizontalScrollBar( )->value( ) - ( e->x( ) - _panStartX ) );
    verticalScrollBar( )->setValue( verticalScrollBar( )->value( ) - ( e->y( ) - _panStartY ) );
    _panStartX = e->x( );
    _panStartY = e->y( );
    e->accept( );
    return;
  }
  _panStartX = e->x( );
  _panStartY = e->y( );
  QGraphicsView::mouseMoveEvent( e );
}

void GraphicsView::keyPressEvent( QKeyEvent *e ) {
  if( e->key( ) == Qt::Key_Space ) {
    _space = true;
    QApplication::setOverrideCursor( Qt::ClosedHandCursor );
    e->accept( );
  }
  QGraphicsView::keyPressEvent( e );
}


void GraphicsView::keyReleaseEvent( QKeyEvent *e ) {
  if( e->key( ) == Qt::Key_Space ) {
    _space = false;
    QApplication::restoreOverrideCursor( );
    e->accept( );
  }
  QGraphicsView::keyReleaseEvent( e );
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
