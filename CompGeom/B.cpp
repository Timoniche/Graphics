#include <iostream>
#include <vector>
#include <cmath>
#include <cstddef>

#include <gmpxx.h>
#include "tests.h"

using namespace std;

struct pt
{
    double x, y;
};

inline int area(pt a, pt b, pt c)
{
    double cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    double eps = abs(4 * std::numeric_limits<double>::epsilon() *
                    ((b.x - a.x) * (c.y - a.y) + (c.x - a.x) * (b.y - a.y)));

    if (cross > eps)
    {
        return 1;
    }
    if (cross < -eps)
    {
        return -1;
    }
    mpq_class ax(a.x);
    mpq_class ay(a.y);
    mpq_class bx(b.x);
    mpq_class by(b.y);
    mpq_class cx(c.x);
    mpq_class cy(c.y);
    return sgn((bx - ax) * (cy - ay) - (cx - ax) * (by - ay));
}

inline bool intersect_1(double a, double b, double c, double d)
{
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}

bool intersect(pt a, pt b, pt c, pt d)
{
    return intersect_1(a.x, b.x, c.x, d.x)
           && intersect_1(a.y, b.y, c.y, d.y)
           && area(a, b, c) * area(a, b, d) <= 0
           && area(c, d, a) * area(c, d, b) <= 0;
}

int main()
{
    int test_id;
    std::cin >> test_id;
    std::vector<double> v = genTest(test_id);

    for (int i = 0; i < v.size();)
    {
        double ax = v[i++];
        double ay = v[i++];
        pt a = {ax, ay};
        double bx = v[i++];
        double by = v[i++];
        pt b = {bx, by};
        double cx = v[i++];
        double cy = v[i++];
        pt c = {cx, cy};
        double dx = v[i++];
        double dy = v[i++];
        pt d = {dx, dy};
        intersect(a, b, c, d) ? cout << "Y" : cout << "N";
    }


    return 0;
}
