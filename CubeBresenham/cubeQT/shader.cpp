#include "shader.h"

Shader::Shader(Matrix<float> const* model_view,
       Matrix<float> const* proj,
       Matrix<float> const* viewport) : model_view(model_view),
    proj(proj), viewport(viewport)
{
    MVP = (*viewport) * (*proj) * (*model_view);
}


vec3f Shader::count_coordinates(vec3f world_coordinates)
{
    Matrix<float> m = (MVP) * Matrix<float>(world_coordinates);
    return m.get_projection();
}
