#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QThread>
#include <utility>

#include "glworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    typedef std::vector<Point> vector_of_points;
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
