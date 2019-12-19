#include <SFML/Graphics.hpp>

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <set>

#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"
#include "circumcenter.h"

#define FOR(i, n) for (int i = 0; i < n; i++)

typedef pair<double, double> pdd;
typedef pair<int, int> pii;

using namespace std;

struct Point;

typedef long long ll;
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
    ll x = -1;
    ll y = -1;

    Point();
    Point(ll x, ll y);
    Point(Point const &p);
    Point(Point &&p);
    Point &operator=(Point const &p);
    Point &operator=(Point &&p);
    void swap(Point &p);
    friend std::ostream &operator<<(std::ostream &os, const Point &dt);
    friend bool operator==(Point const &lhs, Point const &rhs);
    friend bool operator!=(Point const &lhs, Point const &rhs);
};

Point::Point() = default;

Point::Point(ll x, ll y) : x(x), y(y)
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

ll distance_pow2(ll x1, ll y1, ll x2, ll y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool left_directed(size_t next_to_top, size_t top, size_t pi, vector_of_points const &data)
{
    Point u = {data[top].x - data[next_to_top].x,
               data[top].y - data[next_to_top].y};
    Point v = {data[pi].x - data[top].x,
               data[pi].y - data[top].y};
    return u.x * v.y - u.y * v.x <= 0;
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
                                                    return a.y < b.y;
                                                });

    data_counter_clock_from_p0.begin()->swap(*it_to_min_y_element);
    ll x0 = data_counter_clock_from_p0[0].x;
    ll y0 = data_counter_clock_from_p0[0].y;
    std::sort(data_counter_clock_from_p0.begin() + 1, data_counter_clock_from_p0.end(), [x0, y0](const Point &a,
                                                                                                 const Point &b)
    {
        Point at{a.y - y0, a.x - x0};
        Point bt{b.y - y0, b.x - x0};
        ll cross = at.x * bt.y - at.y * bt.x;
        if (cross == 0)
        {
            ll dis_a_p0 = distance_pow2(a.x, a.y, x0, y0);
            ll dis_b_p0 = distance_pow2(b.x, b.y, x0, y0);
            return dis_a_p0 < dis_b_p0;
        } else
        {
            return cross < 0;
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

double distance_pow2(double x1, double y1, double x2, double y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool is_left(const pdd &p1, const pdd &p2, const pdd &q)
{
    pdd u = {p2.first - p1.first,
             p2.second - p1.second};
    pdd v = {q.first - p2.first,
             q.second - p2.second};
    return u.first * v.second - u.second * v.first > 0;
}

bool tryEmplace(pdd const &v, std::set<pdd> *const &hulls, int index, pdd before, pdd now)
{
    if (v.first != FLT_MAX && !is_left(before, now, v))
    {
        hulls[index].emplace(v);
        return true;
    }
    return false;
}


int main()
{
    int max_x, max_y, n;
    cin >> max_x >> max_y >> n;
    // points: {0, 0}, {0, max_y}, {max_x, 0}, {max_x, max_y}
    //borders:
    double alow, blow, clow;
    double ahigh, bhigh, chigh;
    double aleft, bleft, cleft;
    double aright, bright, cright;
    lineFromPoints({0, 0}, {max_x, 0}, alow, blow, clow);
    lineFromPoints({0, max_y}, {max_x, max_y}, ahigh, bhigh, chigh);
    lineFromPoints({0, 0}, {0, max_y}, aleft, bleft, cleft);
    lineFromPoints({max_x, 0}, {max_x, max_y}, aright, bright, cright);

    unordered_map<pii, int, hash_pair> point_index_map;
    std::set<pdd> hulls[n];
    std::vector<Vector2> points;
    vector_of_points pointsView;
    FOR(i, n)
    {
        int x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
        pointsView.emplace_back(x, y);
        point_index_map[{x, y}] = i;
    }

    Delaunay triangulation;
    const std::vector<Triangle> triangles = triangulation.triangulate(points);
    const std::vector<Edge> edges = triangulation.getEdges();

    vector_of_points hull = graham(pointsView);
    int sz = int(hull.size());
    for (int i = 1; i <= hull.size(); i++)
    {
        Point nowP = hull[i % sz];
        Point beforeP = hull[i - 1];
        Point afterP = hull[(i + 1) % sz];
        pdd now = {nowP.x, nowP.y};
        pdd before = {beforeP.x, beforeP.y};
        pdd after = {afterP.x, afterP.y};
        // c = -bx + ay
        double a1, b1, c1;
        double a2, b2, c2;
        lineFromPoints(before, now, a1, b1, c1);
        lineFromPoints(now, after, a2, b2, c2);
        perpendicularBisectorFromLine(before, now, a1, b1, c1);
        perpendicularBisectorFromLine(now, after, a2, b2, c2);

        int index = i % sz;

        pdd fromPred;
        pdd fromNext;

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

    std::vector<pdd> voronoi_vertices;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Delaunay triangulation");
    int scaleX = 800 / max_x;
    int scaleY = 600 / max_y;

    window.setFramerateLimit(1);

    for (auto &t : triangles)
    {
        pdd center = findCircumCenter({t.a->x, t.a->y},
                                      {t.b->x, t.b->y},
                                      {t.c->x, t.c->y});
        //cout << center.first << " " << center.second << endl;
        assert(center.first != FLT_MAX && center.second != FLT_MAX);
        voronoi_vertices.push_back(center);

        hulls[point_index_map[{int(t.a->x), int(t.a->y)}]].emplace(center);
        hulls[point_index_map[{int(t.b->x), int(t.b->y)}]].emplace(center);
        hulls[point_index_map[{int(t.c->x), int(t.c->y)}]].emplace(center);

        sf::RectangleShape s{sf::Vector2f(4, 4)};
        s.setPosition(static_cast<float>(center.first * scaleX),
                      static_cast<float>(center.second * scaleY));
        window.draw(s);
    }

//______________________________________________________________________________________________________________________

    std::vector<std::array<sf::Vertex, 2> > lines;
    for (const auto &e : edges)
    {
        const std::array<sf::Vertex, 2> line{{sf::Vertex(sf::Vector2f(
                static_cast<float>(scaleX * e.v->x + 2.),
                static_cast<float>(scaleY * e.v->y + 2.))), sf::Vertex(sf::Vector2f(
                static_cast<float>(scaleX * e.w->x + 2.),
                static_cast<float>(scaleY * e.w->y + 2.))),
                                             }};
        window.draw(std::data(line), 2, sf::Lines);
    }

    FOR(i, n)
    {
        vector<pdd> _hull(hulls[i].begin(), hulls[i].end());
        //______________________________________________________________________________________________________________
        auto it_to_min_y_element = std::min_element(_hull.begin(),
                                                    _hull.end(),
                                                    [](const pdd &a, const pdd &b)
                                                    {
                                                        if (a.second == b.second)
                                                            return a.first < b.first;
                                                        return a.second < b.second;
                                                    });
        _hull.begin()->swap(*it_to_min_y_element);
        double x0 = _hull[0].first;
        double y0 = _hull[0].second;
        std::sort(_hull.begin() + 1, _hull.end(), [x0, y0](const pdd &a, const pdd &b)
        {
            pdd at{a.second - y0, a.first - x0};
            pdd bt{b.second - y0, b.first - x0};
            double cross = at.first * bt.second - at.second * bt.first;
            if (cross == 0)
            {
                double dis_a_p0 = distance_pow2(a.first, a.second, x0, y0);
                double dis_b_p0 = distance_pow2(b.first, b.second, x0, y0);
                return dis_a_p0 < dis_b_p0;
            } else
            {
                return cross < 0;
            }
        });
        //______________________________________________________________________________________________________________
        int k = int(_hull.size());
        cout << k << endl;
        FOR(j, k)
        {
            cout << _hull[j].first << " " << _hull[j].second << endl;
        }
    }
//______________________________________________________________________________________________________________________
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