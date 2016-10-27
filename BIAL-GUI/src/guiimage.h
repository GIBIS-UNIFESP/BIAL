#ifndef GUIIMAGE_H
#define GUIIMAGE_H
#include "Color.hpp"
#include "Geometrics.hpp"
#include "Image.hpp"
#include "MultiImage.hpp"
#include "RealColor.hpp"
#include "Signal.hpp"
#include "displayformat.h"
#include <QObject>
#include <QPixmap>
#include <QVector>

class Tool;

/**
 * @brief The GuiImage class is a bridge to the Bial::Image data structure to the QImage data structure. <br>
 * It have many important functions, such as the views spatial transforms, histogram equalization and others.
 */
class GuiImage : public QObject {
  Q_OBJECT
  /**
   * @brief m_modality is the image modality.
   * @see Modality enum class.
   */
  /* Probably not used anymore. Should be removed soon. */
  Modality m_modality;
  /**
   * @brief size size of the image.
   */
  Bial::MultiImage image;
  /**
   * @brief m_fileName is the image absolute path.
   */
  QString m_fileName;
  /**
   * @brief cachedPixmaps holds a copy of the last generated pixmap at each view.
   */
  QVector< QPixmap > cachedPixmaps;
  /**
   * @brief needUpdate is a flag that determines if a pixmap needs to be updated.
   */
  QVector< bool > needUpdate;
  /**
   * @brief equalization is the equalization transform.
   */
  QVector< int > equalization;
  /**
   * @brief histogram is the input image histogram.
   */
  Bial::Signal histogram;
  /**
   * @brief equalized is the equalized image histogram.
   */
  Bial::Signal equalized;
  /**
   * @brief m_equalizeHistogram determines if the image must be equalized.
   */
  bool m_equalizeHistogram;
  /**
   * @brief transform is the affine transform related to each view.
   */
  QVector< Bial::FastTransform > transform;
  /**
   * @brief bounding is the bounding box that determines each view boundings.
   */
  QVector< Bial::BBox > bounding;
  /**
   * @brief m_currentSlice is the current slice at the z axis of each view.
   */
  QVector< size_t > m_currentSlice;
  /**
   * @brief m_max is the maximum integer value at the input image. Used for int and Color images.
   */
  int m_max;
  /**
   * @brief m_fmax is the maximum float value at the input image. Used for float and RealColor images.
   */
  int m_fmax;
  /**
   * @brief currentToolPos is the current Tool number.
   */
  size_t m_currentToolPos;
  /**
    * @brief contrast level
    */
  int m_contrast;
  /**
    * @brief brightness level
    */
  int m_brightness;
private:
  /**
   * @brief updateBoundings is called each time the transformation matrix is updated. <br>
   * This function applys the affone transform to the input image boundings, and fixes
   * negative positions.
   * @param view
   */
  void updateBoundings( size_t axis );

  public:
  /**
   * @brief tools is a vector containing the image tools.
   */
  QVector< Tool* > tools;
  /**
   * @brief GuiImage is the GuiImage class constructor.
   * @param fName is the absolute path to the input image.
   * @param parent is the QObject parent.
   */
  explicit GuiImage( QString fName, QObject *parent = 0 );
  /**
   * @brief Destructor
   */
  virtual ~GuiImage( );
  /**
   * @brief currentTool returns the current Tool.
   * @return
   */
  Tool* currentTool( );
  /**
   * @brief modality is the image modality getter.
   * @return
   */
  Modality modality( );
  /**
   * @brief fileName is the image absolute path getter.
   * @return
   */
  QString fileName( );
  /**
   * @brief getSlice calculates and returns a QImage with the current slice of the view. <br>
   * @see setCurrentSlice
   * @param view
   * @return
   */
  QPixmap getSlice( size_t view );
  /**
   * @brief width is the view width.
   * @param view
   * @return
   */
  size_t width( size_t view );
  /**
   * @brief heigth is the view heigth.
   * @param view
   * @return
   */
  size_t heigth( size_t view );
  /**
   * @brief depth is the number of slices of the view.
   * @param view
   * @return
   */
  size_t depth( size_t view );
  /**
   * @brief currentSlice is the view's current slice.
   * @param view
   * @return
   */
  size_t currentSlice( size_t view );
  /**
   * @brief currentSlice sets the view's current slice.
   * @param view
   * @param slice
   * @return
   */
  void setCurrentSlice( size_t view, size_t slice );
  /**
   * @brief getPosition transforms the scene position to the input image coordinates.
   * @param pos is the graphicsscene position.
   * @param view is the view number.
   * @return The input image coordinates.
   */
  Bial::Point3D getPosition( QPointF pos, size_t view );
  /**
   * @brief getTransform returns the transform matrix of the views.
   * @param view
   * @return
   */
  Bial::FastTransform getTransform( size_t axis );
  /**
   * @brief getImageType returns image type among int, float, Color and RealColor.
   * @return
   */
  Bial::MultiImageType getImageType( ) const;
  /**
   * @brief get*Image
   * @return a reference to the input image.
   */
  Bial::Image< int > &getIntImage( ) const;
  Bial::Image< float > &getFltImage( ) const;
  Bial::Image< Bial::Color > &getClrImage( ) const;
  Bial::Image< Bial::RealColor > &getRclImage( ) const;

  /**
   * @brief getDims
   * @return the number of dimensions
   */
  size_t getDims( ) const;
  /**
   * @brief getDim
   * @return a vector with bial image dimensions.
   */
  Bial::Vector< size_t > getDim( ) const;
  /**
   * @brief rotateAll90 rotates all views in 90 degrees.
   */
  void rotateAll90( );
  /**
   * @brief rotate90 rotates a view in 90 degrees.
   * @param view View number
   */
  void rotate90( size_t view );
  /**
   * @brief flipH mirrors the current view on X axis.
   * @param view View number
   */
  void flipH( size_t view );
  /**
   * @brief flipV mirrors the current view on Y axis.
   * @param view View number
   */
  void flipV( size_t view );
  /**
   * @brief max is the maximum integer intensity of the input image. Used for int and Color images.
   * @return
   */
  int max( );
  /**
   * @brief fmax is the maximum float intensity of the input image. Used for float and RealColor images.
   * @return
   */
  float fmax( );
  /**
   * @brief getEqualizeHistogram returns a boolean value that says if the image must be equalized or not.
   * @return
   */
  bool getEqualizeHistogram( ) const;
  /**
   * @brief setEqualizeHistogram updates a boolean value that says if the image must be equalized or not.
   * @param equalizeHistogram
   */
  void setEqualizeHistogram( bool equalizeHistogram );
  /**
   * @brief getHistogram returns the image histogram.
   * @return
   */
  const Bial::Signal &getHistogram( ) const;

  /**
   * @brief getPixel returns the pixel intensity of the image at the given position.
   * @param x
   * @param y
   * @param z
   * @return
   */
  int getPixel( int x, int y, int z = 0 );
  size_t currentToolPos( ) const;
  void setCurrentToolPos( const size_t &currentToolPos );

  QPointF getIntersection( size_t view );
  int getContrast() const;
  void setContrast(int contrast);

  int getBrightness() const;
  void setBrightness(int brightness);

  signals:
  /**
   * @brief imageUpdated is called each time a internal property is updated,
   * after that the image views are updated.
   */
  void imageUpdated( );
};

#endif /* GUIIMAGE_H */
