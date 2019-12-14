#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QtGlobal>
#include <QMetaType>
#include <algorithm>
#include <QString>
#include <QDebug>
#include <QMessageBox>

#include "triangulation.h"

#include "boost/polygon/voronoi.hpp"
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Convex Hull");
    qRegisterMetaType<vector_of_points>("vector_of_points");
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
    bool ok16 = connect(_worker, &GLWorker::send_vector_lines,
                        ui->openGLWidget, &GLWidget::draw_vector_lines);
    bool ok17 = connect(ui->voronoiButton, &QPushButton::clicked,
                        this, &MainWindow::voronoi_boost);
    bool ok18 = connect(ui->earButton, &QPushButton::clicked,
                        this, &MainWindow::ear_triangulate);
    Q_ASSERT(ok1 && ok2 && ok3 && ok4 && ok5 && ok6 &&
             ok7 && ok8 && ok9 && ok10 && ok11 && ok12 &&
             ok13 && ok14 && ok15 && ok16 && ok17 && ok18);
}

void MainWindow::get_bar_value(size_t val)
{
    ui->progressBar->setValue(static_cast<int>(val));
}

void MainWindow::graham_button_clicked()
{
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
    emit operate_hull(_counter_clock_wise_hull);
}

void MainWindow::square_button_clicked()
{
    _counter_clock_wise_hull.clear();
    float sq = ui->openGLWidget->get_square();
    QMessageBox msgBox;
    msgBox.setWindowTitle("Square");
    msgBox.setText("Square is not supported: " + QString::number(static_cast<double>(sq)));
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

void MainWindow::voronoi_boost()
{
    std::vector<::Vertex> points;
    points.push_back(::Vertex(0, 0));
    points.push_back(::Vertex(1, 6));
    std::vector<Segment> segments;
    segments.push_back(Segment(-4, 5, 5, -1));
    segments.push_back(Segment(3, -11, 13, -1));
    voronoi_diagram<double> vd;
    construct_voronoi(points.begin(), points.end(), segments.begin(), segments.end(), &vd);

}

void MainWindow::ear_triangulate()
{
    using namespace TRIANGULATION;
    using PRIMITIVES::Point;

    if (_counter_clock_wise_hull.size() < 3) return;
    std::function<float(float, float, float, float)> dist_pow2 = [](float x1, float y1, float x2, float y2)
    {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    };

    auto it_to_min_y_element = std::min_element(_counter_clock_wise_hull.begin(),
                                                _counter_clock_wise_hull.end(),
                                                [](const Point &a, const Point &b)
    {
        return a.y < b.y;
    });
    _counter_clock_wise_hull.begin()->swap(*it_to_min_y_element);
    float x0 = _counter_clock_wise_hull[0].x;
    float y0 = _counter_clock_wise_hull[0].y;
    std::sort(_counter_clock_wise_hull.begin() + 1, _counter_clock_wise_hull.end(), [=](const Point &a,
                                                                                                 const Point &b)
    {
        Point at{a.y - y0, a.x - x0};
        Point bt{b.y - y0, b.x - x0};
        float cross = at.x * bt.y - at.y * bt.x;
        if (cross < 1e-7 && cross > -1e-7)
        {
            float dis_a_p0 = dist_pow2(a.x, a.y, x0, y0);
            float dis_b_p0 = dist_pow2(b.x, b.y, x0, y0);
            return dis_a_p0 < dis_b_p0;
        } else
        {
            return cross < 0;
        }
    });

    auto v = triangulate(_counter_clock_wise_hull);
    for (auto& tr : v)
    {
        ui->openGLWidget->draw_line(tr.a.x, tr.a.y, tr.b.x, tr.b.y, 0.f, 1.f, 0.f, 1.f);
        ui->openGLWidget->draw_line(tr.a.x, tr.a.y, tr.c.x, tr.c.y, 0.f, 1.f, 0.f, 1.f);
        ui->openGLWidget->draw_line(tr.b.x, tr.b.y, tr.c.x, tr.c.y, 0.f, 1.f, 0.f, 1.f);
    }
}

MainWindow::~MainWindow()
{
    _thread.quit();
    _thread.wait();
}
