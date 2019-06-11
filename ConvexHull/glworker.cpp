#include "glworker.h"
#include <limits>
#include <valarray>
#include <cmath>


GLWorker::GLWorker()
{

}

/**
 * Pre: points sorted ascending by y
 */
void GLWorker::gift_wrapping(data_t const& points)
{
    if (points.empty())
    {
        return;
    }
    used_points.assign(points.size(), false);
    used_points[0] = true;
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
    }
    while (!equal(x0, y0, x_next, y_next));
}

bool GLWorker::equal(float x1, float y1, float x2, float y2)
{
    return std::abs(x1 - x2) < eps && std::abs(y1 - y2) < eps;
}

float GLWorker::a_tan2(float y, float x, bool descending)
{

    float tmp = atan2(y, x);
    if (std::abs(x) < eps)
    {
        if (y > 0)
        {
            tmp = float(M_PI_2);
        } else if (y < 0)
        {
            tmp = -float(M_PI_2);
        } else {
            tmp = 0;
        }
    }
    if (tmp < 0)
    {
        tmp += float(M_PI) * 2;
    }

    if (!descending) {
        return tmp;
    } else {
        if (tmp >= float(M_PI))
        {
            tmp -= float(M_PI);
            return tmp;
        } else
        {
            tmp += float(M_PI);
            return tmp;
        }
    }

}

float GLWorker::distance_pow2(float x1, float y1, float x2, float y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
