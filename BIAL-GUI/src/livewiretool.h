#ifndef LIVEWIRE_H
#define LIVEWIRE_H

#include "Common.hpp"
#include "GrowingBucketQueue.hpp"
#include "ImageIFT.hpp"
#include "PathFunction.hpp"
#include "tool.h"

#include <QGraphicsItem>

class LiveWireTool : public Tool {

private:
  Bial::Image< int > m_cost;
  Bial::Image< int > m_pred;
  Bial::Image< int > m_res;
  Bial::Image< int > m_grayImg;
  Bial::Image< int > m_grad;
  Bial::Image< int > m_seeds;

  QVector< QGraphicsEllipseItem* > m_points;

  bool m_predVisible;
  bool m_costVisible;
  QTime timer;
  int circle_radius;
  std::array< QPixmap, 4 > m_pixmaps;
  std::array< bool, 4 > needUpdate;
  Bial::FastTransform m_transf;

  Bial::Point3D toPoint3D( QGraphicsEllipseItem *item );
  size_t toPxIndex( QGraphicsEllipseItem *item );

  Bial::Point3D toPoint3D( const QPointF &qpoint );
  size_t toPxIndex( const QPointF &qpoint );
public:
  enum { Type = 42 };
  QGraphicsScene *m_scene;
  LiveWireTool( GuiImage *guiImage, ImageViewer *viewer );
  ~LiveWireTool( );
  static const int supportedFormats = ( ( int ) Modality::BW2D | ( int ) Modality::RGB2D );

  /* Tool interface */
public:
  int type( );
  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void sliceChanged( size_t axis, size_t slice );
  QPixmap getLabel( size_t axis );

  void setPredVisibility( bool vis );
  void setCostVisibility( bool vis );

  bool getPredVisible( ) const;
  bool getCostVisible( ) const;

  void addPoint( QPointF pt );

  void updatePath( QPointF pt );

private slots:
  void runLiveWire( int axis );
};

#endif /* LIVEWIRE_H */
