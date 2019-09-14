#ifndef SHADER_H
#define SHADER_H

#include "matrix.h"
#include "dglgeometry.h"

using namespace DGL;

class Shader
{
    Matrix<float> const* model_view;
    Matrix<float> const* proj;
    Matrix<float> const* viewport;
    Matrix<float> MVP;

public:
    Shader(Matrix<float> const* model_view,
           Matrix<float> const* proj,
           Matrix<float> const* viewport);

    vec3f count_coordinates(vec3f world_coordinates);

};

#endif // SHADER_H
