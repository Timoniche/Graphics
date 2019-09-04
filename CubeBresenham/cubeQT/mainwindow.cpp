#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(ui->widget->width(), ui->widget->height());
    setWindowTitle("DGL");
}

MainWindow::~MainWindow()
{
}
