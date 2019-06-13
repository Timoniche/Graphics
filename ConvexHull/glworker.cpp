#include "glworker.h"
#include <limits>
#include <valarray>
#include <cmath>
#include <QDebug>

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

//________________________________________
GLWorker::GLWorker()
{

}

/**
 * O(n*logn)
 */
void GLWorker::graham(data_t const &points)
{
    emit send_bar_value(0);
    switch (points.size())
    {
    case 0:
    case 1:
        return;
    case 2:
        emit send_line(points[0].first, points[0].second,
                points[1].first, points[1].second);
        return;
    }
    float x0 = points[0].first;
    float y0 = points[0].second;

    data_t data_counter_clock_from_p0 = points;
    std::sort(data_counter_clock_from_p0.begin() + 1, data_counter_clock_from_p0.end(), [this, x0, y0](const std::pair<float, float>& a,
              const std::pair<float, float>& b)
    {
        float a_angle_p0 = a_tan2_from_0_to_pi(a.second - y0, a.first - x0);
        float b_angle_p0 = a_tan2_from_0_to_pi(b.second - y0, b.first - x0);
        if (equal(a_angle_p0, b_angle_p0))
        {
            float dis_a_p0 = distance_pow2(a.first, a.second, x0, y0);
            float dis_b_p0 = distance_pow2(b.first, b.second, x0, y0);
            return dis_a_p0 < dis_b_p0;
        } else
        {
            return a_angle_p0 < b_angle_p0;
        }
    });
    emit send_max_bar(data_counter_clock_from_p0.size());
    _stack.push(0);
    _stack.push(1);
    emit increase_bar(2);
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
        emit increase_bar(1);
    }
    float square = 0;
    for (size_t i = 1; i < _stack._data.size(); i++)
    {
        auto& p_prev = data_counter_clock_from_p0[_stack._data[i - 1]];
        auto& p_last = data_counter_clock_from_p0[_stack._data[i]];
        emit send_line(p_prev.first, p_prev.second, p_last.first, p_last.second);
        float left = (p_prev.first + p_last.first);
        float right = (p_last.second - p_prev.second);
        if (std::abs(left) > eps && std::abs(right) > eps)
        {
            square += (p_prev.first + p_last.first) * (p_last.second - p_prev.second);
        }
        qDebug() << square << '\n';
    }
    auto& p_prev = data_counter_clock_from_p0[_stack._data[0]];
    auto& p_last = data_counter_clock_from_p0[_stack._data[_stack._data.size() - 1]];
    emit send_line(p_prev.first, p_prev.second, p_last.first, p_last.second);
    emit send_square(square / 2);
    _stack._data.clear();
    return;
}

/**
 *
 */
bool GLWorker::left_directed(size_t next_to_top, size_t top, size_t pi, data_t const& data) const
{
    std::pair<float, float> u = { data[top].first - data[next_to_top].first,
                                  data[top].second - data[next_to_top].second };
    std::pair<float, float> v = { data[pi].first - data[top].first,
                                  data[pi].second - data[top].second };
    return u.first * v.second - u.second * v.first < -eps;
}

/**
 * Pre: points sorted ascending by y
 * O(n^2)
 */
void GLWorker::gift_wrapping(data_t const& points)
{
    if (points.empty())
    {
        return;
    }
    used_points.assign(points.size(), false);
    used_points[0] = true;
    /*
     * ': i |- i+1
     * 2S = sum(0, n - 1): (xi + xi') * (yi' - yi)
     */
    float square = 0;
    float x0 = points[0].first;
    float y0 = points[0].second;
    float x_next = x0;
    float y_next = y0;
    do
    {
        float x = x_next;
        float y = y_next;
        x_next = x0;
        y_next = y0;
        float min_angle = std::numeric_limits<float>::max();
        size_t index_used = 0;

        for (size_t j = 0; j < points.size(); j++)
        {
            if (!used_points[j] || j == 0) {
                float x1 = points[j].first;
                float y1 = points[j].second;
                if (!equal(x1, y1, x, y)) {
                    float angle = a_tan2(y1 - y, x1 - x, used_points[points.size() - 1]);
                    if (angle < min_angle)
                    {
                        min_angle = angle;
                        x_next = x1;
                        y_next = y1;
                        index_used = j;
                    } else if (std::abs(angle - min_angle) < eps)
                    {
                        if (distance_pow2(x, y, x1, y1) <
                                distance_pow2(x, y, x_next, y_next))
                        {
                            x_next = x1;
                            y_next = y1;
                            index_used = j;
                        }
                    }
                }
            }
        }
        used_points[index_used] = true;
        emit send_line(x, y, x_next, y_next);
        square += (x + x_next) * (y_next - y);
    }
    while (!equal(x0, y0, x_next, y_next));
    emit send_square(square / 2);
}

bool GLWorker::equal(float a, float b) const
{
    return std::abs(a - b) < eps;
}

bool GLWorker::equal(float x1, float y1, float x2, float y2) const
{
    return std::abs(x1 - x2) < eps && std::abs(y1 - y2) < eps;
}

float GLWorker::a_tan2_from_0_to_pi(float y, float x) const
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

float GLWorker::a_tan2(float y, float x, bool descending) const
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

float GLWorker::distance_pow2(float x1, float y1, float x2, float y2) const
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
