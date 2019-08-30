#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(ui->widget->width() + 20, ui->widget->height() + 20);
    setWindowTitle("DGL");
}

MainWindow::~MainWindow()
{
}
