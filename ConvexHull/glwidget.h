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
protected:
    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;
signals:
    void add_points(float x, float y);
public slots:
    void random_points();
    void draw_line(float, float, float, float);
private:
    int m_points = 10;
    float m_radius = 0.5;
};

#endif // GLWIDGET_H
