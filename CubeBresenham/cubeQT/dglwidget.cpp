#include "dglwidget.h"
#include <QColor>
#include <functional>
#include <algorithm>
#include <limits>
#include <vector>
#include <QDebug>

dglWidget::dglWidget(QWidget *parent) : QWidget(parent)
{
    resize(m_width, m_height);
    m_image = std::make_unique<QImage>(m_width, m_height, QImage::Format_RGB32);
    m_image->fill(qRgba(0, 0, 0, 255));
    zbuffer = new float[size_t(m_width * m_height)];
    std::fill(zbuffer, zbuffer + m_width * m_height,
              std::numeric_limits<int>::min());
    paintEvent(nullptr);
}

dglWidget::~dglWidget()
{
    delete[] zbuffer;
}

void dglWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        m_eye[0] += dx / 100.f;
        m_eye[1] += dy / 100.f;
    }
    lastPos = event->pos();
    update();
}

void dglWidget::wheelEvent(QWheelEvent *event)
{
    m_eye[2] += event->delta() / 200.f;
    update();
}

void dglWidget::test_draw()
{
    vec3f cube[6][4]
            {
                    {
                            vec3f{-0.5f, -0.5f, 0.5f},
                            vec3f{0.5f, -0.5f, 0.5f},
                            vec3f{0.5f, 0.5f, 0.5f},
                            vec3f{-0.5f, 0.5f, 0.5f},
                    },
                    {
                            vec3f{-0.5f, -0.5f, -0.5f},
                            vec3f{-0.5f, 0.5f, -0.5f},
                            vec3f{0.5f, 0.5f, -0.5f},
                            vec3f{0.5f, -0.5f, -0.5f},
                    },
                    {
                            vec3f{-0.5f, 0.5f, -0.5f},
                            vec3f{-0.5f, 0.5f, 0.5f},
                            vec3f{0.5f, 0.5f, 0.5f},
                            vec3f{0.5f, 0.5f, -0.5f},
                    },
                    {
                            vec3f{-0.5f, -0.5f, -0.5f},
                            vec3f{0.5f, -0.5f, -0.5f},
                            vec3f{0.5f, -0.5f, 0.5f},
                            vec3f{-0.5f, -0.5f, 0.5f},
                    },
                    {
                            vec3f{0.5f, -0.5f, -0.5f},
                            vec3f{0.5f, 0.5f, -0.5f},
                            vec3f{0.5f, 0.5f, 0.5f},
                            vec3f{0.5f, -0.5f, 0.5f},
                    },
                    {
                            vec3f{-0.5f, -0.5f, -0.5f},
                            vec3f{-0.5f, -0.5f, 0.5f},
                            vec3f{-0.5f, 0.5f, 0.5f},
                            vec3f{-0.5f, 0.5f, -0.5f},
                    }
            };

    for (auto &i : cube)
    {
        for (auto &j : i)
        {
            Matrix<float> m = (MVP) * Matrix<float>(j);
            j = m.get_projection();

        }
    }
    for (int i = 0; i < 6; i++)
    {
        draw_quad(cube[i], 10 * i, 20 * i, 30 * i, 255);
    }

}

void dglWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    image = QImage(m_width, m_height, QImage::Format_RGB32);
    image.fill(qRgba(0, 0, 0, 255)); //todo: speed up
    using std::endl, std::cout;
    dgl_look_at(m_eye, m_center, m_up);
    perspective(90.0f, near, far);
    dgl_viewport(0, 0, m_width, m_height);
    MVP = Matrix<float>(((viewport) * (proj) * (model_view)));

    test_draw();
    QPainter painter{this};
    *m_image = image;
    painter.drawImage(0, 0, *m_image);
}

void dglWidget::set_pixel(int x, int y, QRgb color)
{
    if (x >= m_width ||
        y >= m_height ||
        x < 0 ||
        y < 0)
    {
        return;
    }
    image.setPixel(x, y, color);
}

