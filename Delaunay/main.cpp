/**#include <SFML/Graphics.hpp>*/
#include <SFML/Graphics.hpp>

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <set>
#include <iomanip>
#include <vector>
#include <float.h>
#include <cmath>
#include <algorithm>
#include <functional>
#include <random>

using namespace std;

typedef pair<long double, long double> pdd;
typedef pair<int, int> pii;


struct Edge;
struct Vector2;

#define FOR(i, n) for (int i = 0; i < n; i++)

struct Vector2
{
    Vector2() = default;
    Vector2(const Vector2 &v) = default;
    Vector2(Vector2 &&) = default;
    Vector2(const long double vx, const long double vy);

    long double dist2(const Vector2 &v) const;
    long double dist(const Vector2 &v) const;
    long double norm2() const;

    Vector2 &operator=(const Vector2 &) = default;
    Vector2 &operator=(Vector2 &&) = default;
    bool operator==(const Vector2 &v) const;
    friend std::ostream &operator<<(std::ostream &str, const Vector2 &v);

    long double x;
    long double y;
};

Vector2::Vector2(const long double vx, const long double vy) :
        x(vx), y(vy)
{}

long double
Vector2::dist2(const Vector2 &v) const
{
    const long double dx = x - v.x;
    const long double dy = y - v.y;
    return dx * dx + dy * dy;
}

long double
Vector2::dist(const Vector2 &v) const
{
    return hypot(x - v.x, y - v.y);
}

long double
Vector2::norm2() const
{
    return x * x + y * y;
}

bool
Vector2::operator==(const Vector2 &v) const
{
    return (this->x == v.x) && (this->y == v.y);
}

std::ostream &
operator<<(std::ostream &str, const Vector2 &v)
{
    return str << "Point x: " << v.x << " y: " << v.y;
}

bool almost_equal(const long double x, const long double y, int ulp)
{
    return std::abs(x - y) <=
           std::numeric_limits<long double>::epsilon() * std::abs(x + y) * static_cast<long double>(ulp)
           || std::abs(x - y) < std::numeric_limits<long double>::min();
}

bool almost_equal(const Vector2 &v1, const Vector2 &v2, int ulp)
{
    return almost_equal(v1.x, v2.x, ulp) && almost_equal(v1.y, v2.y, ulp);
}

bool almost_equal(const Vector2 &v1, const Vector2 &v2, int ulp = 2);

long double half(const long double x)
{
    return 0.5 * x;
}


void lineFromPoints(pdd P, pdd Q, long double &a,
                    long double &b, long double &c)
{
    a = Q.second - P.second;
    b = P.first - Q.first;
    c = a * (P.first) + b * (P.second);
}

void perpendicularBisectorFromLine(pdd P, pdd Q,
                                   long double &a, long double &b, long double &c)
{
    pdd mid_point = make_pair((P.first + Q.first) / 2,
                              (P.second + Q.second) / 2);

    // c = -bx + ay
    c = -b * (mid_point.first) + a * (mid_point.second);

    long double temp = a;
    a = -b;
    b = temp;
}

pdd lineLineIntersection(long double a1, long double b1, long double c1,
                         long double a2, long double b2, long double c2)
{
    long double determinant = a1 * b2 - a2 * b1;
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return make_pair(FLT_MAX, FLT_MAX);
    } else
    {
        long double x = (b2 * c1 - b1 * c2) / determinant;
        long double y = (a1 * c2 - a2 * c1) / determinant;
        return make_pair(x, y);
    }
}

pdd findCircumCenter(pdd P, pdd Q, pdd R)
{
    // Line PQ is represented as ax + by = c
    long double a, b, c;
    lineFromPoints(P, Q, a, b, c);

    // Line QR is represented as ex + fy = g
    long double e, f, g;
    lineFromPoints(Q, R, e, f, g);

    // Converting lines PQ and QR to perpendicular
    // vbisectors. After this, L = ax + by = c
    // M = ex + fy = g
    perpendicularBisectorFromLine(P, Q, a, b, c);
    perpendicularBisectorFromLine(Q, R, e, f, g);

    // The point of intersection of L and M gives
    // the circumcenter
    pdd circumcenter =
            lineLineIntersection(a, b, c, e, f, g);

    return {circumcenter.first, circumcenter.second};
}

struct Triangle
{
    using EdgeType = Edge;
    using VertexType = Vector2;

    Triangle() = default;
    Triangle(const Triangle &) = default;
    Triangle(Triangle &&) = default;
    Triangle(const VertexType &v1, const VertexType &v2, const VertexType &v3);

