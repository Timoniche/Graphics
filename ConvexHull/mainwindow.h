#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QThread>
#include <utility>

#include "glworker.h"
#include "hulllib.h"

#include "boost/polygon/voronoi.hpp"
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;
namespace Ui {
class MainWindow;
}

struct Vertex {
  int a;
  int b;
  Vertex (int x, int y) : a(x), b(y) {}
};

struct Segment {
  ::Vertex p0;
  ::Vertex p1;
  Segment (int x1, int y1, int x2, int y2) : p0(x1, y1), p1(x2, y2) {}
};

namespace boost {
namespace polygon {

template <>
struct geometry_concept<::Vertex> {
  typedef point_concept type;
};

template <>
struct point_traits<::Vertex> {
  typedef int coordinate_type;

  static inline coordinate_type get(
      const ::Vertex& point, orientation_2d orient) {
    return (orient == HORIZONTAL) ? point.a : point.b;
  }
};

template <>
struct geometry_concept<Segment> {
  typedef segment_concept type;
};

template <>
struct segment_traits<Segment> {
  typedef int coordinate_type;
  typedef ::Vertex point_type;

  static inline point_type get(const Segment& segment, direction_1d dir) {
    return dir.to_int() ? segment.p1 : segment.p0;
  }
};
}  // polygon
}  // boost

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    typedef std::vector<PRIMITIVES::Point> vector_of_points;
public slots:
    void hull_button_clicked();
    void graham_button_clicked();
    void square_button_clicked();
    void emplace_points(float, float);
    void pointsChanged(const QString &);
    void radiusChanged(const QString &);
    void update_bar(size_t);
    void set_max_index_bar(size_t);
    void get_bar_value(size_t);

    void voronoi_boost();
    void ear_triangulate();
signals:
    void operate_hull(vector_of_points const&);
    void operate_graham(vector_of_points const&);
private:
    std::unique_ptr<Ui::MainWindow> ui;
    QThread _thread;
    GLWorker* _worker = nullptr;
    vector_of_points _counter_clock_wise_hull;
};

#endif // MAINWINDOW_H
