#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QtGlobal>
#include <QMetaType>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Convex Hull");
    qRegisterMetaType<data_t>("data_t");
    ui->splitter->setSizes(QList<int>() << 100 << 200);
    bool ok1 = connect(ui->genButton, &QPushButton::clicked,
                       ui->openGLWidget, &GLWidget::random_points);
    Q_ASSERT(ok1);
    _worker = new GLWorker();
    _worker->moveToThread(&_thread);
    bool ok2 = connect(&_thread, &QThread::finished, _worker, &QObject::deleteLater);
    Q_ASSERT(ok2);
    bool ok3 = connect(ui->hullButton, &QPushButton::clicked,
                       this, &MainWindow::hull_button_clicked);
    Q_ASSERT(ok3);
    bool ok4 = connect(this, &MainWindow::operate_hull,
                       _worker, &GLWorker::gift_wrapping);
    Q_ASSERT(ok4);
    bool ok5 = connect(_worker, &GLWorker::send_line,
                       ui->openGLWidget, &GLWidget::draw_line);
    Q_ASSERT(ok5);
    _thread.start();

    bool ok6 = connect(ui->openGLWidget, &GLWidget::add_points,
                       this, &MainWindow::emplace_points);
    Q_ASSERT(ok6);
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

void MainWindow::emplace_points(float x, float y)
{
    _counter_clock_wise_hull.emplace_back(x, y);
}

MainWindow::~MainWindow()
{
    _thread.quit();
    _thread.wait();
}
