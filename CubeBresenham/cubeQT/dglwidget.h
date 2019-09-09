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
    void dgl_viewport(int x, int y, int w, int h);
    Matrix<float> dgl_projection(float c);
    Matrix<float> perspective
    (
            float const & fovy,
            float const & aspect,
            float const & zNear,
            float const & zFar
            );
    void projj(const float &angleOfView, const float &near, const float &far);

    void draw_quad(vec3f* v, QRgb color);
public:
    void test_draw1();
    void test_draw2(vec3f*);
    void test_draw3();
protected:
    void paintEvent(QPaintEvent* event);

private:
    const int m_width = 1 << 10;
    const int m_height = 1 << 10;
    const int m_depth = 255;
    std::unique_ptr<QImage> m_image;
    Matrix<float> model_view;
    Matrix<float> proj;
    Matrix<float> viewport;
    Matrix<float> MVP;
    float* zbuffer;
    vec3f m_eye{10.0f, 10.0f, 10.f};
    float near = 0.1f;
    float far = 100.f;
    vec3f m_center{0., 0., 0.};
    vec3f m_up{0.0f, 1.0f, 0.0f};
    vec3f m_light{0.88f, -0.11f, 0.44f};
    float eps = 0.001f;
    //test draw obj
private:
    vec3f tr1[3] = {vec3f{-0.000581696f, -0.734665f, -0.623267f},
                    vec3f{-0.511812f, -0.845392f, 0.127809f},
                    vec3f{1.f, -0.718624f, -0.109224f}};
    vec3f tr2[3] = {vec3f{0.319444f, 0.00732526f, 0.451091f},
                    vec3f{0.447405f, 0.226107f, 0.139349f},
                    vec3f{0.159013f, -0.363737f, 0.305077f}};
    vec3f z0[3] = {vec3f{-.1f, -.1f, 0},   vec3f{.1f, -.1f, 0},  vec3f{0, .1f, 0}};
    vec3f z1[3] = {vec3f{-.1f, .1f, 0},   vec3f(.1f, .1f, 0),  vec3f{0, -.1f, 0}};
};

#endif // DGLWIDGET_H
