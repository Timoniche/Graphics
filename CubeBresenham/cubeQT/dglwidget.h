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
#include <QKeyEvent>
#include <QString>
#include <QThread>
#include <QTimer>
#include <thread>
#include <functional>

#include "dglgeometry.h"
#include "matrix.h"
#include "worker.h"
#include "bmp.h"

#include "dgl_object.h"

using namespace DGL;

class dglWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dglWidget(QWidget *parent = nullptr);

    ~dglWidget() override;

    signals:

public slots:

    void set_pixel(int x, int y, QRgb color);

    void draw_line(int x0, int y0, int x1, int y1, QRgb color);

    void draw_line(vec2i t0, vec2i t1, QRgb color);

    void triangle(vec2i t0, vec2i t1, vec2i t2, QRgb color);

    vec3f barycentric(vec2f A, vec2f B, vec2f C, vec2f P);

    void
    triangle_scanline_barycentric(vec4f w0,
                                  vec4f w1,
                                  vec4f w2,
                                  vec2f b0, vec2f b1, vec2f b2,
                                  int colorR, int colorG, int colorB, float alp, BMP *bmp, float intensity);

    void triangle_scanline(vec4f w0,
                           vec4f w1,
                           vec4f w2,
                           vec2f b0, vec2f b1, vec2f b2,
                           int colorR, int colorG, int colorB, float alp, BMP *bmp, float intensity);

    void triangle_bbox_barycentric(vec4f w0,
                                   vec4f w1,
                                   vec4f w2,
                                   vec2f b0, vec2f b1, vec2f b2,
                                   int colorR, int colorG, int colorB, float alp, BMP *bmp, float intensity);

    void triangle_filled(vec2i t0, vec2i t1, vec2i t2, QRgb color);

    void dgl_look_at(vec3f eye, vec3f center, vec3f up);

    void dgl_viewport(int x, int y, int w, int h);

    void perspective(const float &angleOfView, const float &aspect, const float &near, const float &far);

    void draw_quad(vec4f bv0,
                   vec4f bv1,
                   vec4f bv2,
                   vec4f bv3,
                   vec2f t0, vec2f t1, vec2f t2, vec2f t3,
                   int colorR, int colorG, int colorB, float alp, float intensity,
                   BMP *bmp);

    void dgl_rotate(float angle, float x, float y, float z);

    vec4f get_bilinear(BMP* bmp, float u, float v);

public slots:

    void onLineEditTextChanged(const QString &text);

    void update_image();

    void clean_image();

public:

    void cout_matrices();

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void resizeEvent(QResizeEvent *e) override;

    void mouseDoubleClickEvent(QMouseEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;

private:
    int m_width;
    int m_height;
    std::unique_ptr <QImage> m_image;
    Matrix<float> model_view;
    Matrix<float> proj;
    Matrix<float> viewport;
    Matrix<float> MVP;
    Matrix<float> VP;
    float *zbuffer;
    enum proj_mode
    {
        ORTHO, PERSP
    } _mode = PERSP;
    vec3f m_eye{2.f, 2.f, 1.f};
    float near = 0.1f;
    float far = 100.f;
    vec3f m_center{0.0f, 0.0f, 0.0f};
    vec3f m_up{0.0f, 1.0f, 0.0f};
    vec3f m_light_v{-0.88f, -0.531f, 0.44f};
    float eps = static_cast<float>(1e-2);
    int xTR = 0;
    int yTR = 0;
    bool mouseDown = false;
    float _xrot = 0.0f;
    float _yrot = 0.0f;
    float _xdiff = 0.0f;
    float _ydiff = 0.0f;
    std::vector<dgl_object *> objects;
    std::function<vec3f(vec3f, int, int)> viewport_f = ([](vec3f p, int m_width, int m_height)
    {
        vec3f ans = {0, 0, 0};
        ans[0] = m_width * (p[0] + 1) / 2;
        ans[1] = m_height * (p[1] + 1) / 2;
        ans[2] = (p[2] + 1) / 2;
        return ans;
    });
    bool _bilinear = false;
public:
    QThread *m_thread = nullptr;
    Worker *m_worker = nullptr;
};

#endif // DGLWIDGET_H
