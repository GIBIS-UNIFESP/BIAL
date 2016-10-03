#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QFileInfo>
#include <QGraphicsItem>

/* class GraphicsItem : public QGraphicsItem { */
class GraphicsItem : public QGraphicsObject {
  Q_OBJECT

public:
  GraphicsItem( QString name, QPixmap pix, QFileInfo file );
  virtual QRectF boundingRect( ) const Q_DECL_OVERRIDE;
  virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) Q_DECL_OVERRIDE;
  QPixmap pixmap( ) const;
  QFileInfo fileInfo( ) const;
  void setPix2( const QPixmap &value );
  QPixmap getPix2( ) const;

signals:
  void Image( const QPixmap &, const QFileInfo & );

protected:
  virtual void mousePressEvent( QGraphicsSceneMouseEvent *event ) Q_DECL_OVERRIDE;
  virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event ) Q_DECL_OVERRIDE;
  virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event ) Q_DECL_OVERRIDE;

private:
  QPixmap pix;
  QPixmap pix2;
  QFileInfo file;
};

#endif /* GRAPHICSITEM_H */
