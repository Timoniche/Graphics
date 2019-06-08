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
void GLWorker::build_hull(data_t const& points)
{
    if (points.empty())
    {
        return;
    }
    used_points.assign(points.size(), false);
    used_points[0] = true;
    double x0 = points[0].first;
    double y0 = points[0].second;
    double x_next = x0;
    double y_next = y0;
    do
    {
        double x = x_next;
        double y = y_next;
        x_next = x0;
        y_next = y0;
        double min_angle = std::numeric_limits<double>::max();
        size_t index_used = 0;

        for (size_t j = 0; j < points.size(); j++)
        {
            if (!used_points[j] || j == 0) {
                double x1 = points[j].first;
                double y1 = points[j].second;
                if (!equal(x1, y1, x, y)) {
                    double angle = a_tan2(y1 - y, x1 - x, used_points[points.size() - 1]);
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

bool GLWorker::equal(double x1, double y1, double x2, double y2)
{
    return std::abs(x1 - x2) < eps && std::abs(y1 - y2) < eps;
}

double GLWorker::a_tan2(double y, double x, bool descending)
{

        double tmp = atan2(y, x);
        if (std::abs(x) < eps)
        {
            if (y > 0)
            {
                tmp = M_PI_2;
            } else if (y < 0)
            {
                tmp = -M_PI_2;
            } else {
                tmp = 0;
            }
        }
        if (tmp < 0)
        {
            tmp += M_PI * 2;
        }

        if (!descending) {
            return tmp;
        } else {
            if (tmp >= M_PI)
            {
                tmp -= M_PI;
                return tmp;
            } else
            {
                tmp += M_PI;
                return tmp;
            }
        }

}

double GLWorker::distance_pow2(double x1, double y1, double x2, double y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
