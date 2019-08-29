#ifndef DGLWIDGET_H
#define DGLWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <memory>

#include "dglgeometry.h"

using namespace DGL;

class dglWidget : public QWidget
{
    Q_OBJECT

public:
    explicit dglWidget(QWidget *parent = nullptr);
    ~dglWidget();

signals:

public slots:
    void set_pixel(int x, int y, QRgb color);
    void draw_line(int x0, int y0, int x1, int y1, QRgb color);
    void draw_line(vec2i t0, vec2i t1, QRgb color);
    void triangle(vec2i t0, vec2i t1, vec2i t2, QRgb color);
    void triangle_filled(vec2i t0, vec2i t1, vec2i t2, QRgb color, QRgb fill);
protected:
    void paintEvent(QPaintEvent* event);

private:
    int m_width = 1 << 10;
    int m_height = 1 << 9;
    std::unique_ptr<QImage> m_image;
};

#endif // DGLWIDGET_H
