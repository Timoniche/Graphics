#include "dglwidget.h"
#include <QColor>
#include <functional>
#include <algorithm>
#include <limits>
#include <vector>
#include <QDebug>
#include <string>
#include <sstream>

#include "shader.h"
#include "worker.h"
#include "shader.h"

dglWidget::dglWidget(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<vec3f>("vec3f");
    resize(m_width, m_height);

    m_image = std::make_unique<QImage>(m_width, m_height, QImage::Format_RGB32);
    m_image->fill(qRgba(0, 0, 0, 255));
    zbuffer = new float[size_t(m_width * m_height)];
    std::fill(zbuffer, zbuffer + m_width * m_height,
              std::numeric_limits<int>::min());

    perspective(80.0f, 3.0f / 3.0f, near, far);
    dgl_viewport(0, 0, m_width, m_height);
    VP = viewport * proj;

    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
    //m_timer.setInterval(FIVE_SECONDS);
    //m_timer.start(0);
    //
    //updateImage();
    //
    repaint();
}

dglWidget::~dglWidget()
{
    delete[] zbuffer;
}

void dglWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = event->x() - last_pos.x();
    float dy = event->y() - last_pos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        m_eye[0] += dx / 1000.f;
        m_eye[1] += dy / 1000.f;
    }
    last_pos = event->pos();
    repaint();
}

void dglWidget::wheelEvent(QWheelEvent *event)
{
    m_eye[2] += float(event->delta()) / 200.f;
    repaint();
}

void dglWidget::clean_image()
{
    m_image->fill(qRgba(0, 0, 0, 255));
    //promise.set_value();
}

void dglWidget::onLineEditTextChanged(const QString& text)
{
    std::string s = text.toStdString();
    std::stringstream ss(s);
    std::string val_str;
    vec3f values;
    for (size_t i = 0; i < 3; i++)
    {
        ss >> val_str;
        try {
            values[i] = std::stof(val_str);
        } catch (std::invalid_argument& e) {
            qDebug() << "incorrect input, should be x.f y.f z.f";
            qDebug() << e.what();
            return;
        } catch (std::out_of_range& e)
        {
            qDebug() << "out of range";
            qDebug() << e.what();
            return;
        }
    }
    m_eye = values;
    std::thread func([this]()
    {
        clean_image();
    });
    if (func.joinable()) { func.join(); }
    repaint();
}

void dglWidget::update_image()
{
    dgl_look_at(m_eye, m_center, m_up);
    cout_matrices();
    m_thread = new QThread();
    m_worker = new Worker(&model_view, &proj, &viewport);
    m_worker->moveToThread(m_thread);
    bool ok1 = connect(m_thread, &QThread::started, m_worker, &Worker::draw_cube);
    bool ok2 = connect(m_worker, &Worker::draw_quad, this, &dglWidget::draw_quad);
    //bool ok3 = connect(m_worker, &Worker::finished, this, &dglWidget::updateImage);
    bool ok4 = connect(m_worker, &Worker::finished, m_thread, &QThread::quit);
    bool ok5 = connect(m_worker, &Worker::finished, m_worker, &QObject::deleteLater);
    bool ok6 = connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    Q_ASSERT(ok1 && ok2 /**&& ok3**/ && ok4 && ok5 && ok6);
    m_thread->start();
}

void dglWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    dgl_look_at(m_eye, m_center, m_up);
    cout_matrices();
    //m_image->fill(qRgba(0, 0, 0, 255));
    QPainter painter{this};
    test_cube();
    painter.drawImage(0, 0, *m_image);

}

void dglWidget::cout_matrices()
{
    std::cout << "_____________________________" << std::endl;
    std::cout << "modelview\n" << model_view << std::endl;
    std::cout << "proj\n" << proj << std::endl;
    std::cout << "viewport\n" << viewport << std::endl;
    std::cout << "_____________________________" << std::endl;
}
void dglWidget::set_pixel(int x, int y, QRgb color)
{
    if (x >= xw_VP ||
            y >= yh_VP ||
            x < x_VP ||
            y < y_VP)
    {
        return;
    }
    auto *rowData = reinterpret_cast<QRgb*>(m_image->scanLine(y));
    rowData[x] = color;
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

void dglWidget::test_cube()
{
    Shader shader(&model_view, &proj, &viewport);
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

    for (int i = 0; i < 6; i++)
    {
        vec3f tmp[4];
        for (int j = 0; j < 4; j++)
        {
            //Matrix<float> m = proj * model_view * Matrix<float>(cube[i][j]);
            Matrix<float> m = proj * model_view * Matrix<float>(cube[i][j]);
            std::cout << "cube [-1, 1] matrix before shrink" << std::endl << m << std::endl;
            //vec3f a = m.get_projection();
            vec3f a(m[0][0], m[1][0], m[2][0]);
            std::cout << "vertex: " << a << std::endl;
            tmp[j] = vec3f((a[0] +1.f) * m_width / 2.f,
                    (a[1]+1.f) * m_height / 2.f, a[2]);
            std::cout << "final: " << tmp[j];
            //tmp[j] = shader.count_coordinates(cube[i][j]);
        }
        draw_quad(tmp[0], tmp[1], tmp[2], tmp[3],
                0, 0, 255, 255);
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
    vec3f n = (t2 - t0) ^ (t1 - t0);
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
            if (P.get_x() >= xw_VP ||
                    P.get_y() >= yh_VP ||
                    P.get_x() < x_VP ||
                    P.get_y() < y_VP)
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
    model_view.transpose();
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
    x_VP = x;
    y_VP = y;
    xw_VP = x + w;
    yh_VP = y + h;
    Matrix<float> Viewport = IdentityMatrix<float>(4, 4);
    Viewport[0][3] = x + w / 2.f;
    Viewport[1][3] = y + h / 2.f;
    Viewport[2][3] = (far + near) / 2.f;

    Viewport[0][0] = w / 2.f;
    Viewport[1][1] = h / 2.f;
    Viewport[2][2] = (far - near) / 2.f;
    viewport = Viewport;
}

void dglWidget::draw_quad(vec3f v0, vec3f v1, vec3f v2, vec3f v3, int colorR, int colorG, int colorB, float alp)
{
    triangle_filled(v0, v1, v2, colorR, colorB, colorG, alp);
    triangle_filled(v0, v2, v3, colorR, colorB, colorG, alp);
    triangle_filled(v1, v2, v3, colorR, colorB, colorG, alp);
    triangle_filled(v0, v1, v3, colorR, colorB, colorG, alp);
}


void dglWidget::perspective(const float &fovy,
                            const float &aspect,
                            const float &zNear,
                            const float &zFar)
{
    float const rad = fovy * float(M_PI) / 180.f;
    float const tanHalfFovy = tanf(rad / 2.0f);
    Matrix<float> Result(4, 4);
    Result.put(0, 0, (1 / (aspect * tanHalfFovy)));
    Result.put(1, 1, (1 / (tanHalfFovy)));
    Result.put(2, 2, (-(zFar + zNear) / (zFar - zNear)));
    Result.put(3, 2, (-1));
    Result.put(2, 3, (-(2 * zFar * zNear) / (zFar - zNear)));
    Result.transpose();
    proj = Result;
}
