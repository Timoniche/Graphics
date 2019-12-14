#include "triangulation.h"

bool TRIANGULATION::circumCircle(double xp, double yp, double x1, double y1, double x2,
                  double y2, double x3, double y3, double &xc, double &yc, double &r)
{
    double m1, m2, mx1, mx2, my1, my2;
    double dx, dy, rsqr, drsqr;

    if (abs(y1 - y2) < EPSILON && abs(y2 - y3) < EPSILON) // Check for coincident points
        return (false);
    if (abs(y2 - y1) < EPSILON)
    {
        m2 = -(x3 - x2) / (y3 - y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (x2 + x1) / 2.0;
        yc = m2 * (xc - mx2) + my2;
    } else if (abs(y3 - y2) < EPSILON)
    {
        m1 = -(x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        xc = (x3 + x2) / 2.0;
        yc = m1 * (xc - mx1) + my1;
    } else
    {
        m1 = -(x2 - x1) / (y2 - y1);
        m2 = -(x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        yc = m1 * (xc - mx1) + my1;
    }
    dx = x2 - xc;
    dy = y2 - yc;
    rsqr = dx * dx + dy * dy;
    r = sqrt(rsqr);
    dx = xp - xc;
    dy = yp - yc;
    drsqr = dx * dx + dy * dy;
    return drsqr <= rsqr;
}

double TRIANGULATION::is_left(const Point &p1, const Point &p2, const Point &q)
{
    Point u = {p2.x - p1.x,
                p2.y - p1.y};
    Point v = {q.x - p2.x,
                q.y - p2.y};
    return u.x * v.y - u.y * v.x;
}

bool TRIANGULATION::dots_inside_triangle(vector_of_points const &hull,
                          Point const &p0, Point const &p1, Point const &p2)
{
    bool ans = false;
    for_each(hull.begin(), hull.end(), [=, &ans](Point const &e)
    {
        if (e != p0 && e != p1 && e != p2)
        {
            double rot1 = is_left(p0, p1, e);
            double rot2 = is_left(p1, p2, e);
            double rot3 = is_left(p2, p0, e);
            if (rot1 >= 0 && rot2 >= 0 && rot3 >= 0) ans = true;
        }
    });
    return ans;
}

vector<Triangle> const& TRIANGULATION::triangulate(vector_of_points const& _hull)
{
    int n = _hull.size();
    PRIMITIVES::StackExtended _stack;


    vector<Triangle>* _triangles = new vector<Triangle>();
    if (n < 3) return *_triangles;
    FOR(i, n)
    {
        _stack.push(i);
        while (_stack.size() >= 3)
        {
            int i_top = _stack.top();
            Point fresh = _hull[i_top];
            int i_next_top = _stack.top_next();
            Point next_to_top = _hull[i_next_top];
            int i_next2_top = _stack.top_next_next();
            Point next2_to_top = _hull[i_next2_top];
            if (is_left(next2_to_top, next_to_top, fresh) <= 0) break;
            if (dots_inside_triangle(_hull, next2_to_top, next_to_top, fresh)) break;
            _stack.pop();
            _stack.pop();
            _stack.push(i);
            _triangles->emplace_back(_hull[i_next2_top], _hull[i_next_top], _hull[i_top]);
        }
    }
    return *_triangles;
}
