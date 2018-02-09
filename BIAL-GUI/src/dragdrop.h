#ifndef DRAGDROP_H
#define DRAGDROP_H

#include <QFileSystemModel>
#include <QWidget>

#include "graphicsitem.h"
#include "guiimage.h"

namespace Ui {
  class DragDrop;
}

class DragDrop : public QWidget {
  Q_OBJECT

public:
  explicit DragDrop( QWidget *parent = 0 );
  ~DragDrop( );

private slots:
  /*    void on_actionAbrir_imagem_triggered(); */
  void createLabel( const QFileInfo &file, const QWidget *widget );
  void on_pushButtonBatchBW_clicked( );
  void on_pushButtonBatchInvert_clicked( );
  void on_pushButtonBW_clicked( );
  void on_pushButtonInputFolder_clicked( );
  void on_pushButtonInvert_clicked( );
  void on_pushButtonOutputFolder_clicked( );
  void on_treeView_clicked( const QModelIndex &index );
  void on_treeView_doubleClicked( const QModelIndex &index );
  void processBatchBW( const QString &folder );
  void processBW( const QString &text );
  void saveImage( const QString &text );
  void setInputFolder( const QString &path );
  void setOutputFolder( const QString &path );
  void showImage( const QPixmap &pix, const QFileInfo &fileInfo );
  void showImage2( const QPixmap &pix, const QFileInfo &fileInfo );
  void on_pushButtonBW_11_clicked( );
  void on_pushButtonBW_13_clicked( );
  void on_pushButtonBW_7_clicked( );
  void on_pushButtonBW_10_clicked( );
  void on_pushButtonBW_9_clicked( );
  void on_pushButtonBW_8_clicked( );
  void on_pushButtonBW_5_clicked( );
  void on_pushButtonBW_4_clicked( );
  void on_pushButtonBW_3_clicked( );
  void on_pushButtonBW_14_clicked( );
  void on_pushButtonBW_15_clicked( );
  void on_pushButtonBW_16_clicked( );
  void on_pushButtonBW_17_clicked( );
  void on_pushButtonBW_18_clicked( );
  void on_pushButtonBW_19_clicked( );
  void on_pushButtonBW_20_clicked( );
  void on_pushButtonBW_21_clicked( );
  void on_pushButtonBW_22_clicked( );
  void on_pushButtonBW_23_clicked( );
  void on_pushButtonBW_24_clicked( );
  void on_pushButtonBW_25_clicked( );
  void on_pushButtonBW_26_clicked( );
  void on_pushButtonBW_27_clicked( );
  void on_pushButtonBW_28_clicked( );
  void on_pushButtonBW_29_clicked( );
  void on_pushButtonBW_30_clicked( );
  void on_pushButtonBW_31_clicked( );
  void on_pushButtonBW_32_clicked( );
  void on_pushButtonBW_33_clicked( );
  void on_pushButtonBW_34_clicked( );
  void on_pushButtonBW_35_clicked( );
  void on_pushButtonBW_37_clicked( );
  void on_pushButtonBW_38_clicked( );
  void on_pushButtonBW_39_clicked( );
  void on_pushButtonBW_40_clicked( );
  void on_groupBoxSaida_toggled( bool checked );

private:
  Ui::DragDrop *ui;
  QString inputFolder;
  QString outputFolder;
  GraphicsItem *item = nullptr;
  GraphicsItem *item2 = nullptr;
  GuiImage *m_img = nullptr;
  GuiImage *m_img2 = nullptr;
  QFileSystemModel *model;
  /*  */
  void loadFolderThumbs( QString folder, QWidget *widget );
  void processInvert( const QString &text );
  void loadInputFolderThumbs( );
  void loadOutputFolderThumbs( );
  Bial::Image< Bial::Color > rotateBial( );
  Bial::Image< int > AnisotropicBial( );
  Bial::Image< int > CannyBial( );
  Bial::Image< int > image_roi( );
  Bial::Image< int > transform_euclidean( );
  Bial::Image< Bial::Color > image_multiply_mask( );
  Bial::Image< int > image_multiply( );
  Bial::Image< int > image_minimum_spanning_forest( );
  Bial::Image< int > image_merge( );
  void adjacency_displacement( );
  void adjancency_gray( );
  void bit_invert( );
  void bit_operations( );
  Bial::Image< int > brain_cmeansclustering( );
  Bial::Image< int > brain_inhomogeneity_corretion( );
  Bial::Image< int > brain_opfclustering( );
  Bial::Image< float > brain_segmentation( );
  void brain_split_opf_clustering( );
  Bial::Image< int > color_cmeans_clustering( );
  Bial::Image< int > filtering_adaptive_anisotropic( );
  Bial::Image< int > filtering_anisotropic( );
  Bial::Image< int > filtering_gaussian( );
  Bial::Image<int> filtering_mean( );
  Bial::Image<int> filtering_median( );
  Bial::Image< int > filtering_optimum_anisotropic( );
  void geometrics_2d( );
  void geometrics_3d( );
  Bial::Image< int > gradient_canny( );
  Bial::Image< int > gradient_directional_sobel( );
  Bial::Image< int > gradient_gabor( );
  Bial::Image< int > gradient_hole_closing( );
  Bial::Image<int> gradient_morphological( );
  Bial::Image< int > gradient_multiscalecanny( );
  Bial::Image< int > gradient_multi_scale_size_canny( );
  Bial::Image< int > gradient_multi_sub_scale_canny( );
  Bial::Image< int > gradient_scalecanny( );
  Bial::Image< int > gradient_sobel( );
  Bial::Image< int > gradient_supressed_sobel( );
  Bial::Image< float > heart_cog( );
  Bial::Image< float > heart_segmentation( );
  void hough_circles( );
  void image_and( );
  Bial::Image< int > image_binary_mask( );
  void image_cmeans_clustering( );
  Bial::Image< int > image_color_binary_mask( );
  void image_compare( );
  void image_copy( );
  Bial::Image< uchar > image_convert( );
  Bial::Image<int> image_dilate( );
  Bial::Image< int > image_equalize( );
  Bial::Image< int > image_erode( );
  void image_getslice( );
  Bial::Image< int > image_ift_watershed( );
};

#endif /* DRAGDROP_H */
