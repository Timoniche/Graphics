#include "dglwidget.h"
#include <QColor>
#include <functional>
#include <algorithm>
#include <limits>
#include <vector>
#include <QDebug>
#include <string>
#include <sstream>
#include <iomanip>

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
              std::numeric_limits<float>::max());

    perspective(80.0f, 4.0f / 3.0f, near, far);
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

//todo: make dglRotate
void dglWidget::mouseMoveEvent(QMouseEvent *event) /**NOT USE IT**/
{
    float dx = event->x() - last_pos.x();
    float dy = event->y() - last_pos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        m_eye[0] += dx / 300.f;
        m_eye[1] += dy / 300.f;

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
    //m_image->fill(qRgba(0, 0, 0, 255));
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
    std::fill(zbuffer, zbuffer + m_width * m_height,
              std::numeric_limits<float>::max());
    Q_UNUSED(event);
    QPainter pnt{this};
    //    pnt.begin(m_image.get());
    //    pnt.fillRect(0, 0, m_width, m_height, Qt::black);
    //    pnt.end();

    dgl_look_at(m_eye, m_center, m_up);
    cout_matrices();
    m_image->fill(qRgba(0, 0, 0, 255));
    //QPainter painter{this};
    test_cube();
    pnt.drawImage(0, 0, *m_image);

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

    auto *rowData = reinterpret_cast<QRgb*>(m_image->scanLine(m_height - y));
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

    int a[6][3] { {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
                  {255, 255, 0}, {255, 0, 255}, {0, 255, 255}};
    for (int i = 0; i < 6; i++)
    {
        vec3f tmp[4];
        for (int j = 0; j < 4; j++)
        {
            vec3f aaa = cube[i][j];
            Matrix<float> m = Matrix<float>(aaa);
            m.transpose();
            m = m * model_view;
            m = m * proj;
            vec3f p = m.get_projection();
            vec3f ans = {0, 0, 0};
            ans[0] = m_width * (p[0] + 1) / 2;
            ans[1] = m_height * (p[1] + 1) / 2;
            ans[2] = 10000 * (p[2] + 1) / 2;
            tmp[j] = ans;
            std::cout << aaa << ans << std::endl;
        }
        draw_quad(tmp[0], tmp[1], tmp[2], tmp[3],
                a[i][0], a[i][1], a[i][2], 255);
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

void dglWidget::triangle_filled(vec3i t0, vec3i t1, vec3i t2,
                                vec2i b0, vec2i b1, vec2i b2,
                                int colorR, int colorG, int colorB, float alp)
{
    //_________________________________________________
    vec3f n = (t2 - t0) ^ (t1 - t0);
    n.normalize();
    m_light_v.normalize();
    float intensity = n * m_light_v;
    intensity = std::max(0.3f, intensity);
    colorR = static_cast<int>(colorR * intensity);
    colorG = static_cast<int>(colorG * intensity);
    colorB = static_cast<int>(colorB * intensity);
    //_________________________________________________

    if (t0.y==t1.y && t0.y==t2.y) return;
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height;
        vec3i A =               t0 + (t2-t0)*alpha;
        vec3i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            vec3i P = A + (B-A)*phi;
            P.x = j; P.y = t0.y+i;
            int idx = j+(t0.y+i)*m_width;
            if (idx < 0 || idx >= m_width * m_height || P.z > 10000) continue;
            if (zbuffer[idx] > P.z) {
                zbuffer[idx] = P.z;
                set_pixel(P.x, P.y, qRgba(colorR, colorG, colorB, alp));
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
    vec2i tmp = {0, 0};
    triangle_filled(v0, v1, v2, tmp, tmp, tmp, colorR, colorB, colorG, alp);
    triangle_filled(v0, v2, v3, tmp, tmp, tmp, colorR, colorB, colorG, alp);
    triangle_filled(v1, v2, v3, tmp, tmp, tmp, colorR, colorB, colorG, alp);
    triangle_filled(v0, v1, v3, tmp, tmp, tmp, colorR, colorB, colorG, alp);
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
