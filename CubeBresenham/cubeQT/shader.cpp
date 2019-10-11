#include "shader.h"

Shader::Shader(Matrix<float> const *model_view,
               Matrix<float> const *proj,
               int w, int h) : model_view(model_view),
    proj(proj),
    m_width(w), m_height(h)
{
    MVP = (*model_view) * (*proj);
}


vec3f Shader::count_coordinates(vec3f world_coordinates)
{
    Matrix<float> m = Matrix<float>(world_coordinates);
    m.transpose();
    m = m * MVP;
    vec3f p = m.get_projection();
    vec3f ans = {0, 0, 0};
    ans[0] = m_width * (p[0] + 1) / 2;
    ans[1] = m_height * (p[1] + 1) / 2;
    ans[2] = 10000 * (p[2] + 1) / 2;
    return ans;
}
