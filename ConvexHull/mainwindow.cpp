#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QtGlobal>
#include <QMetaType>
#include <algorithm>
#include <QString>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Convex Hull");
    qRegisterMetaType<data_t>("data_t");
    qRegisterMetaType<size_t>("size_t");
    ui->splitter->setSizes(QList<int>() << 100 << 200);
    bool ok1 = connect(ui->genButton, &QPushButton::clicked,
                       ui->openGLWidget, &GLWidget::random_points);
    _worker = new GLWorker();
    _worker->moveToThread(&_thread);
    bool ok2 = connect(&_thread, &QThread::finished, _worker, &QObject::deleteLater);
    bool ok3 = connect(ui->hullButton, &QPushButton::clicked,
                       this, &MainWindow::hull_button_clicked);
    bool ok4 = connect(this, &MainWindow::operate_hull,
                       _worker, &GLWorker::gift_wrapping);
    bool ok5 = connect(_worker, &GLWorker::send_line,
                       ui->openGLWidget, &GLWidget::draw_line);
    _thread.start();
    bool ok6 = connect(ui->openGLWidget, &GLWidget::add_points,
                       this, &MainWindow::emplace_points);
    bool ok7 = connect(ui->pointsAmount, &QLineEdit::textChanged,
                       this, &MainWindow::pointsChanged);
    bool ok8 = connect(ui->radiusCircle, &QLineEdit::textChanged,
                       this, &MainWindow::radiusChanged);
    bool ok9 = connect(ui->squareButton, &QPushButton::clicked,
                       this, &MainWindow::square_button_clicked);
    bool ok10 = connect(_worker, &GLWorker::send_square,
                        ui->openGLWidget, &GLWidget::get_square_from);
    bool ok11 = connect(ui->grahamButton, &QPushButton::clicked,
                        this, &MainWindow::graham_button_clicked);
    bool ok12 = connect(this, &MainWindow::operate_graham,
                        _worker, &GLWorker::graham);
    bool ok13 = connect(_worker, &GLWorker::send_max_bar,
                        this, &MainWindow::set_max_index_bar);
    bool ok14 = connect(_worker, &GLWorker::increase_bar,
                        this, &MainWindow::update_bar);
    bool ok15 = connect(_worker, &GLWorker::send_bar_value,
                        this, &MainWindow::get_bar_value);
    Q_ASSERT(ok1 && ok2 && ok3 && ok4 && ok5 && ok6 &&
             ok7 && ok8 && ok9 && ok10 && ok11 && ok12 &&
             ok13 && ok14 && ok15);
}

void MainWindow::get_bar_value(size_t val)
{
    ui->progressBar->setValue(static_cast<int>(val));
}

void MainWindow::graham_button_clicked()
{
    std::sort(_counter_clock_wise_hull.begin(),
              _counter_clock_wise_hull.end(),
              [](const std::pair<float, float> &a,
              const std::pair<float, float> &b)
    {
        return a.second < b.second;
    });
    emit operate_graham(_counter_clock_wise_hull);
}

void MainWindow::update_bar(size_t val) {
    ui->progressBar->setValue(ui->progressBar->value() + static_cast<int>(val));
}

void MainWindow::set_max_index_bar(size_t val) {
    ui->progressBar->setMaximum(static_cast<int>(val));
}

void MainWindow::hull_button_clicked()
{
    std::sort(_counter_clock_wise_hull.begin(),
              _counter_clock_wise_hull.end(),
              [](const std::pair<float, float> &a,
              const std::pair<float, float> &b)
    {
        return a.second < b.second;
    });
    emit operate_hull(_counter_clock_wise_hull);
}

void MainWindow::square_button_clicked()
{
    _counter_clock_wise_hull.clear();
    float sq = ui->openGLWidget->get_square();
    QMessageBox msgBox;
    msgBox.setWindowTitle("Square");
    msgBox.setText("Square is: " + QString::number(static_cast<double>(sq)));
    msgBox.exec();
}

void MainWindow::pointsChanged(const QString &text)
{
    bool ok;
    int val = text.toInt(&ok);
    if (ok)
    {
        ui->openGLWidget->set_points(val);
    } else
    {
        qDebug() << "wrong points value";
    }
}

void MainWindow::radiusChanged(const QString &text)
{
    bool ok;
    float val = text.toFloat(&ok);
    if (ok)
    {
        if (val >= 0 && val <= 1)
        {
            ui->openGLWidget->set_radius(val);
        }
    } else
    {
        qDebug() << "wrong radius, it should be float between 0 & 1";
    }
}

void MainWindow::emplace_points(float x, float y)
{
    _counter_clock_wise_hull.emplace_back(x, y);
}

MainWindow::~MainWindow()
{
    _thread.quit();
    _thread.wait();
}
