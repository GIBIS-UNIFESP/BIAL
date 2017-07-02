#ifndef LIVEWIRE_H
#define LIVEWIRE_H

#include "Common.hpp"
#include "lwmethod.h"
#include "robotuser.h"
#include "tool.h"
#include <QGraphicsItem>

#define NUM_FTR 10
#define MAX_PTS 1000
typedef Bial::Array< double, NUM_FTR > FeatureData;

class LiveWireTool : public Tool {
  friend class RobotUser;

private:
  Bial::Image< int > m_grayImg;
  Bial::Image< int > m_grad;
  Bial::Vector< bool > m_seeds;
  QImage m_res;
  QImage m_cache;

  QVector< QGraphicsEllipseItem* > m_points;
  QVector< int > m_selectedMethods;
  Path m_currentPath;
  QVector< size_t > m_pointIdxs;
  QVector< std::shared_ptr< LWMethod > > m_methods;
  QPointF m_lastPoint;

  int m_currentMethod;

  QTime timer;
  std::array< QPixmap, 4 > m_pixmaps;
  std::array< bool, 4 > needUpdate;
  Bial::FastTransform m_transf;

  bool m_drawing = false;
  bool m_finished = false;

  Bial::Point3D toPoint3D( QGraphicsEllipseItem *item );
  size_t toPxIndex( QGraphicsEllipseItem *item );

  Bial::Point3D toPoint3D( const QPointF &qpoint );
  size_t toPxIndex( const QPointF &qpoint );
  const Bial::Vector< Bial::Point3D > toPoint3DVector( const Path &path );
  Bial::Vector< double > calcHistogram( const Path &path, const Bial::Image< int > &img, size_t bins );
public:
  enum { Type = 42 };
  QGraphicsScene *m_scene;
  LiveWireTool( GuiImage *guiImage, ImageViewer *viewer );
  ~LiveWireTool( );
  static const int supportedFormats = ( ( int ) Modality::BW2D | ( int ) Modality::RGB2D );
  Bial::Image< int > getResult( );
  FeatureData pathDescription( const Path &path, const LWMethod *method );
  /* Tool interface */
public:
  int type( );
  void clear( );
  void roboto( );
  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void sliceChanged( size_t axis, size_t slice );
  QPixmap getLabel( size_t axis );

  void addPoint( QPointF pt );

  void updatePath( QPointF pt );

  void finishSegmentation( );

private slots:
  void runLiveWire( );

  // Tool interface
public:
  void enter( );
  void leave( );
  const QVector< int > &getSelectedMethods( ) const;
  QVector< std::shared_ptr< LWMethod > > getMethods( ) const;
};

#endif /* LIVEWIRE_H */
