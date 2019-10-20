#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "matrix.h"
#include "dglgeometry.h"

using namespace DGL;

class Worker : public QObject
{
    Q_OBJECT
public:
    Matrix<float> const *model_view;
    Matrix<float> const *proj;
    Matrix<float> const *viewport;
    Matrix<float> MVP;

    Worker(Matrix<float> const *model_view,
           Matrix<float> const *proj,
           Matrix<float> const *viewport);

    signals:

    void draw_quad(vec3f
              v0,
              vec3f v1, vec3f
              v2,
              vec3f v3,
              int colorR,
              int colorG,
              int colorB,
              float alp
    );

    void finished();

public slots:

    void draw_cube();
};

#endif // WORKER_H
