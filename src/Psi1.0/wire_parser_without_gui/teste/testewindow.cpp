#include "testewindow.h"
#include "ui_testewindow.h"

TesteWindow::TesteWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TesteWindow)
{
    ui->setupUi(this);
}

TesteWindow::~TesteWindow()
{
    delete ui;
}