    bool containsVertex(const VertexType &v) const;
    bool circumCircleContains(const VertexType &v) const;

    Triangle &operator=(const Triangle &) = default;
    Triangle &operator=(Triangle &&) = default;
    bool operator==(const Triangle &t) const;
    friend std::ostream &operator<<(std::ostream &str, const Triangle &t);

    const VertexType *a;
    const VertexType *b;
    const VertexType *c;
    bool isBad = false;
};

Triangle::Triangle(const VertexType &v1, const VertexType &v2, const VertexType &v3) :
        a(&v1), b(&v2), c(&v3), isBad(false)
{}

bool
Triangle::containsVertex(const VertexType &v) const
{
    // return p1 == v || p2 == v || p3 == v;
    return almost_equal(*a, v) || almost_equal(*b, v) || almost_equal(*c, v);
}

bool
Triangle::circumCircleContains(const VertexType &v) const
{
    const long double ab = a->norm2();
    const long double cd = b->norm2();
    const long double ef = c->norm2();

    const long double ax = a->x;
    const long double ay = a->y;
    const long double bx = b->x;
    const long double by = b->y;
    const long double cx = c->x;
    const long double cy = c->y;

    const long double circum_x =
            (ab * (cy - by) + cd * (ay - cy) + ef * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
    const long double circum_y =
            (ab * (cx - bx) + cd * (ax - cx) + ef * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

    const VertexType circum(half(circum_x), half(circum_y));
    const long double circum_radius = a->dist2(circum);
    const long double dist = v.dist2(circum);
    return dist <= circum_radius;
}

bool
Triangle::operator==(const Triangle &t) const
{
    return (*this->a == *t.a || *this->a == *t.b || *this->a == *t.c) &&
           (*this->b == *t.a || *this->b == *t.b || *this->b == *t.c) &&
           (*this->c == *t.a || *this->c == *t.b || *this->c == *t.c);
}

std::ostream &
operator<<(std::ostream &str, const Triangle &t)
{
    return str << "Triangle:" << "\n\t" <<
               *t.a << "\n\t" <<
               *t.b << "\n\t" <<
               *t.c << '\n';
}

bool almost_equal(const Triangle &t1, const Triangle &t2)
{
    return (almost_equal(*t1.a, *t2.a) || almost_equal(*t1.a, *t2.b) || almost_equal(*t1.a, *t2.c)) &&
           (almost_equal(*t1.b, *t2.a) || almost_equal(*t1.b, *t2.b) || almost_equal(*t1.b, *t2.c)) &&
           (almost_equal(*t1.c, *t2.a) || almost_equal(*t1.c, *t2.b) || almost_equal(*t1.c, *t2.c));
}


bool almost_equal(const Triangle &t1, const Triangle &t2);

struct Edge
{
    using VertexType = Vector2;

    Edge() = default;
    Edge(const Edge &) = default;
    Edge(Edge &&) = default;
    Edge(const VertexType &v1, const VertexType &v2);

    Edge &operator=(const Edge &) = default;
    Edge &operator=(Edge &&) = default;
    bool operator==(const Edge &e) const;
    friend std::ostream &operator<<(std::ostream &str, const Edge &e);

    const VertexType *v;
    const VertexType *w;
    bool isBad = false;
};

Edge::Edge(const VertexType &v1, const VertexType &v2) :
        v(&v1), w(&v2)
{}

bool
Edge::operator==(const Edge &e) const
{
    return (*(this->v) == *e.v && *(this->w) == *e.w) ||
           (*(this->v) == *e.w && *(this->w) == *e.v);
}

std::ostream &
operator<<(std::ostream &str, const Edge &e)
{
    return str << "Edge " << *e.v << ", " << *e.w;
}

bool almost_equal(const Edge &e1, const Edge &e2)
{
    return (almost_equal(*e1.v, *e2.v) && almost_equal(*e1.w, *e2.w)) ||
           (almost_equal(*e1.v, *e2.w) && almost_equal(*e1.w, *e2.v));
}

bool almost_equal(const Edge &e1, const Edge &e2);

class Delaunay
{
public:
    using TriangleType = Triangle;
    using EdgeType = Edge;
    using VertexType = Vector2;

    Delaunay() = default;
    Delaunay(const Delaunay &) = delete;
    Delaunay(Delaunay &&) = delete;

    const std::vector<TriangleType> &triangulate(std::vector<VertexType> &vertices);
    const std::vector<TriangleType> &getTriangles() const;
    const std::vector<EdgeType> &getEdges() const;
    const std::vector<VertexType> &getVertices() const;

    Delaunay &operator=(const Delaunay &) = delete;
    Delaunay &operator=(Delaunay &&) = delete;

private:
    std::vector<TriangleType> _triangles;
    std::vector<EdgeType> _edges;
    std::vector<VertexType> _vertices;
};

const std::vector<Delaunay::TriangleType> &
Delaunay::triangulate(std::vector<VertexType> &vertices)
{
    _vertices = vertices;
    long double minX = vertices[0].x;
    long double minY = vertices[0].y;
    long double maxX = minX;
    long double maxY = minY;

    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
    }

    const long double dx = maxX - minX;
    const long double dy = maxY - minY;
    const long double deltaMax = std::max(dx, dy);
    const long double midx = half(minX + maxX);
    const long double midy = half(minY + maxY);

    const VertexType p1(midx - 20 * deltaMax, midy - deltaMax);
    const VertexType p2(midx, midy + 20 * deltaMax);
    const VertexType p3(midx + 20 * deltaMax, midy - deltaMax);

    // Create a list of triangles, and add the supertriangle in it
    _triangles.push_back(TriangleType(p1, p2, p3));

    for (auto p = begin(vertices); p != end(vertices); p++)
    {
        std::vector<EdgeType> polygon;

        for (auto &t : _triangles)
        {
            if (t.circumCircleContains(*p))
            {
                t.isBad = true;
                polygon.push_back(Edge{*t.a, *t.b});
                polygon.push_back(Edge{*t.b, *t.c});
                polygon.push_back(Edge{*t.c, *t.a});
            }
        }

        _triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [](TriangleType &t)
        {
            return t.isBad;
        }), end(_triangles));

        for (auto e1 = begin(polygon); e1 != end(polygon); ++e1)
        {
            for (auto e2 = e1 + 1; e2 != end(polygon); ++e2)
            {
                if (almost_equal(*e1, *e2))
                {
                    e1->isBad = true;
                    e2->isBad = true;
                }
            }
        }

        polygon.erase(std::remove_if(begin(polygon), end(polygon), [](EdgeType &e)
        {
            return e.isBad;
        }), end(polygon));

        for (const auto e : polygon)
            _triangles.push_back(TriangleType(*e.v, *e.w, *p));

    }

    _triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [p1, p2, p3](TriangleType &t)
    {
        return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
    }), end(_triangles));

    for (const auto t : _triangles)
    {
        _edges.push_back(Edge{*t.a, *t.b});
        _edges.push_back(Edge{*t.b, *t.c});
        _edges.push_back(Edge{*t.c, *t.a});
    }

    return _triangles;
}

