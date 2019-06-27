#include "glworker.h"
#include <limits>
#include <valarray>
#include <cmath>
#include <QDebug>

GLWorker::GLWorker()
{
}

void GLWorker::graham(vector_of_points const &points)
{
    emit send_vector_lines(_lib.graham(points));
}

void GLWorker::gift_wrapping(vector_of_points const& points)
{
    emit send_vector_lines(_lib.gift_wrapping(points));
}


