#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <QMetaType>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    bool ok = connect(ui->lineEdit, &QLineEdit::textChanged,
//                      ui->widget, &dglWidget::onLineEditTextChanged);
//    Q_ASSERT(ok);
//    resize(ui->widget->width() + ui->widget->width() + 20,
//           std::max(ui->widget->height(),
//                    ui->widget_2->height()) + 20);
    setWindowTitle("DGL");
    wid = new dglWidget(this);
    //wid->resize(500, 500);
    wid->show();
//    qRegisterMetaType<vec3f>("vec3f");
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    int h = e->size().height();
    int w = e->size().width();
    wid->resize(w, h);
}

MainWindow::~MainWindow()
{
    delete wid;
}
