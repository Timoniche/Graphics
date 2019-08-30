#ifndef DGLGEOMETRY_H
#define DGLGEOMETRY_H

#include <cmath>

namespace DGL {

template<typename T> struct vec2;
template<typename T> struct vec3;

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;

typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<int> vec3i;

//_______________________________________________________________________
template<typename T> struct vec2
{
private:
    T x;
    T y;

public:
    vec2() : x(0), y(0) {}
    vec2(T x, T y) : x(x), y(y) {}
public:
    T get_x() const { return x; }
    T get_y() const { return y; }
    T& operator[](const int i)
    {
        if (i == 0) return x;
        return y;
    }

    vec2<T> operator +(const vec2<T> &rhs) const
    {
        return vec2<T>(x + rhs.x, y + rhs.y);
    }
    vec2<T> operator -(const vec2<T> &rhs) const
    {
        return vec2<T>(x - rhs.x, y - rhs.y);
    }
    vec2<T> operator *(float k) const
    {
        return vec2<T>(x * k, y * k);
    }
};

//_______________________________________________________________________
template<typename T> struct vec3
{
private:
    T x;
    T y;
    T z;

public:
    vec3() : x(0), y(0), z(0) {}
    vec3(T x, T y, T z) : x(x), y(y), z(z) {}
    template<typename U> vec3<T>(vec3<U> const &v);
public:
    T get_x() const { return x; }
    T get_y() const { return y; }
    T get_z() const { return z; }
    T& operator[](const int i)
    {
        switch (i)
        {
            case 0: return x;
            case 1: return y;
        }
        return z;
    }

    vec3<T> operator +(const vec3<T> &rhs) const
    {
        return vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    vec3<T> operator -(const vec3<T> &rhs) const
    {
        return vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    vec3<T> operator *(float k) const
    {
        return vec3<T>(x * k, y * k, z * k);
    }
    T operator *(const vec3<T> &rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }
    vec3<T> operator ^(const vec3<T> &rhs) const
    {
        return vec3<T>(y * rhs.z - z * rhs.y,
                       z * rhs.x - x * rhs.z,
                       x * rhs.y - y * rhs.x);
    }
    float norm() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    vec3<T>& normalize()
    {
        return *this = (*this) * (1.f / norm());
    }

};
template<> template<> vec3<int>::vec3(vec3<float> const &v);
template<> template<> vec3<float>::vec3(vec3<int> const &v);

void sort_vec2i_y(vec2i& t0, vec2i& t1, vec2i& t2);
void sort_vec3i_y(vec3i& t0, vec3i& t1, vec3i& t2);
//_______________________________________________________________________

}
#endif // DGLGEOMETRY_H
