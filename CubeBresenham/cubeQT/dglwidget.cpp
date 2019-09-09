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
    zbuffer = new float[size_t(m_width * m_height)];
    //todo: change buffer depth
    std::fill(zbuffer, zbuffer + m_width * m_height,
              std::numeric_limits<int>::min());
    paintEvent(nullptr);
}

dglWidget::~dglWidget()
{
    delete[] zbuffer;
    //    delete model_view;
    //    delete proj;
    //    delete viewport;
    //    delete MVP;
}

void dglWidget::test_draw3()
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

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Matrix<float> m  = (MVP) * Matrix<float>(cube[i][j]);
            cube[i][j] = m.get_projection();

        }
    }
    for (int i = 0; i < 6; i++)
    {
        draw_quad(cube[i], qRgba(10 * i, 20 * i, 30 * i, 255));
    }

}
void dglWidget::test_draw1()
{
    using std::endl, std::cout;
    //_________________________________________________________________________________
    //triangle_filled(z0[0], z0[1], z0[2], qRgba(255, 0, 0, 255));
    //triangle_filled(z1[0], z1[1], z1[2], qRgba(0, 255, 0, 255));
    //________________________________________________________________________________
    Matrix<float> m01  = Matrix<float>(z0[0]);
    Matrix<float> m02  = Matrix<float>(z0[1]);
    Matrix<float> m03  = Matrix<float>(z0[2]);
    Matrix<float> m11  = Matrix<float>(z1[0]);
    Matrix<float> m12  = Matrix<float>(z1[1]);
    Matrix<float> m13  = Matrix<float>(z1[2]);
    //________________________________________________________________________________
    //Whole plan: Viewport * Projection * [View * Model] * v.
    dgl_look_at(m_eye, m_center, m_up);
    std::cout << "ModelView" << std::endl;
    std::cout << model_view << std::endl;
    // Matrix<float> proj = perspective(45.0f * float(M_PI) / 180.0f, 4.0f / 3.0f, near, far);
    //Matrix<float> proj = dgl_projection(-1.f/(m_eye-m_center).norm());
    projj(90.0f, near, far);
    std::cout << "PROJ" << std::endl;
    std::cout << proj << std::endl;

    std::cout << "VIEWPORT" << std::endl;
    dgl_viewport(0, 0, m_width, m_height);

    std::cout << viewport << std::endl;

    std::cout << "ProjModel" << std::endl;
    Matrix<float> total = proj * (model_view);
    std::cout << total << std::endl;
    //______________________________________________________________________________
    std::cout << "Before viewport -- draw only [-1; 1] x [-1; 1] x [-1; 1]" << std::endl;
    Matrix<float> a  = total * m01;
    Matrix<float> b  = total * m02;
    Matrix<float> c  = total * m03;
    Matrix<float> aa  = total * m11;
    Matrix<float> bb  = total * m12;
    Matrix<float> cc = total * m13;
    vec3f NNz0[3] = {a.get_projection(), b.get_projection(), c.get_projection()};
    vec3f NNz1[3] = {aa.get_projection(), bb.get_projection(), cc.get_projection()};
    cout << NNz0[0] << endl << NNz0[1] << endl << NNz0[2] << endl << endl;
    cout << NNz1[0] << endl << NNz1[1] << endl << NNz1[2] << endl;
    //______________________________________________________________________________
    std::cout << "ViewProjModel" << std::endl;
    total = viewport * total;
    std::cout << total << std::endl;

    Matrix<float> tm01  = total * m01;
    Matrix<float> tm02  = total * m02;
    Matrix<float> tm03  = total * m03;
    Matrix<float> tm11  = total * m11;
    Matrix<float> tm12  = total * m12;
    Matrix<float> tm13  = total * m13;
    //_________________________________________________________________________________
    vec3f Nz0[3] = {tm01.get_projection(), tm02.get_projection(), tm03.get_projection()};
    vec3f Nz1[3] = {tm11.get_projection(), tm12.get_projection(), tm13.get_projection()};
    cout << Nz0[0] << endl << Nz0[1] << endl << Nz0[2] << endl << endl;
    cout << Nz1[0] << endl << Nz1[1] << endl << Nz1[2] << endl;
    triangle_filled(Nz0[0], Nz0[1], Nz0[2], qRgba(255, 0, 0, 255));
    triangle_filled(Nz1[0], Nz1[1], Nz1[2], qRgba(0, 255, 0, 255));
    //________________________________________________________________________________
}

