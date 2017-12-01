#ifndef SEGMENTATIONTOOL_H
#define SEGMENTATIONTOOL_H

#include "Common.hpp"
#include "GrowingBucketQueue.hpp"
#include "ImageIFT.hpp"
#include "PathFunction.hpp"
#include "tool.h"

class SegmentationTool : public Tool {
private:
  Bial::Image< int > seeds;
  Bial::Image< int > mask;

  // Aditional objects required to run IFT from scratch. Used for DIFT.
  Bial::MultiImage grad;
  Bial::MultiImage cost[ 2 ];
  Bial::Image< int > pred[ 2 ];
  Bial::Image< int > label[ 2 ];
  Bial::PathFunction< Bial::Image, int > *int_path_func[ 2 ];
  Bial::PathFunction< Bial::Image, float > *flt_path_func[ 2 ];
  Bial::Adjacency adj;
  Bial::GrowingBucketQueue *queue;
  Bial::ImageIFT< int > *int_ift[ 2 ];
  Bial::ImageIFT< float > *flt_ift[ 2 ];
  double curr_alpha;
  double curr_beta;

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
  bool initiated;
  int pf; // Path function used during last execution. -1: none; 0: geosum; 1:max; 2:sum same order of radio buttons.
  int grad_type; //Type of gradient: -1: none; 0: morphological; 1: sobel.

public:
  enum { Type = 1 };
  SegmentationTool( GuiImage *guiImage, ImageViewer *viewer );
  ~SegmentationTool( );
  static const int supportedFormats = ( ( int ) Modality::BW2D | ( int ) Modality::BW3D | ( int ) Modality::RGB2D );

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

  bool hasMask( );

  template< class D >
  void InitiateSeeds( size_t map_set,
                      const Bial::Vector< size_t > &obj_seeds,
                      const Bial::Vector< size_t > &bkg_seeds,
                      Bial::Image< D > &grad );
  void GeodesicSum( Bial::Image< int > &img,
                    const Bial::Vector< size_t > &obj_seeds,
                    const Bial::Vector< size_t > &bkg_seeds,
                    float alpha,
                    float beta );
  void GeodesicSum( Bial::Image< float > &img,
                    const Bial::Vector< size_t > &obj_seeds,
                    const Bial::Vector< size_t > &bkg_seeds,
                    float alpha,
                    float beta );
  void Watershed( Bial::Image< int > &img,
                  const Bial::Vector< size_t > &obj_seeds,
                  const Bial::Vector< size_t > &bkg_seeds );
  void Watershed( Bial::Image< float > &img,
                  const Bial::Vector< size_t > &obj_seeds,
                  const Bial::Vector< size_t > &bkg_seeds );
  void FSum( Bial::Image< int > &img, const Bial::Vector< size_t > &obj_seeds,
             const Bial::Vector< size_t > &bkg_seeds );
  void FSum( Bial::Image< float > &img, const Bial::Vector< size_t > &obj_seeds,
             const Bial::Vector< size_t > &bkg_seeds );

  void MorphologicalGradient( );
  void SobelGradient( );

  /* pf_type: 0-maxgeo, 1-max, 2-sum */
  Bial::Image< int > connect( int pf_type, double alpha, double beta );

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

  bool isInitiated( ) const;

  Bial::Image< int > getMask( ) const;
  Bial::Image< int > getSeeds( ) const;
};

#endif /* SEGMENTATIONTOOL_H */
