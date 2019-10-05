#ifndef DGLWIDGET_H
#define DGLWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <memory>
#include <vector>
#include <QMoveEvent>
#include <QPoint>
#include <QWheelEvent>
#include <QString>
#include <QThread>
#include <QTimer>
//#include <future>
//#include <condition_variable>
#include <thread>

#include "dglgeometry.h"
#include "matrix.h"
#include "worker.h"

using namespace DGL;

class dglWidget : public QWidget
{
Q_OBJECT

public:
    explicit dglWidget(QWidget *parent = nullptr);

    ~dglWidget() override;

signals:

public slots:

    //todo: put in separate dll
    //todo: replace with Strassen multiplication
    void set_pixel(int x, int y, QRgb color);

    void draw_line(int x0, int y0, int x1, int y1, QRgb color);

    void draw_line(vec2i t0, vec2i t1, QRgb color);

    void triangle(vec2i t0, vec2i t1, vec2i t2, QRgb color);

    void triangle_filled(vec3i t0, vec3i t1, vec3i t2, int colorR, int colorG, int colorB, float alp );

    void triangle_filled(vec2i t0, vec2i t1, vec2i t2, QRgb color);

    void dgl_look_at(vec3f eye, vec3f center, vec3f up);

    void dgl_viewport(int x, int y, int w, int h);

    void perspective(const float &angleOfView, const float &aspect, const float &near, const float &far);

    void draw_quad(vec3f v0, vec3f v1, vec3f v2, vec3f v3, int colorR, int colorG, int colorB, float alp);

public slots:

    void onLineEditTextChanged(const QString& text);

    void update_image();

    void clean_image();

public:

    void cout_matrices();

    void test_cube();

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

private:
    const int FIVE_SECONDS = 5'000;
//    const int m_width = 1 << 10;
//    const int m_height = 1 << 10;
    const int m_width = 500;
    const int m_height = 500;
    std::unique_ptr<QImage> m_image;
    Matrix<float> model_view;
    Matrix<float> proj;
    Matrix<float> viewport;
    Matrix<float> MVP;
    Matrix<float> VP;
    float *zbuffer;
    vec3f m_eye{2.0f, 3.0f, 5.0f};
    float near = 0.1f;
    float far = 100.f;
    vec3f m_center{.2f, .3f, .1f};
    vec3f m_up{0.0f, 1.0f, 0.0f};
    vec3f m_light_v{-0.88f, -0.531f, 0.44f}; //todo normalize;
    float eps = 0.0001f;
    float x_VP = 0;
    float y_VP = 0;
    float xw_VP = m_width;
    float yh_VP = m_height;
    QPoint last_pos{};
    QTimer m_timer;
public:
    QThread *m_thread = nullptr;
    Worker *m_worker = nullptr;
};

#endif // DGLWIDGET_H
