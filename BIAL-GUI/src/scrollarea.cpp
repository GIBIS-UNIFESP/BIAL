#include <QDebug>
#include <QDropEvent>
#include <QMimeData>

#include "scrollarea.h"

ScrollArea::ScrollArea( QWidget *parent ) : QScrollArea( parent ) {
}

void ScrollArea::dragEnterEvent( QDragEnterEvent *event ) {
  qDebug( ) << "enter event: " << event;
  event->accept( );
}

void ScrollArea::dragMoveEvent( QDragMoveEvent *event ) {
  Q_UNUSED( event )
  /*    qDebug() << "move event: " << event; */
}

void ScrollArea::dragLeaveEvent( QDragLeaveEvent *event ) {
  Q_UNUSED( event )
  /*    qDebug() << "leave event: " << event; */
}

void ScrollArea::dropEvent( QDropEvent *event ) {
  qDebug( ) << "drop event: " << event;
  /*
   *  if (event->mimeData()->hasImage()) {
   *    emit pixmap(QPixmap::fromImage(qvariant_cast<QImage>(event->mimeData()->imageData())));
   *  }
   */
  if( event->mimeData( )->hasText( ) ) {
    emit item( event->mimeData( )->text( ) );
  }
}
