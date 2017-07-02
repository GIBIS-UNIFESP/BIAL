#ifndef LIVEWIREWIDGET_H
#define LIVEWIREWIDGET_H

#include "livewiretool.h"

#include <QWidget>

namespace Ui {
  class LiveWireWidget;
}

class LiveWireWidget : public QWidget {
  Q_OBJECT
  Controller * m_controller;
public:
  explicit LiveWireWidget( QWidget *parent = 0 );
  ~LiveWireWidget( );
  void setTool( Tool *sTool );

  Controller* controller( ) const;
  void setController( Controller *controller );

private slots:

  void on_pushButtonClear_clicked( );

  void on_pushButtonRobotUser_clicked( );
  void on_pushButtonSave_clicked( );

  void on_pushButtonProcessAll_clicked( );

private:
  Ui::LiveWireWidget *ui;
  LiveWireTool *tool;
};

#endif /* LIVEWIREWIDGET_H */
