#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QThread>

#include "hulllib.h"

typedef std::vector<PRIMITIVES::Point> vector_of_points;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public: 
    explicit GLWidget(QWidget *parent = nullptr);
    ~GLWidget() override;
    float get_square();
protected:
    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;
signals:
    void add_points(float x, float y);
public slots:
    void set_radius(float);
    void set_points(int);
    void get_square_from(float);
    void random_points();
    void draw_line(float, float, float, float, float, float, float, float);
    void draw_vector_lines(vector_of_points const&);
private:
    int m_points = 10;
    float m_radius = 0.5;
    float m_square = 0;
};

#endif // GLWIDGET_H
