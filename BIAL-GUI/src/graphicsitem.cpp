#include <QtWidgets>

#include "graphicsitem.h"

/* GraphicsItem::GraphicsItem(QPixmap pix, QString file) : QGraphicsItem(), pix(pix), file(file) { */
GraphicsItem::GraphicsItem( QString name, QPixmap pix, QFileInfo file ) : QGraphicsObject( ), pix( pix ), file( file ) {
  setCursor( Qt::OpenHandCursor );
  setObjectName( name );
}

QRectF GraphicsItem::boundingRect( ) const {
  return( pix.rect( ) );
}

void GraphicsItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
  Q_UNUSED( option )
  Q_UNUSED( widget )
  painter->drawPixmap( pix.rect( ), pix );
}

void GraphicsItem::mousePressEvent( QGraphicsSceneMouseEvent *event ) {
  Q_UNUSED( event )
  emit Image( pix2, file );
  /*    setCursor(Qt::OpenHandCursor); */
}

void GraphicsItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event ) {
  /*
   *  qDebug() << "move";
   *    setCursor(Qt::ClosedHandCursor);
   *  qDebug() << "press";
   */
  if( QLineF( event->screenPos( ), event->buttonDownScreenPos( Qt::LeftButton ) ).length( ) <
      QApplication::startDragDistance( ) ) {
    return;
  }
  /*  QByteArray itemData; */
  qDebug( ) << objectName( );
  QDrag *drag = new QDrag( event->widget( ) );
  QMimeData *mime = new QMimeData;
  /*  mime->setData("application/x-imagedata", itemData); */
  mime->setText( objectName( ) );
  /*  mime->setImageData(pix); */
  drag->setMimeData( mime );
  drag->setPixmap( pix.scaledToWidth( 100 ) );
  /*    drag->setHotSpot(event->scenePos()); */
  drag->exec( Qt::CopyAction, Qt::CopyAction );
}

void GraphicsItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) {
  Q_UNUSED( event )
  /*  qDebug() << "release"; */
  setCursor( Qt::OpenHandCursor );
}

QPixmap GraphicsItem::getPix2( ) const {
  return( pix2 );
}

void GraphicsItem::setPix2( const QPixmap &value ) {
  pix2 = value;
}

QFileInfo GraphicsItem::fileInfo( ) const {
  return( file );
}

QPixmap GraphicsItem::pixmap( ) const {
  return( pix );
}
