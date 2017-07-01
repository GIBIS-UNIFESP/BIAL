#include "livewirewidget.h"
#include "ui_livewirewidget.h"

LiveWireWidget::LiveWireWidget( QWidget *parent ) :
  QWidget( parent ),
  ui( new Ui::LiveWireWidget ) {
  ui->setupUi( this );

  ui->pushButtonSave->hide( );
}

LiveWireWidget::~LiveWireWidget( ) {
  delete ui;
}

void LiveWireWidget::setTool( Tool *sTool ) {
  tool = dynamic_cast< LiveWireTool* >( sTool );
  setEnabled( tool != nullptr );
}

void LiveWireWidget::on_pushButtonClear_clicked( ) {
  tool->clear( );
}

void LiveWireWidget::on_pushButtonRobotUser_clicked( ) {
  tool->roboto( );
}
