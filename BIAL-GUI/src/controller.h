#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "guiimage.h"
#include "tool.h"

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QVector>

class ThumbsWidget;

/**
 * @brief The Controller class is one of the most important classes of the User Interface,
 * and is responsible to manage the images.
 */
class Controller : public QObject {
  Q_OBJECT

  DisplayFormat * bw2dFormat;
  DisplayFormat *rgb2dFormat;
  DisplayFormat *bw3dFormat;
  /**
   *
   * @brief m_images holds all opened images.
   *
   */
  QVector< GuiImage* > m_images;
  /**
   *
   * @brief m_pixmapItems holds all pixmapLabelItems.
   *        This class holds two pixmaps, one for the current image,
   *        and one for the current label.
   *
   */
  QVector< QGraphicsPixmapItem* > m_pixmapItems;
  QVector< QGraphicsPixmapItem* > m_labelItems;
  /**
   *
   * @brief m_currentImagePos holds the current image position.
   *
   */
  int m_currentImagePos;
  /**
   * @brief m_thumbsWidget is a pointer to the thumbnails dock.
   */
  ThumbsWidget *m_thumbsWidget;
  /**
   * @brief scale
   */
  double scale;

public:
  enum { MaxRecentFiles = 10 };
  /**
   *
   * @brief Controller's constructor
   * @param views is the number of the views of the imageViewer.
   * @param parent is the parent object.
   *
   */
  explicit Controller( int views, QObject *parent = 0 );
  /**
   *
   * @brief currentImage
   * @return A pointer to the current guiimage.
   *
   */
  GuiImage* currentImage( );
  /**
   *
   * @brief currentImage
   * @return A pointer to the guiimage at position pos.
   *
   */
  GuiImage* imageAt( int pos );
  /**
   *
   * @brief currentImagePos
   * @return The current image position
   *
   */
  int currentImagePos( ) const;
  /**
   *
   * @brief addImage Adds an image to vector m_images.
   * @param fname is the file name of the Image to be opened.
   *
   */
  bool addImage( QString fname );
  /**
   *
   * @brief removeCurrentLabel removes the current label from current image.
   * @return true if removed successfully.
   *
   */
  bool removeCurrentLabel( );
  /**
   *
   * @brief removeCurrentImage removes the current image from vector.
   *
   */
  void removeCurrentImage( );
  /**
   *
   * @brief isEmpty
   * @return true the vector is empty
   *
   */
  bool isEmpty( );
  /**
   *
   * @brief clear Clears the image vector, and resets thumbnails.
   *
   */
  void clear( );
  /**
   *
   * @brief size
   * @return The image vector size.
   *
   */
  int size( );
  /**
   * @brief setThumbsWidget setThumbsWidget sets the pointer to the thumbnails dock.
   * @param thumbsWidget
   */
  void setThumbsWidget( ThumbsWidget *thumbsWidget );
  /**
   * @brief currentFormat returns the modality of current image.
   */
  DisplayFormat* currentFormat( );
  /**
   * @brief getPixmapItem returns the PixmapItem of the view.
   * @param view is the number of the view;
   * @return
   */
  QGraphicsPixmapItem* getPixmapItem( size_t view );
  /**
   * @brief getLabelItem returns the LabelItem of the view.
   * @param view is the number of the view;
   * @return
   */
  QGraphicsPixmapItem* getLabelItem( size_t view );

signals:
  /**
   * @brief This signal is emmited every time the current image changes.
   */
  void currentImageChanged( );
  /**
   * @brief This signal is emmited every time the current image is updated.
   */
  void imageUpdated( );
  /**
   * @brief This signal is emmited avery time the m_images vector is updated.
   */
  void containerUpdated( );
  /**
   * @brief recentFilesUpdated
   */
  void recentFilesUpdated( );
public slots:
  /**
   *
   * @brief update updates the image pixmaps.
   *
   */
  void update( );
  /**
   *
   * @brief setCurrentImagePos
   * @param position is the position of the image in the images vector.
   *
   */
  void setCurrentImagePos( int position );
  /**
   *
   * @brief loadNextImage is a slot called from controlsdock
   * that loads the next image ( like an circular list ).
   *
   */
  void loadNextImage( );
  /**
   * @brief setCurrentSlice is called by the imageViewer when the slider or
   *  the spinbox have theis values updated.
   * @param view
   * @param slice
   */
  void setCurrentSlice( size_t view, size_t slice );
  /**
   * @brief setZoom updates the zoom factor.
   * @param value
   */
  void setZoom( int value );
  /**
   * @brief setInterpolation switches between smoot and fast interpolation.
   * @param isSmooth
   */
  void setInterpolation( bool isSmooth );
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

private:
  /**
   * @brief setRecentFile
   * @param file
   */
  void setRecentFile( QString fname );
};

#endif /** CONTROLLER_H */
