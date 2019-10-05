#ifndef DGLGEOMETRY_H
#define DGLGEOMETRY_H

#include <cmath>
#include <iostream>

namespace DGL
{

    template<typename T>
    struct vec2;
    template<typename T>
    struct vec3;

    typedef vec2<float> vec2f;
    typedef vec2<double> vec2d;
    typedef vec2<int> vec2i;

    typedef vec3<float> vec3f;
    typedef vec3<double> vec3d;
    typedef vec3<int> vec3i;

//______________________________________________________________________________________________________________________
    template<typename T>
    struct vec2
    {
    private:
        T x;
        T y;

    public:
        vec2() : x(0), y(0)
        {}

        vec2(T x, T y) : x(x), y(y)
        {}

    public:
        T get_x() const
        { return x; }

        T get_y() const
        { return y; }

        T &operator[](const size_t i)
        {
            if (i == 0) return x;
            return y;
        }

        vec2<T> operator+(const vec2<T> &rhs) const
        {
            return vec2<T>(x + rhs.x, y + rhs.y);
        }

        vec2<T> operator-(const vec2<T> &rhs) const
        {
            return vec2<T>(x - rhs.x, y - rhs.y);
        }

        vec2<T> operator*(float k) const
        {
            return vec2<T>(x * k, y * k);
        }
    };

//______________________________________________________________________________________________________________________
    template<typename T>
    struct vec3
    {
    public:
        T x;
        T y;
        T z;

    public:
        vec3() : x(0), y(0), z(0)
        {}

        vec3(T x, T y, T z) : x(x), y(y), z(z)
        {}

        template<typename U>
        vec3<T>(vec3<U> const &v);

    public:
        T get_x() const
        { return x; }

        T get_y() const
        { return y; }

        T get_z() const
        { return z; }

        T &operator[](const size_t i)
        {
            switch (i)
            {
                case 0:
                    return x;
                case 1:
                    return y;
            }
            return z;
        }

        vec3<T> operator+(const vec3<T> &rhs) const
        {
            return vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        vec3<T> operator-(const vec3<T> &rhs) const
        {
            return vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        vec3<T> operator*(float k) const
        {
            return vec3<T>(x * k, y * k, z * k);
        }

        T operator*(const vec3<T> &rhs) const
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        vec3<T> operator^(const vec3<T> &rhs) const
        {
            return vec3<T>(y * rhs.z - z * rhs.y,
                           z * rhs.x - x * rhs.z,
                           x * rhs.y - y * rhs.x);
        }

        float norm() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        vec3<T> &normalize()
        {
            return *this = (*this) * (1.f / norm());
        }

        template<typename U>
        friend std::ostream &operator<<(std::ostream &s, vec3<U> &v);

    };

    template<>
    template<>
    vec3<int>::vec3(vec3<float> const &v);

    template<>
    template<>
    vec3<float>::vec3(vec3<int> const &v);

    template<typename T>
    std::ostream &operator<<(std::ostream &s, vec3<T> &v)
    {
        s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
        return s;
    }

    void sort_vec2i_y(vec2i &t0, vec2i &t1, vec2i &t2);

    template<typename T>
    void sort_vec3_y(vec3<T> &t0, vec3<T> &t1, vec3<T> &t2)
    {
        using std::swap;
        if (t0.get_y() > t1.get_y())
        { swap(t0, t1); }
        if (t0.get_y() > t2.get_y())
        { swap(t0, t2); }
        if (t1.get_y() > t2.get_y())
        { swap(t1, t2); }
    }

//______________________________________________________________________________________________________________________

}
#endif // DGLGEOMETRY_H
