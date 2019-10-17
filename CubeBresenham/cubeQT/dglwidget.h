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
#include "bmp.h"

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

    void triangle_filled(vec3i t0, vec3i t1, vec3i t2,
                         vec2i b0, vec2i b1, vec2i b2,
                         int colorR, int colorG, int colorB, float alp, BMP* bmp);

    void triangle_filled(vec2i t0, vec2i t1, vec2i t2, QRgb color);

    void dgl_look_at(vec3f eye, vec3f center, vec3f up);

    void dgl_viewport(int x, int y, int w, int h);

    void perspective(const float &angleOfView, const float &aspect, const float &near, const float &far);

    void draw_quad(vec3f v0, vec3f v1, vec3f v2, vec3f v3,
                   vec2f t0, vec2f t1, vec2f t2, vec2f t3,
                   int colorR, int colorG, int colorB, float alp);
    void dgl_rotate(float angle, float x, float y, float z);

public slots:

    void onLineEditTextChanged(const QString &text);

    void update_image();

    void clean_image();

public:

    void cout_matrices();

    void test_cube();

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent* e) override;

private:
    const int FIVE_SECONDS = 5'000;
    int m_width;
    int m_height;
    std::unique_ptr <QImage> m_image;
    Matrix<float> model_view;
    Matrix<float> proj;
    Matrix<float> viewport;
    Matrix<float> MVP;
    Matrix<float> VP;
    float *zbuffer;
    enum proj_mode {ORTHO, PERSP} _mode = PERSP;
    vec3f m_eye{2, 2, 1};
    float near = 0.1f;
    float far = 100.f;
    vec3f m_center{0.0f, 0.0f, 0.0f};
    vec3f m_up{0.0f, 1.0f, 0.0f};
    vec3f m_light_v{-0.88f, -0.531f, 0.44f};
    float eps = 0.000001f;
    float x_VP = 0;
    float y_VP = 0;
    //float xw_VP = m_width;
    //float yh_VP = m_height;
    QTimer m_timer;
    //BMP _bmp{"C:/Users/Timoniche/Desktop/Graphics/CubeBresenham/cubeQT/TallGreenGrass.bmp"};
    //{"C:/Users/Timoniche/Desktop/Graphics/CubeBresenham/cubeQT/one.bmp"}
    BMP* _bmp = nullptr;

    bool mouseDown = false;

    float xrot = 0.0f;
    float yrot = 0.0f;
    float xdiff = 0.0f;
    float ydiff = 0.0f;
public:
    QThread *m_thread = nullptr;
    Worker *m_worker = nullptr;
};

#endif // DGLWIDGET_H
