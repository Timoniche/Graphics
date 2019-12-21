#include "primitives.h"

using namespace PRIMITIVES;

PRIMITIVES::Point::Point() = default;

PRIMITIVES::Point::Point(double x, double y, Half_edge *half_edge) : x(x), y(y), half_edge(half_edge)
{}

void PRIMITIVES::Point::swap(Point &p)
{
    Point tmp(std::move(p));
    p = std::move(*this);
    *this = std::move(tmp);
}

PRIMITIVES::Point &PRIMITIVES::Point::operator=(Point const &p)
{
    x = p.x;
    y = p.y;
    half_edge = p.half_edge;
    return *this;
}

PRIMITIVES::Point &PRIMITIVES::Point::operator=(Point &&p)
{
    x = p.x;
    y = p.y;
    half_edge = p.half_edge;
    return *this;
}

PRIMITIVES::Point::Point(Point const &p)
{
    x = p.x;
    y = p.y;
    half_edge = p.half_edge;
}

PRIMITIVES::Point::Point(Point &&p)
{
    x = p.x;
    y = p.y;
    half_edge = p.half_edge;
}

std::ostream &PRIMITIVES::operator<<(std::ostream &os, const PRIMITIVES::Point &dt)
{
    os << "(" << dt.x << ' ' << dt.y << ")";
    return os;
}

bool PRIMITIVES::operator==(PRIMITIVES::Point const &lhs, PRIMITIVES::Point const &rhs)
{
    return equal(lhs.x, rhs.x) && equal(lhs.y, rhs.y) && lhs.half_edge == rhs.half_edge;
}

bool PRIMITIVES::operator!=(PRIMITIVES::Point const &lhs, PRIMITIVES::Point const &rhs)
{
    return !(equal(lhs.x, rhs.x) && equal(lhs.y, rhs.y) && lhs.half_edge == rhs.half_edge);
}

void PRIMITIVES::StackExtended::push(int x)
{
    _data.push_back(x);
}

int PRIMITIVES::StackExtended::top()
{
    if (_data.empty())
    { return INF; }
    return _data[_data.size() - 1];
}

int PRIMITIVES::StackExtended::top_next()
{
    if (_data.size() < 2)
    { return INF; }
    return _data[_data.size() - 2];
}

int PRIMITIVES::StackExtended::top_next_next()
{
    if (_data.size() < 3)
    { return INF; }
    return _data[_data.size() - 3];
}

int PRIMITIVES::StackExtended::pop()
{
    if (_data.empty())
    { return INF; }
    int tmp = top();
    _data.pop_back();
    return tmp;
}

size_t PRIMITIVES::StackExtended::size()
{
    return _data.size();
}

template<typename T>
bool PRIMITIVES::equal(T a, T b)
{
    return std::abs(a - b) < EPSILON;
}

struct PRIMITIVES::Half_edge
{
    Half_edge *next, *twin, *prev;
    Point *origin;
    Face *face;

    Half_edge(Half_edge *next, Half_edge *twin, Half_edge *prev, Point *origin, Face *face) : next(next), twin(twin),
        prev(prev),
        origin(origin),
        face(face)
    {}

    Half_edge &operator=(Half_edge const &p)
    {
        next = p.next;
        twin = p.twin;
        prev = p.prev;
        origin = p.origin;
        face = p.face;
        return *this;
    }
};

struct PRIMITIVES::Face
{
    Half_edge *boundary;
    Half_edge **holes;
    unsigned long int count_of_holes;

    Face(Half_edge *boundary, Half_edge **holes, unsigned long count_of_holes) : boundary(boundary), holes(holes),
        count_of_holes(count_of_holes)
    {}

    Face &operator=(Face const &p)
    {
        boundary = p.boundary;
        holes = p.holes;
        count_of_holes = p.count_of_holes;
        return *this;
    }

};
PRIMITIVES::Triangle::Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {}
