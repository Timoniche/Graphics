#include "glworker.h"
#include <limits>
#include <valarray>
#include <cmath>
#include <QDebug>


//________________________________________
GLWorker::GLWorker()
{

}

/**
 * O(n*logn)
 */
void GLWorker::graham(vector_of_points const &points)
{
    emit send_vector_lines(_lib.graham(points));
}

/**
 * Pre: points sorted ascending by y
 * O(n^2)
 */
//void GLWorker::gift_wrapping(vector_of_points const& points)
//{
//    if (points.empty())
//    {
//        return;
//    }
//    used_points.assign(points.size(), false);
//    used_points[0] = true;
//    /*
//     * ': i |- i+1
//     * 2S = sum(0, n - 1): (xi + xi') * (yi' - yi)
//     */
//    float square = 0;
//    float x0 = points[0].x;
//    float y0 = points[0].y;
//    float x_next = x0;
//    float y_next = y0;
//    do
//    {
//        float x = x_next;
//        float y = y_next;
//        x_next = x0;
//        y_next = y0;
//        float min_angle = std::numeric_limits<float>::max();
//        size_t index_used = 0;

//        for (size_t j = 0; j < points.size(); j++)
//        {
//            if (!used_points[j] || j == 0) {
//                float x1 = points[j].x;
//                float y1 = points[j].y;
//                if (!equal(x1, y1, x, y)) {
//                    float angle = a_tan2(y1 - y, x1 - x, used_points[points.size() - 1]);
//                    if (angle < min_angle)
//                    {
//                        min_angle = angle;
//                        x_next = x1;
//                        y_next = y1;
//                        index_used = j;
//                    } else if (std::abs(angle - min_angle) < eps)
//                    {
//                        if (distance_pow2(x, y, x1, y1) <
//                                distance_pow2(x, y, x_next, y_next))
//                        {
//                            x_next = x1;
//                            y_next = y1;
//                            index_used = j;
//                        }
//                    }
//                }
//            }
//        }
//        used_points[index_used] = true;
//        emit send_line(x, y, x_next, y_next);
//        square += (x + x_next) * (y_next - y);
//    }
//    while (!equal(x0, y0, x_next, y_next));
//    emit send_square(square / 2);
//}


