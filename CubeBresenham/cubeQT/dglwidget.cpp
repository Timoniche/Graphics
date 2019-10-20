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
    setStyleSheet("background-color: black;");
    //this->setPaletteBackgroundColor(Qt::black);
    setAutoFillBackground(true);

    qRegisterMetaType<vec3f>("vec3f");
    m_width = parent->width();
    m_height = parent->height();
    resize(m_width, m_height);
    m_image = std::make_unique<QImage>(3000, 3000, QImage::Format_RGB32);
    m_image->fill(qRgba(0, 0, 0, 255));
    zbuffer = new float[3000 * 3000];
    std::fill(zbuffer, zbuffer + 3000 * 3000,
              std::numeric_limits<float>::max());
    dgl_look_at(m_eye, m_center, m_up);
    perspective(80.0f, 4.0f / 3.0f, near, far);
    dgl_viewport(0, 0, m_width, m_height);
    VP = viewport * proj;
    m_light_v.normalize();
    repaint();
    _mode = PERSP;
    BMP bmp1("C:/Users/Timoniche/Desktop/Graphics/CubeBresenham/cubeQT/one.bmp");
    BMP bmp2("C:/Users/Timoniche/Desktop/Graphics/CubeBresenham/cubeQT/TallGreenGrass.bmp");

    vec3f cube1[6][4]
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

    vec3f translate2 = {0.3f, -0.5f, -0.5f};
    float alpha2 = 1.2f;
    vec3f cube2[6][4];
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cube2[i][j] = (cube1[i][j] + translate2) * alpha2;
        }
    }
    dgl_cube* o1 = new dgl_cube(cube1, bmp1);
    dgl_cube* o2 = new dgl_cube(cube2, bmp2);
    objects.push_back(o1);
    objects.push_back(o2);
}

dglWidget::~dglWidget()
{
    delete[] zbuffer;
    delete _bmp;
    for (int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }
}

void dglWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    Shader shader(&model_view, &proj, m_width, m_height);
    float x = float(e->x());
    float y = float(e->y());
    vec3f eye_screen = shader.count_coordinates(m_eye);
    float z = float(eye_screen.z - 1);
    vec3f to_reverse = {x, y, z};

    m_center = shader.reverse_coordinates(to_reverse);
    repaint();
}

void dglWidget::resizeEvent(QResizeEvent* e)
{
    int size = std::min(e->size().height(), e->size().width());
    float delta_w = std::abs(e->size().width() - size) / 2;
    float delta_h = std::abs(e->size().height() - size) / 2;
    m_width = size;
    m_height = size;

    x_VP = delta_w;
    y_VP = delta_h;
}

void dglWidget::mousePressEvent(QMouseEvent *event)
{
    float x = event->x();
    float y = event->y();

    if (event->buttons() & Qt::LeftButton)
    {
        mouseDown = true;
        xdiff = x - yrot;
        ydiff = -y + xrot;
    } else
    {
        mouseDown = false;
    }
    repaint();
}
void dglWidget::mouseMoveEvent(QMouseEvent *event)
{
    float x = event->x();
    float y = event->y();

    if (mouseDown)
    {
        yrot = x - xdiff;
        xrot = y + ydiff;
        repaint();
    }
}

void dglWidget::wheelEvent(QWheelEvent *event)
{
    vec3f direction_to_get_closer = m_center - m_eye;

    direction_to_get_closer.normalize();
    m_eye = m_eye + direction_to_get_closer * (float(event->delta()) / 500.0f);
    repaint();
}

void dglWidget::clean_image()
{
    m_image->fill(qRgba(0, 0, 0, 255));
}