//culling
void dglWidget::test_draw2(vec3f* tr1)
{
    vec2i screen_coords[3];
    for (int j=0; j<3; j++) {
        screen_coords[j] = vec2i(int((tr1[j][0]+1.)*m_width/2.), int((tr1[j][1]+1.)*m_height/2.));
    }
    vec3f n = (tr1[2] - tr1[0]) ^ (tr1[1] - tr1[0]);
    n.normalize();
    m_light.normalize();
    std::cout << m_light;
    float intensity = n * m_light;
    std::cout << intensity << std::endl;
    if (intensity>0) {
        triangle_filled(screen_coords[0], screen_coords[1], screen_coords[2], qRgba(intensity*255, intensity*255, intensity*255, 255));
    }
}

void dglWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    using std::endl, std::cout;
    QPainter painter{this};
    painter.drawImage(0, 0, *m_image);

    dgl_look_at(m_eye, m_center, m_up);
    projj(90.0f, near, far);
    dgl_viewport(0, 0, m_width, m_height);
    MVP = *new Matrix<float>(((viewport) * (proj) * (model_view)));

    test_draw3();
    //test_draw1();
    //test_draw2(tr1);
    //test_draw2(tr2);
    //test_draw3();
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
        for (int j = A.get_x(); j <= B.get_x(); j++)
        {
            set_pixel(j, y, color);
        }
    }
}

