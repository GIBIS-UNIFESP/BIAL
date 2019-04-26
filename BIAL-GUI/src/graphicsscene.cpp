#include "graphicsscene.h"
#include <QPainter>

QPointF GraphicsScene::overlayPos( ) const {
  return( m_overlayPos );
}

GraphicsScene::GraphicsScene( QObject *parent ) : QGraphicsScene( parent ) {
  m_overlay = false;
  m_overlayPen = QPen( Qt::green );
  m_overlayPos = QPointF( -1, -1 );
}

void GraphicsScene::setOverlayPen( const QPen &overlayPen ) {
  m_overlayPen = overlayPen;
  //update( );
}

void GraphicsScene::setOverlayPos( QPointF pos ) {
  m_overlayPos = pos;
  //update( );
}

bool GraphicsScene::overlay( ) const {
  return( m_overlay );
}

void GraphicsScene::setOverlay( bool overlay ) {
  m_overlay = overlay;
  //update( );
}

void GraphicsScene::drawForeground( QPainter *painter, const QRectF &rect ) {
  if( m_overlay ) {
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( m_overlayPen );
    /*    painter->setOpacity( 1.0 ); */
    painter->drawLine( m_overlayPos.x( ), 0, m_overlayPos.x( ), height( ) ); /* vertical */
    painter->drawLine( 0, m_overlayPos.y( ), width( ), m_overlayPos.y( ) ); /* horizontal */
  }
  QGraphicsScene::drawForeground( painter, rect );
}
