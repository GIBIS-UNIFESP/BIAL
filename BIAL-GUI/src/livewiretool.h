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
  Bial::Image< int > pred;
  Bial::Image< int > cost;

  QVector< QGraphicsEllipseItem* > m_points;

  bool predVisible;
  bool costVisible;
  QTime timer;
  int circle_radius;
  std::array< QPixmap, 4 > pixmaps;
  std::array< bool, 4 > needUpdate;

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

private slots:
  void runLiveWire( int axis, const Bial::FastTransform &transf );
};

#endif /* LIVEWIRE_H */
