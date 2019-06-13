#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QThread>

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
    void get_square_from(float sq);
    void random_points();
    void draw_line(float, float, float, float);
private:
    int m_points = 10;
    float m_radius = 0.5;
    float m_square = 0; 
};

#endif // GLWIDGET_H