const std::vector<Delaunay::TriangleType> &
Delaunay::getTriangles() const
{
    return _triangles;
}

const std::vector<Delaunay::EdgeType> &
Delaunay::getEdges() const
{
    return _edges;
}

const std::vector<Delaunay::VertexType> &
Delaunay::getVertices() const
{
    return _vertices;
}


struct Point;

typedef std::vector<Point> vector_of_points;

struct StackExtended
{
public:
    void push(size_t x);
    int top();
    int top_next();
    int pop();
public:
    std::vector<size_t> _data;
};

struct Point
{
    long double x = -1;
    long double y = -1;

    Point();
    Point(long double x, long double y);
    Point(Point const &p);
    Point(Point &&p);
    Point &operator=(Point const &p);
    Point &operator=(Point &&p);
    void swap(Point &p);
    friend std::ostream &operator<<(std::ostream &os, const Point &dt);
    friend bool operator==(Point const &lhs, Point const &rhs);
    friend bool operator!=(Point const &lhs, Point const &rhs);
    friend bool operator<(Point const &lhs, Point const &rhs);
};

Point::Point() = default;

Point::Point(long double x, long double y) : x(x), y(y)
{}

void Point::swap(Point &p)
{
    Point tmp(std::move(p));
    p = std::move(*this);
    *this = std::move(tmp);
}

Point &Point::operator=(Point const &p)
{
    x = p.x;
    y = p.y;
    return *this;
}

Point &Point::operator=(Point &&p)
{
    x = p.x;
    y = p.y;
    return *this;
}

Point::Point(Point const &p)
{
    x = p.x;
    y = p.y;
}

Point::Point(Point &&p)
{
    x = p.x;
    y = p.y;
}

std::ostream &operator<<(std::ostream &os, const Point &dt)
{
    os << dt.x << ' ' << dt.y;
    return os;
}

