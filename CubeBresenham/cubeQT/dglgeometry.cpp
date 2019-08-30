#include "dglgeometry.h"

template<> template<> DGL::vec3<int>::vec3(vec3<float> const &v) : x(int(v.get_x() + 0.5f)), y(int(v.get_y() + 0.5f)), z(int(v.get_z() + 0.5f)) {}
template<> template<> DGL::vec3<float>::vec3(vec3<int> const &v) : x(v.get_x()), y(v.get_y()), z(v.get_z()) {}