void dglWidget::draw_line(int x0, int y0, int x1, int y1, QRgb color)
{
    using std::swap, std::abs;
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
    {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    float derror = abs(dy / float(dx));
    float error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
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
    for (int y = low; y <= high; y++)
    {
        bool second_half = y > mid;
        int segment_height = second_half ? high - mid + 1 :
                             mid - low + 1;
        float alpha = static_cast<float>(y - low) / whole_height;
        float beta = second_half ? static_cast<float>(y - mid) / segment_height :
                     static_cast<float>(y - low) / segment_height;
        vec2i A = t0 + (t2 - t0) * alpha;
        vec2i B = second_half ? t1 + (t2 - t1) * beta :
                  t0 + (t1 - t0) * beta;
        if (A.get_x() > B.get_x())
        { swap(A, B); }
        for (int j = A.get_x(); j <= B.get_x(); j++)
        {
            set_pixel(j, y, color);
        }
    }
}

void dglWidget::triangle_filled(vec3i t0, vec3i t1, vec3i t2, int colorR, int colorG, int colorB, float alp)
{
    vec3f n = (t2 - t0) ^(t1 - t0);
    n.normalize();
    m_light_v.normalize();
    float intensity = n * m_light_v;
    if (intensity <= 0) { return; }
    colorR = static_cast<int>(colorR * intensity);
    colorG = static_cast<int>(colorG * intensity);
    colorB = static_cast<int>(colorB * intensity);

    using std::swap;
    if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y())
    { return; }
    sort_vec3i_y(t0, t1, t2);
    int total_height = t2.get_y() - t0.get_y();
    for (int y = 0; y < total_height; y++)
    {
        bool second_half = y >= t1.get_y() - t0.get_y();
        int segment_height = second_half ? t2.get_y() - t1.get_y() : t1.get_y() - t0.get_y();
        float alpha = static_cast<float>(y) / total_height;
        float beta = static_cast<float>(y - (second_half ? t1.get_y() - t0.get_y() : 0)) / segment_height;
        vec3f A = t0 + vec3f(t2 - t0) * alpha;
        vec3f B = second_half ? t1 + vec3f(t2 - t1) * beta : t0 + vec3f(t1 - t0) * beta;
        if (A.get_x() > B.get_x())
        { swap(A, B); }
        for (int j = int(A.get_x()); j <= B.get_x(); j++)
        {
            float phi = (B.get_x() - A.get_x() <= eps) ? 1.0f : (j - A.get_x()) /
                                                                (B.get_x() - A.get_x());
            vec3f P = vec3f(A) + vec3f(B - A) * phi;
            int idx = int(P.get_x() + P.get_y() * m_width);
            if (P.get_x() >= m_width ||
                P.get_y() >= m_height ||
                P.get_x() < 0 ||
                P.get_y() < 0)
            { continue; }
            if (zbuffer[idx] < P.get_z())
            {
                zbuffer[idx] = P.get_z();
                set_pixel(int(P.get_x()), int(P.get_y()), qRgba(colorR, colorG, colorB,
                        static_cast<int>(alp)));
            }
        }
    }
}

/***
  Whole plan: Viewport * Projection * [View * Model] * v.
  dgl_look_at makes View * Model
  1) Model matrix (М):
 * Model coordinates --> to world coordinates (as model is uniqie -> 0, 0, 0)
 * so M == E
  2) after View matrix (VM):
 * world coordinates basis -- E
 * our new normalize coordinates -- <x_n, y_n, z_n> (B)
 * (E|B) --> B satisfies:   <x_n, y_n, z_n> = E * B
 * There's only translate left: x, y, z is added to vector (TR)
 * |1 0 0 X|
 * |0 1 0 Y|
 * |0 0 1 Z|
 * |0 0 0 1|
 *  we take X = (-1) * eye[0], Y = -eye[1... to move world to camera
 */
void dglWidget::dgl_look_at(vec3f eye, vec3f center, vec3f up)
{
    vec3f z = (eye - center).normalize();
    vec3f x = (up ^ z).normalize();
    vec3f y = (z ^ x).normalize();
    Matrix<float> B = IdentityMatrix<float>(4, 4);
    Matrix<float> Tr = IdentityMatrix<float>(4, 4);
    for (unsigned int i = 0; i < 3; i++)
    {
        B.put(0, i, x[i]);
        B.put(1, i, y[i]);
        B.put(2, i, z[i]);
        Tr.put(i, 3, -eye[i]);
    }
    model_view = Matrix<float>(B * Tr);
}

/***
 * Viewport matrix: [-1,1]x[-1,1]x[-1,1] -> [x,x+w]x[y,y+h]x[...]
 * |w/2 0   0        [x+w/2]|
 * |0   h/2 0        [y+h/2]|
 * |0   0   (f-n)/2  (f+n)/2|
 * |0   0   0         1     |
*/
void dglWidget::dgl_viewport(int x, int y, int w, int h)
{
    Matrix<float> Viewport = IdentityMatrix<float>(4, 4);
    Viewport[0][3] = x + w / 2.f;
    Viewport[1][3] = y + h / 2.f;

    Viewport[2][3] = (far + near) / 2.f;
    Viewport[0][0] = w / 2.f;
    Viewport[1][1] = h / 2.f;
    Viewport[2][2] = (far - near) / 2.f;
    viewport = Viewport;
}

void dglWidget::draw_quad(vec3f *v, int colorR, int colorG, int colorB, float alp)
{
    triangle_filled(v[0], v[1], v[2], colorR, colorB, colorG, alp);
    triangle_filled(v[0], v[2], v[3], colorR, colorB, colorG, alp);
    triangle_filled(v[1], v[2], v[3], colorR, colorB, colorG, alp);
    triangle_filled(v[0], v[1], v[3], colorR, colorB, colorG, alp);
}

void dglWidget::perspective(const float &angleOfView, const float &near, const float &far)
{
    Matrix<float> M = Matrix<float>(4, 4);
    auto scale = static_cast<float>(1 / tan(angleOfView * 0.5 * M_PI / 180));
    M[0][0] = scale;
    M[1][1] = scale;
    M[2][2] = -far / (far - near);
    M[3][2] = -far * near / (far - near);
    M[2][3] = -1;
    M[3][3] = 0;
    proj = M;
}
