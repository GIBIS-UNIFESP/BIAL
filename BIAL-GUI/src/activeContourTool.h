#ifndef LIVEWIRE_H
#define LIVEWIRE_H

#include "Common.hpp"
#include "activeContourMethod.h"
#include "robotuser.h"
#include "tool.h"
#include <QGraphicsItem>

#define NUM_FTR 10
#define MAX_PTS 1000
typedef Bial::Array< double, NUM_FTR > FeatureData;

class ActiveContourTool : public Tool {

private:
  Bial::Image< int > m_grayImg;
  Bial::Image< int > m_grad;
  QImage m_res;
  QImage m_cache;

  QVector< QGraphicsEllipseItem* > m_points;
  Bial::Vector< int > m_selectedMethods;
  Path m_currentPath;
  QVector< size_t > m_pointIdxs;
  QVector< std::shared_ptr< ActiveContourMethod > > m_methods;
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
  ActiveContourTool( GuiImage *guiImage, ImageViewer *viewer );
  ~ActiveContourTool( );
  static const int supportedFormats = ( ( int ) Modality::BW2D | ( int ) Modality::RGB2D );
  Bial::Image< int > getResult( );
  FeatureData pathDescription( const Path &path, const ActiveContourMethod *method );
  /* Tool interface */
public:
  int type( );
  void clear( );
  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void sliceChanged( size_t axis, size_t slice );
  QPixmap getLabel( size_t axis );

  void addPoint(QPointF pt);

  void updatePath( QPointF pt );

  void finishSegmentation( );

public slots:
  void runLiveWire( );

  // Tool interface
public:
  void enter( );
  void leave( );
  const Bial::Vector< int > &getSelectedMethods( ) const;
  QVector< std::shared_ptr< ActiveContourMethod > > getMethods( ) const;
  Path getCurrentPath( ) const;
  int getCurrentMethod( ) const;
  void setCurrentMethod( int currentMethod );
  QImage getRes( ) const;
  void setRes( const QImage &res );
  QImage getCache( ) const;
  void setCache( const QImage &cache );
};

#endif /* LIVEWIRE_H */
