#include <QDebug>
#include <QDropEvent>
#include <QFileInfo>
#include <QMessageBox>
#include <QMimeData>

#include "pushbutton.h"

PushButton::PushButton( QWidget *parent ) : QPushButton( parent ) {
}

PushButton::~PushButton( ) {
}

void PushButton::dropEvent( QDropEvent *event ) {
  /*    qDebug() << event; */
  qDebug( ) << "drop event " << event;
  qDebug( ) << "text?" << event->mimeData( )->text( );
  /*  if (not event->mimeData()->hasText()) return; */
  if( event->mimeData( )->hasText( ) ) {
    QString text = event->mimeData( )->text( );
    if( text == "item" or text == "item2" ) {
      emit item( text );
    }
    QString filePath = text.remove( "file:///" );

    QFileInfo info( filePath );
    if( info.isFile( ) ) {
      emit pixmap( QPixmap( filePath ) );
    }
    if( info.isDir( ) ) {
      emit file( filePath );
    }
    return;
  }
}

void PushButton::dragEnterEvent( QDragEnterEvent *event ) {
  /*  qDebug() << "drag enter"; */
  event->accept( );
}

void PushButton::dragMoveEvent( QDragMoveEvent *event ) {
  Q_UNUSED( event )
  /*  qDebug() << "drag move"; */
}

void PushButton::dragLeaveEvent( QDragLeaveEvent *event ) {
  Q_UNUSED( event )
  /*  qDebug() << "drag leave"; */
}
