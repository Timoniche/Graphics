#include "dglgeometry.h"

template<>
template<>
DGL::vec3<int>::vec3(vec3<float> const &v) : x(int(v.get_x() + 0.5f)), y(int(v.get_y() + 0.5f)),
                                             z(int(v.get_z() + 0.5f))
{}

template<>
template<>
DGL::vec3<float>::vec3(vec3<int> const &v) : x(v.get_x()), y(v.get_y()), z(v.get_z())
{}

using std::swap;

void DGL::sort_vec2i_y(vec2i &t0, vec2i &t1, vec2i &t2)
{

    if (t0.get_y() > t1.get_y())
    { swap(t0, t1); }
    if (t0.get_y() > t2.get_y())
    { swap(t0, t2); }
    if (t1.get_y() > t2.get_y())
    { swap(t1, t2); }
}

void DGL::sort_vec3i_y(vec3i &t0, vec3i &t1, vec3i &t2)
{

    if (t0.get_y() > t1.get_y())
    { swap(t0, t1); }
    if (t0.get_y() > t2.get_y())
    { swap(t0, t2); }
    if (t1.get_y() > t2.get_y())
    { swap(t1, t2); }
}
