#include "glworker.h"

GLWorker::GLWorker()
{

}

void GLWorker::build_hull(data_t const& points)
{
    for (auto &p : points)
    {
        for (auto &p2 : points)
        {
            emit send_line(p.first, p.second,
                           p2.first, p2.second);
        }
    }
}
