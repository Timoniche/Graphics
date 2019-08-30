#include "dglwidget.h"
#include <QColor>
#include <functional>
#include <algorithm>
#include <limits>
#include <vector>

dglWidget::dglWidget(QWidget *parent) : QWidget(parent)
{
    resize(m_width, m_height);
    m_image = std::make_unique<QImage>(m_width, m_height, QImage::Format_RGB32);
    m_image->fill(qRgba(0, 0, 0, 255));
    zbuffer = new int[size_t(m_width * m_height)];
    std::fill(zbuffer, zbuffer + m_width * m_height,
              std::numeric_limits<int>::min());

    //paint sth_____________________________________________________________________
    draw_line(0, 0, m_width - 1, m_height - 1, qRgba(255, 255, 255, 255));
    vec2i t0[3] = {vec2i(10, 70),   vec2i(50, 160),  vec2i(70, 80)};
    vec2i t1[3] = {vec2i(180, 50),  vec2i(150, 1),   vec2i(70, 180)};
    vec2i t2[3] = {vec2i(180, 150), vec2i(120, 160), vec2i(130, 180)};
    triangle_filled(t0[0], t0[1], t0[2], qRgba(0, 255, 255, 255));
    triangle(t1[0], t1[1], t1[2], qRgba(255, 0, 255, 255));
    triangle(t2[0], t2[1], t2[2], qRgba(255, 255, 0, 255));
    vec3i delta =  {vec3i(200, 200, 0)};
    vec3i z0[3] = {vec3i(10, 70, 0) + delta,   vec3i(50, 160, 0) + delta,  vec3i(70, 80, 0) + delta};
    vec3i p0[3] = {vec3i(10, 70, 1) + delta,   vec3i(50, 160, -1) + delta,  vec3i(70, 80, 1) + delta};
    triangle_filled(z0[0], z0[1], z0[2], qRgba(255, 0, 0, 255));
    triangle_filled(p0[0], p0[1], p0[2], qRgba(0, 255, 0, 255));
    //____________________________________________________________________________
}

dglWidget::~dglWidget()
{
    delete[] zbuffer;
}

void dglWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter{this};
    painter.drawImage(0, 0, *m_image);
}

void dglWidget::set_pixel(int x, int y, QRgb color)
{
    m_image->setPixel(x, y, color);
}

void dglWidget::draw_line(int x0, int y0, int x1, int y1, QRgb color)
{
    using std::swap, std::abs;
    bool steep = false;
    if (abs(x0-x1) < abs(y0-y1)) {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    float derror = abs(dy / float(dx));
    float error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        steep ? set_pixel(y, x, color) : set_pixel(x, y, color);
        error += derror;
        if (error > 0.5f)
        {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.0f;
        }
    }
}

void dglWidget::draw_line(vec2i t0, vec2i t1, QRgb color)
{
    draw_line(t0.get_x(), t0.get_y(), t1.get_x(), t1.get_y(), color);
}

void dglWidget::triangle(vec2i t0, vec2i t1, vec2i t2, QRgb color)
{
    draw_line(t0, t1, color);
    draw_line(t1, t2, color);
    draw_line(t2, t0, color);
}

void dglWidget::triangle_filled(vec2i t0, vec2i t1, vec2i t2, QRgb color)
{
    using std::swap;
    sort_vec2i_y(t0, t1, t2);
    int low = t0.get_y();
    int mid = t1.get_y();
    int high = t2.get_y();
    int whole_height = high - low;
    for (int y = low; y <= high; y++) {
        bool second_half = y > mid;
        int segment_height = second_half ? high - mid + 1 :
                                           mid - low + 1;
        float alpha = static_cast<float>(y - low) / whole_height;
        float beta  = second_half ? static_cast<float>(y - mid) / segment_height :
                                    static_cast<float>(y - low) / segment_height;
        vec2i A = t0 + (t2 - t0) * alpha;
        vec2i B = second_half ? t1 + (t2 - t1) * beta :
                                t0 + (t1 - t0) * beta;
        if (A.get_x() > B.get_x()) { swap(A, B); }
        for (int j= A.get_x(); j <= B.get_x(); j++)
        {
            set_pixel(j, y, color);
        }
    }
}

void dglWidget::triangle_filled(vec3i t0, vec3i t1, vec3i t2, QRgb color) {
    using std::swap;
    if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y()) { return; }
    sort_vec3i_y(t0, t1, t2);
    int total_height = t2.get_y() - t0.get_y();
    for (int y = 0; y < total_height; y++) {
        bool second_half = y >= t1.get_y() - t0.get_y();
        int segment_height = second_half ? t2.get_y() - t1.get_y() : t1.get_y() - t0.get_y();
        float alpha = static_cast<float>(y) / total_height;
        float beta  = static_cast<float>(y - (second_half ? t1.get_y()-t0.get_y() : 0)) / segment_height;
        vec3i A = t0 + vec3f(t2 - t0) * alpha;
        vec3i B = second_half ? t1 + vec3f(t2 - t1) * beta : t0 + vec3f(t1 - t0) * beta;
        if (A.get_x() > B.get_x()) { swap(A, B); }
        for (int j = A.get_x(); j <= B.get_x(); j++) {
            float phi = B.get_x() == A.get_x() ? 1.0f : static_cast<float>(j - A.get_x()) /
                                                 static_cast<float>(B.get_x() - A.get_x());
            vec3i P = vec3f(A) + vec3f(B - A) * phi;
            int idx = P.get_x() + P.get_y() * m_width;
            if (zbuffer[idx] < P.get_z()) {
                zbuffer[idx] = P.get_z();
                set_pixel(P.get_x(), P.get_y(), color);
            }
        }
    }
}