bool operator==(Point const &lhs, Point const &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(Point const &lhs, Point const &rhs)
{
    return !(lhs == rhs);
}

bool operator<(Point const &lhs, Point const &rhs)
{
    if (lhs.x == rhs.x) return lhs.y < rhs.y;
    return lhs.x < rhs.x;
}


void StackExtended::push(size_t x)
{
    _data.push_back(x);
}

int StackExtended::top()
{
    if (_data.empty())
    { return -1; }
    return int(_data[_data.size() - 1]);
}

int StackExtended::top_next()
{
    if (_data.size() < 2)
    { return -1; }
    return int(_data[_data.size() - 2]);
}

int StackExtended::pop()
{
    if (_data.empty())
    { return -1; }
    int tmp = top();
    _data.pop_back();
    return tmp;
}

long double distance_pow2(long double x1, long double y1, long double x2, long double y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool left_directed(size_t next_to_top, size_t top, size_t pi, vector_of_points const &data)
{
    Point u = {data[top].x - data[next_to_top].x,
               data[top].y - data[next_to_top].y};
    Point v = {data[pi].x - data[top].x,
               data[pi].y - data[top].y};
    long double cross = u.x * v.y - u.y * v.x;
    return cross < 0;
}

StackExtended _stack;

vector_of_points const &graham(vector_of_points const &points)
{
    auto *ret = new vector_of_points();
    switch (points.size())
    {
        case 0:
        case 1:
            return *ret;
        case 2:
            ret->emplace_back(points[0].x, points[0].y);
            ret->emplace_back(points[1].x, points[1].y);
            return *ret;
        default:
            break;
    }
    vector_of_points data_counter_clock_from_p0 = points;
    auto it_to_min_y_element = std::min_element(data_counter_clock_from_p0.begin(),
                                                data_counter_clock_from_p0.end(),
                                                [](const Point &a, const Point &b)
                                                {
                                                    if (a.y == b.y) return a.x < b.x;
                                                    return a.y < b.y;
                                                });
    auto it_to_max_y_element = std::min_element(data_counter_clock_from_p0.begin(),
                                                data_counter_clock_from_p0.end(),
                                                [](const Point &a, const Point &b)
                                                {
                                                    return a.y > b.y;
                                                });
    data_counter_clock_from_p0.begin()->swap(*it_to_min_y_element);
    long double x0 = data_counter_clock_from_p0[0].x;
    long double y0 = data_counter_clock_from_p0[0].y;
    long double maxx = (*it_to_max_y_element).x;
    long double maxy = (*it_to_max_y_element).y;
    Point tomax{maxx - x0, maxy - y0};
    std::sort(data_counter_clock_from_p0.begin() + 1, data_counter_clock_from_p0.end(), [x0, y0, tomax](const Point &a,
                                                                                                        const Point &b)
    {
        Point at{a.x - x0, a.y - y0};
        Point bt{b.x - x0, b.y - y0};
        long double cross = at.x * bt.y - at.y * bt.x;

        if (cross == 0)
        {
            long double cross_with_max = at.x * tomax.y - at.y * tomax.x;
            long double dis_a_p0 = distance_pow2(a.x, a.y, x0, y0);
            long double dis_b_p0 = distance_pow2(b.x, b.y, x0, y0);
            return cross_with_max >= 0 ? dis_a_p0 < dis_b_p0 : dis_a_p0 > dis_b_p0;
        } else
        {
            return cross > 0;
        }
    });
    _stack.push(0);
    _stack.push(1);
    for (size_t pi = 2; pi < data_counter_clock_from_p0.size(); pi++)
    {
        int _next_to_top = _stack.top_next();
        assert(_next_to_top != -1);
        int _top = _stack.top();
        assert(_top != -1);
        while (left_directed(size_t(_next_to_top), size_t(_top), pi, data_counter_clock_from_p0))
        {
            _stack.pop();
            if (_stack._data.size() == 1) break;
            _next_to_top = _stack.top_next();
            assert(_next_to_top != -1);
            _top = _stack.top();
            assert(_top != -1);
        }
        _stack.push(pi);
    }
    for (size_t i = 0; i < _stack._data.size(); i++)
    {
        ret->push_back(data_counter_clock_from_p0[_stack._data[i]]);
    }
    _stack._data.clear();
    return *ret;
}

struct hash_pair
{
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

long double cross(const pdd &p1, const pdd &p2, const pdd &q)
{
    pdd u = {p2.first - p1.first,
             p2.second - p1.second};
    pdd v = {q.first - p2.first,
             q.second - p2.second};
    return u.first * v.second - u.second * v.first;
}

bool is_left(const pdd &p1, const pdd &p2, const pdd &q)
{
    pdd u = {p2.first - p1.first,
             p2.second - p1.second};
    pdd v = {q.first - p2.first,
             q.second - p2.second};
    return cross(p1, p2, q) > 0;
}

bool belongs(const pdd &ebeg, const pdd &eend, const pdd &q)
{
    long double rot = cross(ebeg, eend, q);
    long double eps = 1e-4;
    //long double eps = numeric_limits<long double>::epsilon();
    //if (rot <= eps && rot >= -eps)
    {
        long double low = min(ebeg.second, eend.second);
        long double high = max(ebeg.second, eend.second);
        long double left = min(ebeg.first, eend.first);
        long double right = max(ebeg.first, eend.first);
        return q.first >= left - eps && q.first <= right + eps &&
               q.second >= low - eps && q.second <= high + eps;
    }
    return false;
}

int max_x, max_y;


set<Point> clip_hull(vector_of_points _hull, int MAXX, int MAXY, int deltaX, int deltaY)
{
    set<Point> ret;

    long double alow, blow, clow;
    long double ahigh, bhigh, chigh;
    long double aleft, bleft, cleft;
    long double aright, bright, cright;
    lineFromPoints({0, 0}, {MAXX, 0}, alow, blow, clow);
    lineFromPoints({0, MAXY}, {MAXX, MAXY}, ahigh, bhigh, chigh);
    lineFromPoints({0, 0}, {0, MAXY}, aleft, bleft, cleft);
    lineFromPoints({MAXX, 0}, {MAXX, MAXY}, aright, bright, cright);

    int k = int(_hull.size());

    for (int i = 0; i < k; ++i)
    {
        _hull[i].x -= deltaX;
        _hull[i].y -= deltaY;
    }

    for (int i = 0; i < k; ++i)
    {
        pdd now = {_hull[i].x, _hull[i].y};
        if (now.first >= 0 && now.first <= MAXX && now.second >= 0 && now.second <= MAXY)
        {
            ret.emplace(now.first, now.second);
        }
        pdd after = {_hull[(i + 1) % k].x, _hull[(i + 1) % k].y};

        long double a, b, c;
        lineFromPoints(now, after, a, b, c);
        pdd withLow = lineLineIntersection(a, b, c, alow, blow, clow);
        if (belongs({0, 0}, {MAXX, 0}, withLow) &&
            belongs(now, after, withLow))
            ret.emplace(withLow.first, withLow.second);
        pdd withHigh = lineLineIntersection(a, b, c, ahigh, bhigh, chigh);
        if (belongs({0, MAXY}, {MAXX, MAXY}, withHigh) &&
            belongs(now, after, withHigh))
            ret.emplace(withHigh.first, withHigh.second);
        pdd withLeft = lineLineIntersection(a, b, c, aleft, bleft, cleft);
        if (belongs({0, 0}, {0, MAXY}, withLeft) &&
            belongs(now, after, withLeft))
            ret.emplace(withLeft.first, withLeft.second);
        pdd withRight = lineLineIntersection(a, b, c, aright, bright, cright);
        if (belongs({MAXX, 0}, {MAXX, MAXY}, withRight) &&
            belongs(now, after, withRight))
            ret.emplace(withRight.first, withRight.second);
    }
    return ret;
}

bool tryEmplace(pdd const &v, std::set<pdd> *const &hulls, int index, pdd before, pdd now)
{
    if (v.first < 0 || v.first > max_x || v.second < 0 || v.second > max_y) return false;
    if (v.first != FLT_MAX && !is_left(before, now, v))
    {
        hulls[index].emplace(v);
        return true;
    }
    return false;
}

int main()
{
    int n;
    int MAXX, MAXY, deltaX, deltaY;
    //cin >> MAXX >> MAXY >> n;
    n = 10000;
    MAXX = 10000;
    MAXY = 10000;
    vector_of_points input;
    std::default_random_engine eng(std::random_device{}());
    std::uniform_real_distribution<long double> dist_w(0, MAXX);
    std::uniform_real_distribution<long double> dist_h(0, MAXY);

    max_x = 1000 * MAXX;
    max_y = 1000 * MAXY;
//    max_x = 100000 * 1000;
//    max_y = 100000 * 1000;
    deltaX = max_x / 2;
    deltaY = max_y / 2;

    /**
    sf::RenderWindow window(sf::VideoMode(800, 600), "Voronoi");
    window.setFramerateLimit(1);
     */
    sf::RenderWindow window(sf::VideoMode(800, 600), "Voronoi");
    window.setFramerateLimit(1);

    //long double scaleX = 800 / max_x;
    //long double scaleY = 600 / max_y;
    long double scaleX = 800.0 / MAXX;
    long double scaleY = 600.0 / MAXY;

    // points: {0, 0}, {0, max_y}, {max_x, 0}, {max_x, max_y}
    //borders:
    long double alow, blow, clow;
    long double ahigh, bhigh, chigh;
    long double aleft, bleft, cleft;
    long double aright, bright, cright;
    lineFromPoints({0, 0}, {max_x, 0}, alow, blow, clow);
    lineFromPoints({0, max_y}, {max_x, max_y}, ahigh, bhigh, chigh);
    lineFromPoints({0, 0}, {0, max_y}, aleft, bleft, cleft);
    lineFromPoints({max_x, 0}, {max_x, max_y}, aright, bright, cright);

    unordered_map<pii, int, hash_pair> point_index_map;
    std::set<pdd> hulls[n];
    std::vector<Vector2> points;
    std::vector<pdd> pointsPddView;
    vector_of_points pointsView;

    set<Point> allpoints;
    FOR(i, n)
    {
        int x, y;
        //cin >> x >> y;
        x = int(dist_w(eng));
        y = int(dist_h(eng));
        while (allpoints.find(Point{double(x), double(y)}) != allpoints.end())
        {
            x = int(dist_w(eng));
            y = int(dist_h(eng));
        }
        allpoints.emplace(x, y);

        cout << "xy: " << x << " " << y << endl;

        /**
        sf::RectangleShape s{sf::Vector2f(4, 4)};
        s.setPosition(static_cast<float>(x * scaleX),
                      static_cast<float>((MAXY - y) * scaleY));
        window.draw(s);
        */
        sf::RectangleShape s{sf::Vector2f(2, 2)};
        s.setPosition(static_cast<float>(x * scaleX),
                      static_cast<float>((MAXY - y) * scaleY));
        window.draw(s);

        //
        x += deltaX;
        y += deltaY;
        //
        points.emplace_back(x, y);
        pointsView.emplace_back(x, y);
        pointsPddView.emplace_back(x, y);
        point_index_map[{x, y}] = i;

//        sf::RectangleShape s{sf::Vector2f(4, 4)};
//        s.setPosition(static_cast<float>(x * scaleX),
//                      static_cast<float>((max_y - y) * scaleY));
//        window.draw(s);
    }

    Delaunay triangulation;
    const std::vector<Triangle> triangles = triangulation.triangulate(points);
    const std::vector<Edge> edges = triangulation.getEdges();

    vector_of_points hull = graham(pointsView);
    int sz = int(hull.size());
    //hull is a line
    if (triangles.empty())
    {
        for (int i = sz - 1 - 1; i >= 0 + 1; i--)
        {
            hull.push_back(hull[i]);
        }
    }
    sz = int(hull.size());
    for (int i = 1; i <= hull.size(); i++)
    {
        Point nowP = hull[i % sz];
        Point beforeP;
        beforeP = hull[i - 1];
        Point afterP;
        afterP = hull[(i + 1) % sz];
        pdd now = {nowP.x, nowP.y};
        pdd before = {beforeP.x, beforeP.y};
        pdd after = {afterP.x, afterP.y};

        long double a1, b1, c1;
        long double a2, b2, c2;
        lineFromPoints(before, now, a1, b1, c1);
        lineFromPoints(now, after, a2, b2, c2);
        // c = -bx + ay
        perpendicularBisectorFromLine(before, now, a1, b1, c1);
        perpendicularBisectorFromLine(now, after, a2, b2, c2);

        int index = i % sz;
        index = point_index_map[{hull[index].x, hull[index].y}];

        pdd predWithLow = lineLineIntersection(a1, b1, c1, alow, blow, clow);
        pdd predWithHigh = lineLineIntersection(a1, b1, c1, ahigh, bhigh, chigh);
        pdd predWithLeft = lineLineIntersection(a1, b1, c1, aleft, bleft, cleft);
        pdd predWithRight = lineLineIntersection(a1, b1, c1, aright, bright, cright);
        if (tryEmplace(predWithLow, hulls, index, before, now)) /**fromPred = LOW*/;
        if (tryEmplace(predWithHigh, hulls, index, before, now)) /**fromPred = HIGH*/;
        if (tryEmplace(predWithLeft, hulls, index, before, now)) /**fromPred = LEFT*/;
        if (tryEmplace(predWithRight, hulls, index, before, now)) /**fromPred = RIGHT*/;


        pdd nextWithLow = lineLineIntersection(a2, b2, c2, alow, blow, clow);
        pdd nextWithHigh = lineLineIntersection(a2, b2, c2, ahigh, bhigh, chigh);
        pdd nextWithLeft = lineLineIntersection(a2, b2, c2, aleft, bleft, cleft);
        pdd nextWithRight = lineLineIntersection(a2, b2, c2, aright, bright, cright);
        if (tryEmplace(nextWithLow, hulls, index, now, after)) /**fromNext = LOW*/;
        if (tryEmplace(nextWithHigh, hulls, index, now, after)) /**fromNext = HIGH*/;
        if (tryEmplace(nextWithLeft, hulls, index, now, after)) /**fromNext = LEFT*/;
        if (tryEmplace(nextWithRight, hulls, index, now, after)) /**fromNext = RIGHT*/;

    }
    auto it_leftlow = std::min_element(pointsPddView.begin(),
                                       pointsPddView.end(),
                                       [](const pdd &a, const pdd &b)
                                       {
                                           return distance_pow2(a.first, a.second, 0, 0) <
                                                  distance_pow2(b.first, b.second, 0, 0);
                                       });
    auto it_lefthigh = std::min_element(pointsPddView.begin(),
                                        pointsPddView.end(),
                                        [](const pdd &a, const pdd &b)
                                        {
                                            return distance_pow2(a.first, a.second, 0, max_y) <
                                                   distance_pow2(b.first, b.second, 0, max_y);
                                        });
    auto it_rightlow = std::min_element(pointsPddView.begin(),
                                        pointsPddView.end(),
                                        [](const pdd &a, const pdd &b)
                                        {
                                            return distance_pow2(a.first, a.second, max_x, 0) <
                                                   distance_pow2(b.first, b.second, max_x, 0);
                                        });
    auto it_righthigh = std::min_element(pointsPddView.begin(),
                                         pointsPddView.end(),
                                         [](const pdd &a, const pdd &b)
                                         {
                                             return distance_pow2(a.first, a.second, max_x, max_y) <
                                                    distance_pow2(b.first, b.second, max_x, max_y);
                                         });


    std::vector<pdd> voronoi_vertices;

    for (auto &t : triangles)
    {
        pdd center = findCircumCenter({t.a->x, t.a->y},
                                      {t.b->x, t.b->y},
                                      {t.c->x, t.c->y});
        if (center.first == FLT_MAX || center.second == FLT_MAX) continue;
        voronoi_vertices.push_back(center);
        hulls[point_index_map[{int(t.a->x), int(t.a->y)}]].emplace(center);
        hulls[point_index_map[{int(t.b->x), int(t.b->y)}]].emplace(center);
        hulls[point_index_map[{int(t.c->x), int(t.c->y)}]].emplace(center);
    }

//______________________________________________________________________________________________________________________
    /**std::vector<std::array<sf::Vertex, 2> > lines;*/
    std::vector<std::array<sf::Vertex, 2> > lines;

    auto leftlow = std::min_element(pointsPddView.begin(),
                                    pointsPddView.end(),
                                    [MAXX, MAXY, deltaX, deltaY](const pdd &a, const pdd &b)
                                    {
                                        return distance_pow2(a.first - deltaX, a.second - deltaY, 0, 0) <
                                               distance_pow2(b.first - deltaX, b.second - deltaY, 0, 0);
                                    });
    auto lefthigh = std::min_element(pointsPddView.begin(),
                                     pointsPddView.end(),
                                     [MAXX, MAXY, deltaX, deltaY](const pdd &a, const pdd &b)
                                     {
                                         return distance_pow2(a.first - deltaX, a.second - deltaY, 0, MAXY) <
                                                distance_pow2(b.first - deltaX, b.second - deltaY, 0, MAXY);
                                     });
    auto rightlow = std::min_element(pointsPddView.begin(),
                                     pointsPddView.end(),
                                     [MAXX, MAXY, deltaX, deltaY](const pdd &a, const pdd &b)
                                     {
                                         return distance_pow2(a.first - deltaX, a.second - deltaY, MAXX, 0) <
                                                distance_pow2(b.first - deltaX, b.second - deltaY, MAXX, 0);
                                     });
    auto righthigh = std::min_element(pointsPddView.begin(),
                                      pointsPddView.end(),
                                      [MAXX, MAXY, deltaX, deltaY](const pdd &a, const pdd &b)
                                      {
                                          return distance_pow2(a.first - deltaX, a.second - deltaY, MAXX, MAXY) <
                                                 distance_pow2(b.first - deltaX, b.second - deltaY, MAXX, MAXY);
                                      });
    FOR(i, n)
    {
        if (pointsPddView[i] == *it_leftlow) hulls[i].emplace(0, 0);
        if (pointsPddView[i] == *it_lefthigh) hulls[i].emplace(0, max_y);
        if (pointsPddView[i] == *it_rightlow) hulls[i].emplace(max_x, 0);
        if (pointsPddView[i] == *it_righthigh) hulls[i].emplace(max_x, max_y);

        vector<pdd> hullV(hulls[i].begin(), hulls[i].end());
        /**assert(!hullV.empty());*/
        assert(!hullV.empty());
        if (hullV.empty())
        {
            cout << "xyempty: " << pointsPddView[i].first - deltaX << " " <<
                 pointsPddView[i].second - deltaY << endl;
            continue;
        }

        vector_of_points hulltmp;
        for (auto &e : hullV) hulltmp.push_back({e.first, e.second});
        vector_of_points _hull = graham(hulltmp);
        //______________________________________________________________________________________________________________
        set<Point> _hullSet = clip_hull(_hull, MAXX, MAXY, deltaX, deltaY);

        if (pointsPddView[i] == *leftlow) _hullSet.emplace(0, 0);
        if (pointsPddView[i] == *lefthigh) _hullSet.emplace(0, MAXY);
        if (pointsPddView[i] == *rightlow) _hullSet.emplace(MAXX, 0);
        if (pointsPddView[i] == *righthigh) _hullSet.emplace(MAXX, MAXY);

        _hull = vector_of_points(_hullSet.begin(), _hullSet.end());

        _hull = graham(_hull);
        //______________________________________________________________________________________________________________
        int k = int(_hull.size());
        cout.precision(std::numeric_limits<long double>::max_digits10);
        cout << k << endl;
        FOR(j, k)
        {
            auto v = _hull[j];
            auto w = _hull[(j + 1) % k];
            cout << _hull[j].x << " " << _hull[j].y << endl;


//            const std::array<sf::Vertex, 2> line{{sf::Vertex(sf::Vector2f(
//                    static_cast<float>(scaleX * v.x + 2.),
//                    static_cast<float>(scaleY * (max_y - v.y) + 2.))), sf::Vertex(sf::Vector2f(
//                    static_cast<float>(scaleX * w.x + 2.),
//                    static_cast<float>(scaleY * (max_y - w.y) + 2.))),
//                                                 }};
//            window.draw(std::data(line), 2, sf::Lines);
            scaleX = 800.0 / MAXX;
            scaleY = 600.0 / MAXY;
            /**
            const std::array<sf::Vertex, 2> line{{sf::Vertex(sf::Vector2f(
                    static_cast<float>(scaleX * v.x + 2.),
                    static_cast<float>(scaleY * (MAXY - v.y) + 2.))), sf::Vertex(sf::Vector2f(
                    static_cast<float>(scaleX * w.x + 2.),
                    static_cast<float>(scaleY * (MAXY - w.y) + 2.))),
                                                 }};
            window.draw(std::data(line), 2, sf::Lines);
             */
            const std::array<sf::Vertex, 2> line{{sf::Vertex(sf::Vector2f(
                    static_cast<float>(scaleX * v.x + 2.),
                    static_cast<float>(scaleY * (MAXY - v.y) + 2.))), sf::Vertex(sf::Vector2f(
                    static_cast<float>(scaleX * w.x + 2.),
                    static_cast<float>(scaleY * (MAXY - w.y) + 2.))),
                                                 }};
            window.draw(std::data(line), 2, sf::Lines);

        }
    }

//______________________________________________________________________________________________________________________
    /**
    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
     */
    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    return 0;
}

//tests:
//6 5 5 2 1 2 2 2 3 1 4 3 4
//6 5 5 3 1 3 2 3 3 2 4 4 4
//10 10 5 4 1 4 2 4 3 3 4 5 4
//6 5 4 2 1 2 2 2 3 3 4
//no triangles tests:
//6 5 5 1 1 1 2 1 3 1 4 1 5
//10 10 5 1 1 2 2 3 3 4 4 5 5