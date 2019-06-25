#include "glwidget.h"
#include <GL/glu.h>
#include <qgl.h>
#include <cstdlib>
#include <random>
#include <cmath>

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {}

GLWidget::~GLWidget() = default;

float GLWidget::get_square()
{
    return m_square;
}

void GLWidget::get_square_from(float sq)
{
    m_square = sq;
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::set_radius(float r)
{
    m_radius = r;
}

void GLWidget::set_points(int p)
{
    m_points = p;
}

void GLWidget::random_points()
{
    makeCurrent();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(3);
    glBegin(GL_POINTS);
    glColor3f(1, 1, 1);
    for (int i = 0; i < m_points; i++)
    {
        /*
         * x2 + y2 <= r2
         * y belongs sqrt|r2 - x2|
         */
        float x = -m_radius + (float(rand()) * m_radius) / (RAND_MAX / 2);
        float border = static_cast<float>(sqrt(static_cast<double>(m_radius * m_radius - x * x)));
        float y = -border + (float(rand()) * border) / (RAND_MAX / 2);
        glVertex2f(x, y);
        emit add_points(x, y);
    }
    glEnd();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::draw_vector_lines(vector_of_points const& data)
{
    makeCurrent();
    glLineWidth(40);
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (auto& p : data)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void GLWidget::draw_line(float x1, float y1,
                         float x2, float y2)
{
    makeCurrent();
    glLineWidth(5);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}











