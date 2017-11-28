#ifndef SEGMENTATIONWIDGET_H
#define SEGMENTATIONWIDGET_H

#include "segmentationtool.h"

#include <QWidget>

namespace Ui {
  class Segmentationwidget;
}

class SegmentationWidget : public QWidget {
  Q_OBJECT

public:
  explicit SegmentationWidget( QWidget *parent = 0 );
  ~SegmentationWidget( );
  void setTool( Tool *sTool );

private slots:

  void on_SegmentationButton_clicked( );
  void on_eraserButton_clicked( );
  void on_drawButton_clicked( );
  void on_ClearButton_clicked( );

  void on_AlphaSpinBox_valueChanged( double arg1 );

  void on_BetaSpinBox_valueChanged( double arg1 );

  void on_pushButtonShowSeeds_clicked( bool checked );

  void on_pushButtonShowMask_clicked( bool checked );

  void on_thickDoubleSpinBox_valueChanged( double arg1 );

  void on_pfmaxgeo_toggled( bool checked );

  void on_pfmax_toggled( bool checked );

  void on_pfsum_toggled( bool checked );

  void on_morphological_grad_toggled( bool checked );

  void on_sobel_grad_toggled( bool checked );

  void on_pushButtonSave_clicked( );

  void on_pushButtonSaveSeeds_clicked( );

private:
  Ui::Segmentationwidget *ui;
  SegmentationTool *tool;
};

#endif /* SEGMENTATIONWIDGET_H */
