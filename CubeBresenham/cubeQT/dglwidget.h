#ifndef DGLWIDGET_H
#define DGLWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <memory>
#include <vector>

#include "dglgeometry.h"
#include "matrix.h"

using namespace DGL;

class dglWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dglWidget(QWidget *parent = nullptr);
    ~dglWidget();

signals:

public slots:
    //todo: put in separate dll
    //todo: replace with Strassen multiplication
    void set_pixel(int x, int y, QRgb color);
    void draw_line(int x0, int y0, int x1, int y1, QRgb color);
    void draw_line(vec2i t0, vec2i t1, QRgb color);
    void triangle(vec2i t0, vec2i t1, vec2i t2, QRgb color);
    void triangle_filled(vec3i t0, vec3i t1, vec3i t2, QRgb color);
    void triangle_filled(vec2i t0, vec2i t1, vec2i t2, QRgb color);
    void dgl_look_at(vec3f eye, vec3f center, vec3f up);
    Matrix<float> dgl_viewport(int x, int y, int w, int h);
    Matrix<float> dgl_projection(float c);
    Matrix<float> perspective
    (
            float const & fovy,
            float const & aspect,
            float const & zNear,
            float const & zFar
            );

protected:
    void paintEvent(QPaintEvent* event);

private:
    const int m_width = 1 << 10;
    const int m_height = 1 << 9;
    const int m_depth = 255;
    std::unique_ptr<QImage> m_image;
    Matrix<float>* model_view = nullptr;
    int* zbuffer;

    vec3i z0[3] = {vec3i(-3, 3, 1),   vec3i(3, 3, 1),  vec3i(0, -3, 1)};
    vec3i z1[3] = {vec3i(50, 200, 1),   vec3i(350, 100, 4),  vec3i(150, 300, -1)};

    vec3f m_eye{0, 0, -2};
    float near = 1.0f;
    float far = 10.f;
    vec3f m_center{0, 0, 0};
    vec3f m_up{0.0f, 1.0f, 0.0f};
};

#endif // DGLWIDGET_H