void dglWidget::onLineEditTextChanged(const QString &text)
{
    std::string s = text.toStdString();
    std::stringstream ss(s);
    std::string val_str;
    vec3f values;
    for (size_t i = 0; i < 3; i++)
    {
        ss >> val_str;
        try
        {
            values[i] = std::stof(val_str);
        } catch (std::invalid_argument &e)
        {
            qDebug() << "incorrect input, should be x.f y.f z.f";
            qDebug() << e.what();
            return;
        } catch (std::out_of_range &e)
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
    if (func.joinable())
    { func.join(); }
    repaint();
}

void dglWidget::update_image()
{
    //    dgl_look_at(m_eye, m_center, m_up);
    //    cout_matrices();
    //    m_thread = new QThread();
    //    m_worker = new Worker(&model_view, &proj, &viewport);
    //    m_worker->moveToThread(m_thread);
    //    bool ok1 = connect(m_thread, &QThread::started, m_worker, &Worker::draw_cube);
    //    bool ok2 = connect(m_worker, &Worker::draw_quad, this, &dglWidget::draw_quad);
    //    //bool ok3 = connect(m_worker, &Worker::finished, this, &dglWidget::updateImage);
    //    bool ok4 = connect(m_worker, &Worker::finished, m_thread, &QThread::quit);
    //    bool ok5 = connect(m_worker, &Worker::finished, m_worker, &QObject::deleteLater);
    //    bool ok6 = connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    //    Q_ASSERT(ok1 && ok2 /**&& ok3**/ && ok4 && ok5 && ok6);
    //    m_thread->start();
}

void dglWidget::paintEvent(QPaintEvent *event)
{
    clean_image();
    std::fill(zbuffer, zbuffer + 3000 * 3000,
              std::numeric_limits<float>::max());
    Q_UNUSED(event);
    QPainter pnt{this};
    dgl_look_at(m_eye, m_center, m_up);
    dgl_rotate(xrot, 1.0f, 0.0f, 0.0f);
    dgl_rotate(yrot, 0.0f, 1.0f, 0.0f);
    //cout_matrices();
    //test_cube();
    for (auto o : objects)
    {
        if (auto* t = dynamic_cast<dgl_cube*>(o))
        {
            Shader shader(&model_view, &proj, m_width, m_height);
            auto cube = t->quads;
            auto textures = t->textures;
            float norms[6];
            for (int i = 0; i < 6; i++)
            {
                vec3f t0 = cube[i][0];
                vec3f t1 = cube[i][1];
                vec3f t2 = cube[i][2];
                vec3f n = (t2 - t0) ^ (t1 - t0);
                n.normalize();
                m_light_v.normalize();
                float intensity = n * m_light_v;
                intensity = std::max(0.2f, intensity);
                norms[i] = intensity;

            }
            int a[6][3]{{255, 0,   0},
                        {0,   255, 0},
                        {0,   0,   255},
                        {255, 255, 0},
                        {255, 0,   255},
                        {0,   255, 255}};
            for (int i = 0; i < 6; i++)
            {
                vec3f tmp[4];
                std::pair<vec3f, float> bv[4];
                for (int j = 0; j < 4; j++)
                {

                    tmp[j] = shader.count_coordinates(cube[i][j]);
                    bv[j] = shader.before_viewport(cube[i][j]);
                }
                draw_quad(bv[0], bv[1], bv[2], bv[3],
                        textures[i][0], textures[i][1], textures[i][2], textures[i][3],
                        a[i][0], a[i][1], a[i][2], 255, norms[i], &t->bmp);
            }
        }
    }

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
    //    if (x >= xw_VP ||
    //            y >= yh_VP ||
    //            x < x_VP ||
    //            y < y_VP)
    //    {
    //        return;
    //    }
    auto *rowData = reinterpret_cast<QRgb *>(m_image->scanLine(m_height - y));
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
    Shader shader(&model_view, &proj, m_width, m_height);
    vec2f textures[6][4]
    {
        {
            vec2f{0, 0},
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
        },
        {
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
            vec2f{0, 0},
        },
        {
            vec2f{0, 1},
            vec2f{0, 0},
            vec2f{1, 0},
            vec2f{1, 1},
        },
        {
            vec2f{1, 1},
            vec2f{0, 1},
            vec2f{0, 0},
            vec2f{1, 0},
        },
        {
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
            vec2f{0, 0},
        },
        {
            vec2f{0, 0},
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
        }
    };
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
    float norms[6];
    for (int i = 0; i < 6; i++)
    {

        vec3f t0 = cube[i][0];
        vec3f t1 = cube[i][1];
        vec3f t2 = cube[i][2];
        vec3f n = (t2 - t0) ^ (t1 - t0);
        n.normalize();
        m_light_v.normalize();
        float intensity = n * m_light_v;
        intensity = std::max(0.2f, intensity);
        norms[i] = intensity;

    }
    int a[6][3]{{255, 0,   0},
                {0,   255, 0},
                {0,   0,   255},
                {255, 255, 0},
                {255, 0,   255},
                {0,   255, 255}};
    for (int i = 0; i < 6; i++)
    {
        vec3f tmp[4];
        std::pair<vec3f, float> bv[4];
        for (int j = 0; j < 4; j++)
        {

            tmp[j] = shader.count_coordinates(cube[i][j]);
            bv[j] = shader.before_viewport(cube[i][j]);
        }
        draw_quad(bv[0], bv[1], bv[2], bv[3],
                textures[i][0], textures[i][1], textures[i][2], textures[i][3],
                a[i][0], a[i][1], a[i][2], 255, norms[i], _bmp);
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

vec3f dglWidget::barycentric(vec2f A, vec2f B, vec2f C, vec2f P) {
    vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    vec3f u = (s[0] ^ s[1]);
    if (std::abs(u[2])>1e-2)
        return vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return vec3f(-1,1,1);
}

void dglWidget::triangle_bbox_barycentric(std::pair<vec3f, float> w0,
                                          std::pair<vec3f, float> w1,
                                          std::pair<vec3f, float> w2,
                                          vec2f b0, vec2f b1, vec2f b2,
                                          int colorR, int colorG, int colorB, float alp,
                                          BMP* bmp, float intensity)
{
    colorR = static_cast<int>(colorR * intensity);
    colorG = static_cast<int>(colorG * intensity);
    colorB = static_cast<int>(colorB * intensity);

    std::function viewport_f = ([](vec3f p, int m_width, int m_height)
    {
            vec3f ans = {0, 0, 0};
            ans[0] = m_width * (p[0] + 1) / 2;
    ans[1] = m_height * (p[1] + 1) / 2;
    ans[2] = 10000 * (p[2] + 1) / 2;
    return ans;
});
vec3f t0 = viewport_f(w0.first * (1.f / w0.second), m_width, m_height);
vec3f t1 = viewport_f(w1.first * (1.f / w1.second), m_width, m_height);
vec3f t2 = viewport_f(w2.first * (1.f / w2.second), m_width, m_height);

vec3f pts[3]  = {t0, t1, t2};
vec2f pts2[3] = {{t0.x, t0.y}, {t1.x, t1.y}, {t2.x, t2.y}};
vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
vec2f clamp(m_width-1, m_height-1);
for (int i=0; i<3; i++) {
    for (int j=0; j<2; j++) {
        bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
        bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
    }
}
vec2i P;
for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
    for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
        vec3f bc_screen  = barycentric(pts2[0], pts2[1], pts2[2], P);
        vec3f bc_clip    = vec3f(bc_screen.x/w0.second, bc_screen.y/w1.second, bc_screen.z/w2.second);
        bc_clip = bc_clip * (1 / (bc_clip.x+bc_clip.y+bc_clip.z));
        vec3f ans = pts[0] * bc_clip[0] + pts[1] * bc_clip[1] + pts[2] * bc_clip[2];
        vec2i T = b0 * bc_clip[0] + b1 * bc_clip[1] + b2 * bc_clip[2];
        int idx = P.x+P.y*m_width;
        if (idx < 0 || idx >= m_width * m_height || bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
        if (zbuffer[idx] > ans.z)
        {
            zbuffer[idx] = ans.z;
            if (bmp != nullptr)
            {
                vec3i col;
                uint32_t channels = bmp->bmp_info_header.bit_count / 8;
                unsigned int max_size_index = static_cast<unsigned int>(bmp->data.size() - 1);
                size_t index_col0 = std::min(max_size_index, channels * (T.y * bmp->bmp_info_header.width + T.x) + 0);
                size_t index_col1 = std::min(max_size_index, channels * (T.y * bmp->bmp_info_header.width + T.x) + 1);
                size_t index_col2 = std::min(max_size_index, channels * (T.y * bmp->bmp_info_header.width + T.x) + 2);
                col[0] = bmp->data[index_col0];   // B
                col[1] = bmp->data[index_col1];   // G
                col[2] = bmp->data[index_col2];   // R
                if (channels == 4) alp = bmp->data[channels * (T.y * bmp->bmp_info_header.width + T.x) + 3];
                set_pixel(P.x + x_VP, P.y - y_VP, qRgba(int(intensity * col[2]),
                          int(intensity * col[1]),
                        int(intensity * col[0]), alp));
            } else
            {
                set_pixel(P.x + x_VP, P.y - y_VP, qRgba(colorR, colorG, colorB, alp));
            }
            //}
        }
    }
}
}

void dglWidget::triangle_scanline(std::pair<vec3f, float> w0,
                                  std::pair<vec3f, float> w1,
                                  std::pair<vec3f, float> w2,
                                  vec2f b0, vec2f b1, vec2f b2,
                                  int colorR, int colorG, int colorB, float alp, BMP* bmp, float intensity)
{
    colorR = static_cast<int>(colorR * intensity);
    colorG = static_cast<int>(colorG * intensity);
    colorB = static_cast<int>(colorB * intensity);

    std::function viewport_f = ([](vec3f p, int m_width, int m_height)
    {
        vec3f ans = {0, 0, 0};
        ans[0] = m_width * (p[0] + 1) / 2;
        ans[1] = m_height * (p[1] + 1) / 2;
        ans[2] = 10000 * (p[2] + 1) / 2;
        return ans;
    });

float Z0 = 1 / w0.second;
float Z1 = 1 / w1.second;
float Z2 = 1 / w2.second;

    vec3i t0 = viewport_f(w0.first * Z0, m_width, m_height);
    vec3i t1 = viewport_f(w1.first * Z1, m_width, m_height);
    vec3i t2 = viewport_f(w2.first * Z2, m_width, m_height);



    b0 = b0 * Z0;
    b1 = b1 * Z1;
    b2 = b2 * Z2;

    using std::swap;
    if (t0.get_y() > t1.get_y())
    {
        swap(t0, t1);
        swap(b0, b1);
        swap(Z0, Z1);
    }
    if (t0.get_y() > t2.get_y())
    {
        swap(t0, t2);
        swap(b0, b2);
        swap(Z0, Z2);
    }
    if (t1.get_y() > t2.get_y())
    {
        swap(t1, t2);
        swap(b1, b2);
        swap(Z1, Z2);
    }
    //qDebug() << w0.second << " " << w1.second << " " << w2.second << "\n";
    int total_height = t2.y - t0.y;
    int beginh = std::abs(std::max(0, t0.y) - t0.y);
    int endh = std::abs(std::min(m_height - 1, t2.y) - t2.y);
    for (int i = 0 + beginh; i < total_height - endh; i++)
    {
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        float alpha = (float) i / total_height;
        float beta = (float) (i - (second_half ? t1.y - t0.y : 0)) / segment_height;


        vec3f A = t0 + (t2 - t0) * alpha;
        float ZA = Z0 + (Z2 - Z0) * alpha;
        vec2f Ab = ((b0) + ((b2) - (b0)) * alpha) / ZA;
        vec3f B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        float ZB = second_half ? Z1 + (Z2 - Z1) * beta : Z0 + (Z1 - Z0) * beta;
        vec2f Bb = (second_half ? ((b1) + ((b2) - (b1)) * beta) :
                                  ((b0) + ((b1) - (b0)) * beta)) / ZB;
        if (A.x > B.x)
        {
            std::swap(A, B);
            std::swap(Ab, Bb);
            std::swap(ZA, ZB);
        }
         for (int j = std::max(int(A.x), 0); j <= std::min(int(B.x), m_width - 1); j++)
        {
            float phi = int(B.x) == int(A.x) ? 1. : (float) (j - A.x) / (float) (B.x - A.x);
            vec3f P = A + (B - A) * phi;
            float ZP = ZA + (ZB - ZA) * phi;
            float d1 = (float)ZA / ZP;
            float d2 = (float)ZB / ZP;
            vec2f Pb = ((Ab * d1) + ((Bb * d2) - (Ab * d1)) * phi);
            P.x = j;
            P.y = t0.y + i;
            int idx = j + (t0.y + i) * m_width;
            if (P.z > 10000) continue;
            if (zbuffer[idx] > P.z)
            {
                zbuffer[idx] = P.z;
                if (bmp != nullptr)
                {
                    vec3i col;
                    uint32_t channels = bmp->bmp_info_header.bit_count / 8;
                    unsigned int max_size_index = static_cast<unsigned int>(bmp->data.size() - 1);
                    size_t index_col0 = std::min(max_size_index, channels * (int(Pb[1]) * bmp->bmp_info_header.width + int(Pb[0])) + 0);
                    size_t index_col1 = std::min(max_size_index, channels * (int(Pb[1]) * bmp->bmp_info_header.width + int(Pb[0])) + 1);
                    size_t index_col2 = std::min(max_size_index, channels * (int(Pb[1]) * bmp->bmp_info_header.width + int(Pb[0])) + 2);
                    col[0] = bmp->data[index_col0];   // B
                    col[1] = bmp->data[index_col1];   // G
                    col[2] = bmp->data[index_col2];   // R
                    if (channels == 4) alp = bmp->data[channels * (Pb[1] * bmp->bmp_info_header.width + Pb[0]) + 3];
                    set_pixel(P.x + x_VP, P.y - y_VP, qRgba(int(intensity * col[2]),
                              int(intensity * col[1]),
                            int(intensity * col[0]), alp));
                } else
                {
                    set_pixel(P.x + x_VP, P.y - y_VP, qRgba(colorR, colorG, colorB, alp));
                }
            }
        }
    }
}

void dglWidget::triangle_scanline_barycentric(std::pair<vec3f, float> w0,
                                              std::pair<vec3f, float> w1,
                                              std::pair<vec3f, float> w2,
                                              vec2f b0, vec2f b1, vec2f b2,
                                              int colorR, int colorG, int colorB, float alp,
                                              BMP* bmp, float intensity)
{
    colorR = static_cast<int>(colorR * intensity);
    colorG = static_cast<int>(colorG * intensity);
    colorB = static_cast<int>(colorB * intensity);

    std::function viewport_f = ([](vec3f p, int m_width, int m_height)
    {
            vec3f ans = {0, 0, 0};
            ans[0] = m_width * (p[0] + 1) / 2;
    ans[1] = m_height * (p[1] + 1) / 2;
    ans[2] = 10000 * (p[2] + 1) / 2;
    return ans;
});
vec3i t0 = viewport_f(w0.first / w0.second, m_width, m_height);
vec3i t1 = viewport_f(w1.first / w1.second, m_width, m_height);
vec3i t2 = viewport_f(w2.first / w2.second, m_width, m_height);
//if (t0.y - t1.y < eps && t0.y - t2.y < eps) return;
using std::swap;
if (t0.get_y() > t1.get_y())
{
    swap(t0, t1);
    swap(w0, w1);
    swap(b0, b1);
}
if (t0.get_y() > t2.get_y())
{
    swap(t0, t2);
    swap(w0, w2);
    swap(b0, b2);
}
if (t1.get_y() > t2.get_y())
{
    swap(t1, t2);
    swap(w1, w2);
    swap(b1, b2);
}

vec3i pts[3]  = {t0, t1, t2};
vec2i pts2[3] = {{t0.x, t0.y}, {t1.x, t1.y}, {t2.x, t2.y}};

int total_height = t2.y - t0.y;
int beginh = std::abs(std::max(0, t0.y) - t0.y);
int endh = std::abs(std::min(m_height - 1, t2.y) - t2.y);
for (int i = 0 + beginh; i < total_height - endh; i++)
{
    bool second_half = i > t1.y - t0.y || t1.y == t0.y;
    int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
    float alpha = (float) i / total_height;
    float beta = (float) (i - (second_half ? t1.y - t0.y : 0)) / segment_height;
    vec3i A =  t0 + (t2 - t0) * alpha;
    vec3i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
    if (A.x > B.x)
    {
        std::swap(A, B);
        //std::swap(Ab, Bb);
    }
    for (int j = std::max(A.x, 0); j <= std::min(B.x, m_width - 1); j++)
    {
        vec2i P;
        P.x = j;
        P.y = t0.y + i;
        vec3f bc_screen  = barycentric(pts2[0], pts2[1], pts2[2], P);
        vec3f bc_clip    = vec3f(bc_screen.x/w0.second, bc_screen.y/w1.second, bc_screen.z/w2.second);
        bc_clip = bc_clip / (bc_clip.x+bc_clip.y+bc_clip.z);
        //vec3f ans = pts[0] * bc_screen[0] + pts[1] * bc_screen[1] + pts[2] * bc_screen[2];
        vec3f ans = pts[0] * bc_clip[0] + pts[1] * bc_clip[1] + pts[2] * bc_clip[2]; //?!
        vec2i T = b0 * bc_clip[0] + b1 * bc_clip[1] + b2 * bc_clip[2];
        int idx = j + (t0.y + i) * m_width;
        if (ans.z > 10000 || ans.z < 0) continue;
        if (zbuffer[idx] > ans.z)
        {
            zbuffer[idx] = ans.z;
            if (bmp != nullptr)
            {
                vec3i col;
                uint32_t channels = bmp->bmp_info_header.bit_count / 8;
                unsigned int max_size_index = static_cast<unsigned int>(bmp->data.size() - 1);
                size_t index_col0 = std::min(max_size_index, channels * (T.y * bmp->bmp_info_header.width + T.x) + 0);
                size_t index_col1 = std::min(max_size_index, channels * (T.y * bmp->bmp_info_header.width + T.x) + 1);
                size_t index_col2 = std::min(max_size_index, channels * (T.y * bmp->bmp_info_header.width + T.x) + 2);
                col[0] = bmp->data[index_col0];   // B
                col[1] = bmp->data[index_col1];   // G
                col[2] = bmp->data[index_col2];   // R
                if (channels == 4) alp = bmp->data[channels * (T.y * bmp->bmp_info_header.width + T.x) + 3];
                set_pixel(P.x + x_VP, P.y - y_VP, qRgba(int(intensity * col[2]),
                          int(intensity * col[1]),
                        int(intensity * col[0]), alp));
            } else
            {
                set_pixel(P.x + x_VP, P.y - y_VP, qRgba(colorR, colorG, colorB, alp));
            }
        }
    }
}

}

void dglWidget::dgl_rotate(float angle, float x, float y, float z)
{
    vec3f axis{x, y, z};
    axis.normalize();
    x = axis.x;
    y = axis.y;
    z = axis.z;
    float angle_rad = angle * (float(M_PI) / 180.0f);

    float c = cosf(angle_rad);
    float s = sinf(angle_rad);
    float t = 1 - c;

    float m[16] = {
        c+x*x*t,y*x*t+z*s,z*x*t-y*s,0,
        x*y*t-z*s,c+y*y*t,z*y*t+x*s,0,
        x*z*t+y*s,y*z*t-x*s,z*z*t+c,0,
        0,0,0,1
    };

    Matrix<float> ro = Matrix<float>(m);
    model_view = ro * model_view;
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
    //xw_VP = x + w;
    //yh_VP = y + h;
    Matrix<float> Viewport = IdentityMatrix<float>(4, 4);
    Viewport[0][3] = x + w / 2.f;
    Viewport[1][3] = y + h / 2.f;
    Viewport[2][3] = (far + near) / 2.f;

    Viewport[0][0] = w / 2.f;
    Viewport[1][1] = h / 2.f;
    Viewport[2][2] = (far - near) / 2.f;
    viewport = Viewport;
}

/**
 * Pre: v0 -- v1
 *      \     \
 *      v3 -- v2
 */
void dglWidget::draw_quad(std::pair<vec3f, float> bv0,
                          std::pair<vec3f, float> bv1,
                          std::pair<vec3f, float> bv2,
                          std::pair<vec3f, float> bv3,
                          vec2f t0, vec2f t1, vec2f t2, vec2f t3,
                          int colorR, int colorG, int colorB, float alp, float intensity,
                          BMP* _bmp)
{
    vec2i tt0 = {int(t0[0] * 255), int(t0[1] * 255)};
    vec2i tt1 = {int(t1[0] * 255), int(t1[1] * 255)};
    vec2i tt2 = {int(t2[0] * 255), int(t2[1] * 255)};
    vec2i tt3 = {int(t3[0] * 255), int(t3[1] * 255)};

    //    vec2f tt0 = {t0[0] * 255, t0[1] * 255};
    //    vec2f tt1 = {t1[0] * 255, t1[1] * 255};
    //    vec2f tt2 = {t2[0] * 255, t2[1] * 255};
    //    vec2f tt3 = {t3[0] * 255, t3[1] * 255};

    triangle_scanline(bv1, bv2, bv3, tt1, tt2, tt3, colorR, colorB, colorG, alp, _bmp, intensity);
    triangle_scanline(bv0, bv1, bv3, tt0, tt1, tt3, colorR, colorB, colorG, alp, _bmp, intensity);

    //triangle_scanline_barycentric(bv1, bv2, bv3, tt1, tt2, tt3, colorR, colorB, colorG, alp, _bmp, intensity);
    //triangle_scanline_barycentric(bv0, bv1, bv3, tt0, tt1, tt3, colorR, colorB, colorG, alp, _bmp, intensity);

    //triangle_bbox_barycentric(bv1, bv2, bv3, tt1, tt2, tt3, colorR, colorB, colorG, alp, _bmp, intensity);
    //triangle_bbox_barycentric(bv0, bv1, bv3, tt0, tt1, tt3, colorR, colorB, colorG, alp, _bmp, intensity);
}


void dglWidget::perspective(const float &fovy,
                            const float &aspect,
                            const float &zNear,
                            const float &zFar)
{
    switch (_mode) {
    case PERSP:
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
        break;
    }
    case ORTHO:
    {
        float const rad = fovy * float(M_PI) / 180.f;
        float const tanHalfFovy = tanf(rad / 2.0f);
        Matrix<float> Result = IdentityMatrix<float>(4, 4);
        Result.put(0, 0, (1 / (aspect * tanHalfFovy * zNear)));
        Result.put(1, 1, (1 / (tanHalfFovy * zNear)));
        Result.put(2, 2, (-2 / (zFar - zNear)));
        Result.put(2, 3, (-(zFar + zNear) / (zFar - zNear)));
        Result.transpose();
        proj = Result;
        break;
    }
    }
}
