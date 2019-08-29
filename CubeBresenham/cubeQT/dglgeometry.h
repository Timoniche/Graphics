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
    T getX() const { return x; }
    T getY() const { return y; }

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
public:
    T getX() const { return x; }
    T getY() const { return y; }
    T getZ() const { return z; }

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
//_______________________________________________________________________

}
#endif // DGLGEOMETRY_H
