#include <algorithm>
#include <cmath>
#include <cstddef>
#include <random>
#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <cassert>
#include <cstdio>

#define FOR(i, n) for (int i = 0; i < n; i++)

/**
 * Smallest circle problem
 */

using namespace std;

static std::default_random_engine randGen((std::random_device()) ()); // NOLINT(cert-err58-cpp)

struct Point;
struct Circle;

typedef vector<Point> vector_of_points;
/*
 * Pre: size >= 1
 * randomized
 * O(n) time
 */
static Circle enclosingCircle(vector_of_points &points);
static Circle enclosingCircleFromPoint(vector_of_points &points, size_t end, Point &p);
static Circle enclosingCircleFromTwoPoints(vector_of_points &points, size_t end, Point &p,
                                           Point &q);

//______________________________________________________________________________________________________________________
struct Point final
{
public:
    double x;
    double y;
    inline Point operator-(Point &p) noexcept;
    inline double dist(Point &p) noexcept;
    inline double operator^(Point &p) noexcept;
    Point(double x, double y) noexcept;
};

struct Circle final
{
public:
    Point c;
    double r;
    Circle(const Point &c, double r) noexcept;
    inline bool contains(Point &p) noexcept;
    friend Circle circleOnDiameter(Point &a, Point &b) noexcept;
    friend Circle circleFromTriangle(Point &a, Point &b, Point &c) noexcept;
};

//______________________________________________________________________________________________________________________
Point Point::operator-(Point &p) noexcept
{
    return Point{x - p.x, y - p.y};
}


double Point::dist(Point &p) noexcept
{
    return std::hypot(x - p.x, y - p.y);
}


double Point::operator^(Point &p) noexcept
{
    return x * p.y - y * p.x;
}

Point::Point(double x, double y) noexcept : x(x), y(y)
{}

//______________________________________________________________________________________________________________________
bool Circle::contains(Point &p) noexcept
{
    return c.dist(p) <= r;
}

Circle::Circle(const Point &c, double r) noexcept : c(c), r(r)
{}

//______________________________________________________________________________________________________________________
Circle circleOnDiameter(Point &a, Point &b) noexcept
{
    Point c{(a.x + b.x) / 2, (a.y + b.y) / 2};
    return Circle{c, max(c.dist(a), c.dist(b))};
}


Circle circleFromTriangle(Point &a, Point &b, Point &c) noexcept
{
    double ox = (min(min(a.x, b.x), c.x) + max(min(a.x, b.x), c.x)) / 2;
    double oy = (min(min(a.y, b.y), c.y) + max(min(a.y, b.y), c.y)) / 2;
    double ax = a.x - ox, ay = a.y - oy;
    double bx = b.x - ox, by = b.y - oy;
    double cx = c.x - ox, cy = c.y - oy;
    double d = (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by)) * 2;
    if (d == 0)
    { return {{0, 0}, -1}; }
    double x = ((ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) +
                (cx * cx + cy * cy) * (ay - by)) / d;
    double y = ((ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) +
                (cx * cx + cy * cy) * (bx - ax)) / d;
    Point p{ox + x, oy + y};
    double r = max(max(p.dist(a), p.dist(b)), p.dist(c));
    return Circle{p, r};
}

Circle enclosingCircle(vector_of_points &points)
{
    shuffle(points.begin(), points.end(), randGen);
    Circle c{{0, 0}, -1};
    for (size_t i = 0; i < points.size(); i++)
    {
        Point &p = points[i];
        if (c.r < 0 || !c.contains(p))
            c = enclosingCircleFromPoint(points, i + 1, p);
    }
    return c;
}

Circle enclosingCircleFromPoint(vector_of_points &points, size_t end, Point &p)
{
    Circle c{p, 0};
    for (size_t i = 0; i < end; i++)
    {
        Point &q = points[i];
        if (!c.contains(q))
        {
            if (c.r == 0)
            {
                c = circleOnDiameter(p, q);
            } else
            {
                c = enclosingCircleFromTwoPoints(points, i + 1, p, q);
            }
        }
    }
    return c;
}

Circle enclosingCircleFromTwoPoints(vector_of_points &points, size_t end, Point &p, Point &q)
{
    Circle c = circleOnDiameter(p, q);
    for (size_t i = 0; i < end; i++)
    {
        if (!c.contains(points[i]))
        {
            c = circleFromTriangle(p, q, points[i]);
        }
    }
    return c;
}
//______________________________________________________________________________________________________________________

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector_of_points v;
    FOR(i, n)
    {
        double x, y;
        cin >> x >> y;
        v.emplace_back(x, y);
    }
    cout.precision(std::numeric_limits<double>::max_digits10);
    auto circle = enclosingCircle(v);
    assert(circle.r >= 0);
    cout << circle.r << "\n";
    cout << circle.c.x << " " << circle.c.y << "\n";
    return 0;
}