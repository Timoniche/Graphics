#include "dglwidget.h"
#include <QColor>
#include <functional>

dglWidget::dglWidget(QWidget *parent) : QWidget(parent)
{
    resize(m_width, m_height);
    m_image = std::make_unique<QImage>(m_width, m_height, QImage::Format_RGB32);
    m_image->setColorCount(1);
    m_image->setColor(0, qRgba(0, 0, 0, 255));
    m_image->fill(0);


    //paint sth
    draw_line(0, 0, m_width, m_height, qRgba(255, 255, 255, 255));
    vec2i t0[3] = {vec2i(10, 70),   vec2i(50, 160),  vec2i(70, 80)};
    vec2i t1[3] = {vec2i(180, 50),  vec2i(150, 1),   vec2i(70, 180)};
    vec2i t2[3] = {vec2i(180, 150), vec2i(120, 160), vec2i(130, 180)};
    //triangle(t0[0], t0[1], t0[2], qRgba(0, 255, 255, 255));
    triangle_filled(t0[0], t0[1], t0[2], qRgba(0, 255, 255, 255), qRgba(255, 0, 255, 255));
    triangle(t1[0], t1[1], t1[2], qRgba(255, 0, 255, 255));
    triangle(t2[0], t2[1], t2[2], qRgba(255, 255, 0, 255));
}

dglWidget::~dglWidget()
{
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
    draw_line(t0.getX(), t0.getY(), t1.getX(), t1.getY(), color);
}

void dglWidget::triangle(vec2i t0, vec2i t1, vec2i t2, QRgb color)
{
    draw_line(t0, t1, color);
    draw_line(t1, t2, color);
    draw_line(t2, t0, color);
}

void dglWidget::triangle_filled(vec2i t0, vec2i t1, vec2i t2, QRgb color, QRgb fill)
{
    std::vector<std::reference_wrapper<vec2i>> v{t0, t1, t2};
    std::sort(v.begin(), v.end(), [](vec2i const& lhs, vec2i const& rhs)
    {
        return rhs.getY() > lhs.getY();
    });
    int low = v[0].get().getY();
    int mid = v[1].get().getY();
    int high = v[2].get().getY();
    //if (t0.getY() > t1.getY()) { swap(t0, t1); }
    //if (t0.getY() > t2.getY()) { swap(t0, t2); }
    //if (t1.getY() > t2.getY()) { swap(t1, t2); }

    int total_height = high - low;
    for (int y=low; y <= mid; y++) {
        int segment_height = mid - low + 1;
        float alpha = (float)(y-low)/total_height;
        float beta  = (float)(y-low)/segment_height;
        vec2i A = t0 + (t2-t0)*alpha;
        vec2i B = t0 + (t1-t0)*beta;
        if (A.getX() > B.getX()) std::swap(A, B);
        for (int j= A.getX(); j <= B.getX(); j++) {
            set_pixel(j, y, color);
        }
    }
    for (int y=mid; y<=high; y++) {
        int segment_height =  high - mid+1;
        float alpha = (float)(y - low) / total_height;
        float beta  = (float)(y - mid) / segment_height;
        vec2i A = t0 + (t2 - t0) * alpha;
        vec2i B = t1 + (t2 - t1) * beta;
        if (A.getX() > B.getX()) std::swap(A, B);
        for (int j=A.getX(); j<=B.getX(); j++) {
            set_pixel(j, y, color);
        }
    }
}

