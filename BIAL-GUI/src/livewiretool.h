#ifndef LIVEWIRE_H
#define LIVEWIRE_H

#include "Common.hpp"
#include "lwmethod.h"
#include "tool.h"
#include <QGraphicsItem>

class LiveWireTool : public Tool {

private:
  Bial::Image< int > m_grayImg;
  Bial::Image< int > m_grad;
  Bial::Vector< bool > m_seeds;
  QImage m_res;
  QImage m_cache;

  QVector< QGraphicsEllipseItem* > m_points;
  QVector< int > m_selectedMethods;
  QVector< size_t > m_pointIdxs;
  QVector< std::shared_ptr< LWMethod > > m_methods;

  int m_currentMethod;

  bool m_gradVisible;
  bool m_costVisible;
  QTime timer;
  int circle_radius;
  std::array< QPixmap, 4 > m_pixmaps;
  std::array< bool, 4 > needUpdate;
  Bial::FastTransform m_transf;

  bool m_drawing = false;

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

  void setGradVisibility( bool vis );
  void setCostVisibility( bool vis );

  bool getGradVisible( ) const;
  bool getCostVisible( ) const;

  void addPoint( QPointF pt );

  void updatePath( QPointF pt );

private slots:
  void runLiveWire( int axis );
};

#endif /* LIVEWIRE_H */
