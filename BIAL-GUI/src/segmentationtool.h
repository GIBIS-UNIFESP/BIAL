#ifndef SEGMENTATIONTOOL_H
#define SEGMENTATIONTOOL_H

#include "Common.hpp"
#include "tool.h"

class SegmentationTool : public Tool {
private:
  Bial::Image< int > seeds;
  Bial::Image< int > mask;

  Bial::Point3D lastPoint;
  bool drawing;
  int drawType;
  double alpha;
  double beta;
  bool seedsVisible;
  bool maskVisible;
  QTime timer;
  int thickness;
  std::array< QPixmap, 4 > pixmaps;
  std::array< bool, 4 > needUpdate;

public:
  enum { Type = 1 };
  SegmentationTool( GuiImage *guiImage, ImageViewer *viewer );
  static const int supportedFormats = ( ( int ) Modality::BW2D | ( int ) Modality::BW3D );

  /* Tool interface */
public:
  int type( );
  void mouseReleased( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseClicked( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseDragged( QPointF pt, Qt::MouseButtons buttons, size_t axis );
  void mouseMoved( QPointF pt, size_t axis );
  void sliceChanged( size_t axis, size_t slice );
  QPixmap getLabel( size_t axis );

  void drawSeed( Bial::Point3D last, Bial::Point3D actual );
  void setDrawType( int type );
  void clearSeeds( );
  /* pf_type: 0-maxgeo, 1-max, 2-sum */
  Bial::Image< int > segmentationOGS( int pf_type, double alpha, double beta );

  double getAlpha( ) const;
  void setAlpha( double value );
  double getBeta( ) const;
  void setBeta( double value );
  int getDrawType( ) const;

  void setSeedsVisibility( bool vis );
  void setMaskVisibility( bool vis );

  bool getSeedsVisible( ) const;
  bool getMaskVisible( ) const;
  void setThickness( int value );

};

#endif /* SEGMENTATIONTOOL_H */