void dglWidget::triangle_filled(vec3i t0, vec3i t1, vec3i t2, QRgb color)
{
    using std::swap;
    if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y()) { return; }
    sort_vec3i_y(t0, t1, t2);
    int total_height = t2.get_y() - t0.get_y();
    for (int y = 0; y < total_height; y++) {
        bool second_half = y >= t1.get_y() - t0.get_y();
        int segment_height = second_half ? t2.get_y() - t1.get_y() : t1.get_y() - t0.get_y();
        float alpha = static_cast<float>(y) / total_height;
        float beta  = static_cast<float>(y - (second_half ? t1.get_y()-t0.get_y() : 0)) / segment_height;
        vec3f A = t0 + vec3f(t2 - t0) * alpha;
        vec3f B = second_half ? t1 + vec3f(t2 - t1) * beta : t0 + vec3f(t1 - t0) * beta;
        if (A.get_x() > B.get_x()) { swap(A, B); }
        for (int j = int(A.get_x()); j <= B.get_x(); j++) {
            float phi = (B.get_x() - A.get_x() <= eps) ? 1.0f : static_cast<float>(j - A.get_x()) /
                                                         static_cast<float>(B.get_x() - A.get_x());
            vec3f P = vec3f(A) + vec3f(B - A) * phi;
            int idx = int(P.get_x() + P.get_y() * m_width);
            if (P.get_x() >= m_width ||
                    P.get_y() >= m_height ||
                    P.get_x() < 0 ||
                    P.get_y() < 0) { continue; }
            if (zbuffer[idx] < P.get_z()) {
                zbuffer[idx] = P.get_z();
                set_pixel(int(P.get_x()), int(P.get_y()), color);
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
 * \1 0 0 0\
 * \0 1 0 0\
 * \0 0 1 0\
 * \X Y Z 1\
 *  we take X = (-1) * (center!?)eye[0], Y = -eye[1... to move world to camera
 */
//0.995037 -0.00985234 0.0990148 0 0 0.995086 0.0990148 0 -0.0995037 -0.0985234 0.990148 0 -2.98511 -2.9557 -20.7931 1 from OpenGL
//approved
void dglWidget::dgl_look_at(vec3f eye, vec3f center, vec3f up)
{
    //    vec3f z_n = (eye - center).normalize();
    //    vec3f x_n = (up ^ z_n).normalize();
    //    vec3f y_n = (z_n ^ x_n).normalize();

    //    Matrix<float> B = IdentityMatrix<float>(4, 4);
    //    Matrix<float> TR = IdentityMatrix<float>(4, 4);

    //    for (unsigned int i = 0; i < 3; i++) {
    //        B.put(i, 0, x_n[i]);
    //        B.put(i, 1, y_n[i]);
    //        B.put(i, 2, z_n[i]);
    //        TR.put(3, i, -eye[i]);
    //    }
    //    model_view = new Matrix<float>(B * TR);
    vec3f z = (eye-center).normalize();
    vec3f x = (up ^ z).normalize();
    vec3f y = (z ^ x).normalize();
    Matrix<float> Minv = IdentityMatrix<float>(4,4);
    Matrix<float> Tr   = IdentityMatrix<float>(4,4);
    for (unsigned int i=0; i<3; i++) {
        Minv.put(0,i,x[i]);
        Minv.put(1,i, y[i]);
        Minv.put(2, i, z[i]);
        //Tr.put(i, 3, -center[i]);
        Tr.put(i, 3, -eye[i]);
    }
    model_view = *new Matrix<float>(Minv*Tr);
}

/***
 * Viewport matrix: [-1,1]x[-1,1]x[-1,1] -> [x,x+w]x[y,y+h]x[n,f]
 * \w/2 0   0   [x+w/2]\
 * \0   h/2 0   [y+h/2]\
 * \0   0   (f-n)/2  (f+n)/2   \
 * \0   0   0     1    \
*/
//w:1024 h:512
void dglWidget::dgl_viewport(int x, int y, int w, int h)
{
    Matrix<float> Viewport = *new IdentityMatrix<float>(4, 4);
    //    ret.put(0, 0, w / 2.f);
    //    ret.put(0, 3, (x + w / 2.f));

    //    ret.put(1, 1, h / 2.f);
    //    ret.put(1, 3, (y + h / 2.f));

    //    ret.put(2, 2, (far - near) / 2.f);
    //    ret.put(2, 3, (far + near) / 2.f);
    //    ret.transpose();//todo: eliminate
    Viewport[0][3] = x+w/2.f;
    Viewport[1][3] = y+h/2.f;

    Viewport[2][3] = (far + near) / 2.f;  //Viewport[2][3] = 1.f;
    Viewport[0][0] = w/2.f;
    Viewport[1][1] = h/2.f;
    Viewport[2][2] = (far - near) / 2.f; //Viewport[2][2] = 0;
    viewport = Viewport;
}

/**
 * @param c - distance between camera and the origin
 * Projection matrix:
 * \1 0 0    0\
 * \0 1 0    0\
 * \0 0 1    0\
 * \0 0 -1/c 1\
 */
Matrix<float> dglWidget::dgl_projection(float c)
{
    Matrix<float> ret = IdentityMatrix<float>(4, 4);
    ret.put(3, 2, c);
    return ret;
}


//1.81066 0 0 0 0 2.41421 0 0 0 0 -1.0202 -1 0 0 -2.0202 0 fromOpenGL
//approved
Matrix<float> dglWidget::perspective
(
        float const & fovy,
        float const & aspect,
        float const & zNear,
        float const & zFar
        )
{

    float const rad = fovy;
    float const tanHalfFovy = tanf(rad / 2.0f);

    Matrix<float> Result(4, 4);
    Result.put(0, 0, (1 / (aspect * tanHalfFovy)));
    Result.put(1, 1, (1 / (tanHalfFovy)));
    Result.put(2, 2, (-(zFar + zNear) / (zFar - zNear)));
    Result.put(3, 2, (-1));
    Result.put(2, 3, (-(2 * zFar * zNear) / (zFar - zNear)));
    Result.transpose();
    return Result;
}

void dglWidget::draw_quad(vec3f* v, QRgb color)
{
    triangle_filled(v[0], v[1], v[2], color);
    triangle_filled(v[0], v[2], v[3], color);
}

void dglWidget::projj(const float &angleOfView, const float &near, const float &far)
{
    // set the basic projection matrix
    Matrix<float> M = *(new Matrix<float>(4, 4));
    float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
    M[0][0] = scale; // scale the x coordinates of the projected point
    M[1][1] = scale; // scale the y coordinates of the projected point
    M[2][2] = -far / (far - near); // used to remap z to [0,1]
    M[3][2] = -far * near / (far - near); // used to remap z [0,1]
    M[2][3] = -1; // set w = -z
    M[3][3] = 0;
    proj = M;
}


