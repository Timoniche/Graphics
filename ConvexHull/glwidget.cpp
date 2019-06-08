#include "glwidget.h"
#include <GL/glu.h>
#include <qgl.h>
#include <cstdlib>
#include <random>

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
    for (int i = 0; i < 1; i++)
    {
        double x = -1 + double(rand()) / (RAND_MAX / 2);
        double y = -1 + double(rand()) / (RAND_MAX / 2);
        glVertex2d(x, y);
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

void GLWidget::draw_line(double x1, double y1,
                         double x2, double y2)
{
    makeCurrent();
    glBegin(GL_LINES);
        glVertex2d(x1, y1);
        glVertex2d(x2, y2);
    glEnd();
}











