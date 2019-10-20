#ifndef SHADER_H
#define SHADER_H

#include "matrix.h"
#include "dglgeometry.h"

using namespace DGL;

class Shader
{
    Matrix<float> const *model_view;
    Matrix<float> const *proj;
    Matrix<float> MVP;
    int m_width = 500;
    int m_height = 500;

public:
    Shader(Matrix<float> const *model_view,
           Matrix<float> const *proj,
           int w, int h);

    std::pair<vec3f, float> before_viewport(vec3f world_coordinates);

    vec3f count_coordinates(vec3f world_coordinates);

    vec3f reverse_coordinates(vec3f screen_coordinates);

};

#endif // SHADER_H
