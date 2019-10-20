#include "worker.h"
#include "matrix.h"
#include "dglgeometry.h"
#include "shader.h"

Worker::Worker(Matrix<float> const *model_view,
               Matrix<float> const *proj,
               Matrix<float> const *viewport) : model_view(model_view),
                                                proj(proj), viewport(viewport)
{
    qRegisterMetaType<vec3f>("vec3f");
    MVP = (*viewport) * (*proj) * (*model_view);
}

void Worker::draw_cube()
{
}
