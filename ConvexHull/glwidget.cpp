#include "glwidget.h"
#include <GL/glu.h>
#include <qgl.h>
#include <cstdlib>
#include <random>
#include <cmath>

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {}

GLWidget::~GLWidget() = default;

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

void GLWidget::random_points()
{
    makeCurrent();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(3);
    glBegin(GL_POINTS);
    for (int i = 0; i < m_points; i++)
    {
        /*
         * x2 + y2 <= r2
         * y belongs sqrt|r2 - x2|
         */
        float x = -m_radius + (float(rand()) * m_radius) / (RAND_MAX / 2);
        float border = sqrt(m_radius * m_radius - x * x);
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

void GLWidget::draw_line(float x1, float y1,
                         float x2, float y2)
{
    makeCurrent();
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}











