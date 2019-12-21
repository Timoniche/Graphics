#include "hulllib.h"
#include <cmath>
#include <algorithm>

HullLib::HullLib()
{
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

    data_counter_clock_from_p0.begin()->swap(*it_to_min_y_element);
    float x0 = data_counter_clock_from_p0[0].x;
    float y0 = data_counter_clock_from_p0[0].y;
    std::sort(data_counter_clock_from_p0.begin() + 1, data_counter_clock_from_p0.end(), [this, x0, y0](const Point &a,
                                                                                                 const Point &b)
    {
        Point at{a.y - y0, a.x - x0};
        Point bt{b.y - y0, b.x - x0};
        float cross = at.x * bt.y - at.y * bt.x;
        if (equal(cross, 0.f))
        {
            float dis_a_p0 = distance_pow2(a.x, a.y, x0, y0);
            float dis_b_p0 = distance_pow2(b.x, b.y, x0, y0);
            return dis_a_p0 < dis_b_p0;
        } else
        {
            return cross < 0;
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

vector_of_points const& HullLib::gift_wrapping(vector_of_points const& points)
{
    vector_of_points* ret = new vector_of_points(points);
    if (ret->empty())
    {
        return *ret;
    }
    auto it_to_min_y_element = std::min_element(ret->begin(),
                                                ret->end(),
                                                [](const Point &a, const Point &b)
    {
        return a.y < b.y;
    });
    auto it_to_max_y_element = std::max_element(ret->begin(),
                                                ret->end(),
                                                [](const Point &a, const Point &b)
    {
        return a.y < b.y;
    });
    Point max_point = *it_to_max_y_element;
    ret->begin()->swap(*it_to_min_y_element);
    size_t _count = 1;
    bool _descending = false;
    float x0 = ret->operator[](0).x;
    float y0 = ret->operator[](0).y;
    float x_next = x0;
    float y_next = y0;
    ret->emplace_back(x0, y0);
    do
    {
        size_t index_min = 0;
        float x = x_next;
        float y = y_next;
        x_next = x0;
        y_next = y0;
        float min_angle = std::numeric_limits<float>::max();
        for (size_t j = _count; j < ret->size(); j++)
        {
            float x1 = ret->operator[](j).x;
            float y1 = ret->operator[](j).y;
            if (!equal(x1, y1, x, y)) {
                float angle = a_tan2(y1 - y, x1 - x, _descending);
                if (angle < min_angle)
                {
                    min_angle = angle;
                    x_next = x1;
                    y_next = y1;
                    index_min = j;
                } else if (std::abs(angle - min_angle) < eps)
                {
                    if (distance_pow2(x, y, x1, y1) <
                            distance_pow2(x, y, x_next, y_next))
                    {
                        x_next = x1;
                        y_next = y1;
                        index_min = j;
                    }
                }
            }
        }
        if (equal(x_next, y_next, max_point.x, max_point.y))
        {
            _descending = true;
        }
        ret->emplace_back(x_next, y_next);
        std::swap(ret->operator[](_count), ret->operator[](index_min));
        _count++;
    }
    while (!equal(x0, y0, x_next, y_next));
    ret->resize(_count);
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

float HullLib::a_tan2_from_0_to_2pi(float y, float x) const
{
    float tmp = atan2f(y, x);
    if (tmp < 0)
    {
        tmp += PI_F * 2;
    }
    return tmp;
}

float HullLib::a_tan2(float y, float x, bool descending) const
{
    float tmp = a_tan2_from_0_to_2pi(y, x);
    if (!descending) {
        return tmp;
    } else {
        if (tmp >= PI_F)
        {
            tmp -= PI_F;
            return tmp;
        } else
        {
            tmp += PI_F;
            return tmp;
        }
    }

}

float HullLib::distance_pow2(float x1, float y1, float x2, float y2) const
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
