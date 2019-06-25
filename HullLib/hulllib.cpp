#include "hulllib.h"
#include <cmath>
#include <algorithm>

HullLib::HullLib()
{
}

Point::Point(float x, float y) : x(x), y(y) {}

void StackExtended::push(size_t x)
{
    _data.push_back(x);
}
int StackExtended::top()
{
    if (_data.size() == 0) { return -1; }
    return int(_data[_data.size() - 1]);
}
int StackExtended::top_next()
{
    if (_data.size() < 2) { return -1; }
    return int(_data[_data.size() - 2]);
}
int StackExtended::pop()
{
    if (_data.size() == 0) { return -1; }
    int tmp = top();
    _data.pop_back();
    return tmp;
}



vector_of_points const& HullLib::graham(vector_of_points const &points)
{
    vector_of_points* ret = new vector_of_points();
    switch (points.size())
    {
    case 0:
    case 1:
        return *ret;
    case 2:
        ret->emplace_back(points[0].x, points[0].y);
        ret->emplace_back(points[1].x, points[1].y);
        return *ret;
    }
    vector_of_points data_counter_clock_from_p0 = points;
    auto it_to_min_y_element = std::min_element(data_counter_clock_from_p0.begin(),
                               data_counter_clock_from_p0.end(),
                               [](const Point &a, const Point &b)
    {
        return a.y < b.y;
    });

    std::swap(*data_counter_clock_from_p0.begin(), *it_to_min_y_element);
    float x0 = data_counter_clock_from_p0[0].x;
    float y0 = data_counter_clock_from_p0[0].y;
    std::sort(data_counter_clock_from_p0.begin() + 1, data_counter_clock_from_p0.end(), [this, x0, y0](const Point& a,
              const Point& b)
    {
        float a_angle_p0 = a_tan2_from_0_to_pi(a.y - y0, a.x - x0);
        float b_angle_p0 = a_tan2_from_0_to_pi(b.y - y0, b.x - x0);
        if (equal(a_angle_p0, b_angle_p0))
        {
            float dis_a_p0 = distance_pow2(a.x, a.y, x0, y0);
            float dis_b_p0 = distance_pow2(b.x, b.y, x0, y0);
            return dis_a_p0 < dis_b_p0;
        } else
        {
            return a_angle_p0 < b_angle_p0;
        }
    });
    _stack.push(0);
    _stack.push(1);
    for (size_t pi = 2; pi < data_counter_clock_from_p0.size(); pi++)
    {
        int _next_to_top = _stack.top_next();
        Q_ASSERT(_next_to_top != -1);
        int _top = _stack.top();
        Q_ASSERT(_top != -1);
        while (left_directed(size_t(_next_to_top), size_t(_top), pi, data_counter_clock_from_p0))
        {
            _stack.pop();
            _next_to_top = _stack.top_next();
            Q_ASSERT(_next_to_top != -1);
            _top = _stack.top();
            Q_ASSERT(_top != -1);
        }
        _stack.push(pi);
    }
    for (size_t i = 0; i < _stack._data.size(); i++)
    {
        ret->push_back(data_counter_clock_from_p0[_stack._data[i]]);
    }
    _stack._data.clear();
    return *ret;
}

bool HullLib::left_directed(size_t next_to_top, size_t top, size_t pi, vector_of_points const& data) const
{
    Point u = { data[top].x - data[next_to_top].x,
                data[top].y - data[next_to_top].y };
    Point v = { data[pi].x - data[top].x,
                data[pi].y - data[top].y };
    return u.x * v.y - u.y * v.x < -eps;
}

bool HullLib::equal(float a, float b) const
{
    return std::abs(a - b) < eps;
}

bool HullLib::equal(float x1, float y1, float x2, float y2) const
{
    return std::abs(x1 - x2) < eps && std::abs(y1 - y2) < eps;
}

float HullLib::a_tan2_from_0_to_pi(float y, float x) const
{
    float F_PI_2 = static_cast<float>(M_PI_2);
    float F_PI = static_cast<float>(M_PI);
    float tmp = static_cast<float>(atan2(static_cast<double>(y), static_cast<double>(x)));
    if (std::abs(x) < eps)
    {
        if (y > 0)
        {
            tmp = F_PI_2;
        } else if (y < 0)
        {
            tmp = -F_PI_2;
        } else {
            tmp = 0;
        }
    }
    if (tmp < 0)
    {
        tmp += F_PI * 2;
    }
    return tmp;
}

float HullLib::a_tan2(float y, float x, bool descending) const
{
    float F_PI_2 = static_cast<float>(M_PI_2);
    float F_PI = static_cast<float>(M_PI);
    float tmp = static_cast<float>(atan2(static_cast<double>(y), static_cast<double>(x)));
    if (std::abs(x) < eps)
    {
        if (y > 0)
        {
            tmp = F_PI_2;
        } else if (y < 0)
        {
            tmp = -F_PI_2;
        } else {
            tmp = 0;
        }
    }
    if (tmp < 0)
    {
        tmp += F_PI * 2;
    }

    if (!descending) {
        return tmp;
    } else {
        if (tmp >= F_PI)
        {
            tmp -= F_PI;
            return tmp;
        } else
        {
            tmp += F_PI;
            return tmp;
        }
    }

}

float HullLib::distance_pow2(float x1, float y1, float x2, float y2) const
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
