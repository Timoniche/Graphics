#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <functional>
#include <unordered_map>

#define FOR(i, n) for (int i = 0; i < n; i++)

struct Point;
typedef long long ll;
typedef std::vector<Point> vector_of_points;

using namespace std;

const int INF = numeric_limits<int>::max();

//______________________________________________________________________________________________________________________
struct StackExtended
{
public:
    void push(int x);
    int top();
    int top_next();
    int top_next_next();
    int pop();
    size_t size();
public:
    vector<int> _data;
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
    os << "(" << dt.x << ' ' << dt.y << ")";
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

void StackExtended::push(int x)
{
    _data.push_back(x);
}

int StackExtended::top()
{
    if (_data.empty())
    { return INF; }
    return _data[_data.size() - 1];
}

int StackExtended::top_next()
{
    if (_data.size() < 2)
    { return INF; }
    return _data[_data.size() - 2];
}

int StackExtended::top_next_next()
{
    if (_data.size() < 3)
    { return INF; }
    return _data[_data.size() - 3];
}

int StackExtended::pop()
{
    if (_data.empty())
    { return INF; }
    int tmp = top();
    _data.pop_back();
    return tmp;
}

size_t StackExtended::size()
{
    return _data.size();
}
//______________________________________________________________________________________________________________________

ll is_left(const Point &p1, const Point &p2, const Point &q)
{
    Point u = {p2.x - p1.x,
               p2.y - p1.y};
    Point v = {q.x - p2.x,
               q.y - p2.y};
    return u.x * v.y - u.y * v.x;
}

/**
 * Pre: p0, p1, p2 counter clockwise
 */
bool dots_inside_triangle(vector_of_points const &hull,
                          Point const &p0, Point const &p1, Point const &p2)
{
    bool ans = false;
    for_each(hull.begin(), hull.end(), [=, &ans](Point const &e)
    {
        if (e != p0 && e != p1 && e != p2)
        {
            ll rot1 = is_left(p0, p1, e);
            ll rot2 = is_left(p1, p2, e);
            ll rot3 = is_left(p2, p0, e);
            if (rot1 >= 0 && rot2 >= 0 && rot3 >= 0) ans = true;
        }
    });
    return ans;
}

int main()
{
    int n;
    cin >> n;
    vector_of_points _hull;
    StackExtended _stack;
    FOR(i, n)
    {
        ll x, y;
        cin >> x >> y;
        _hull.emplace_back(x, y);
    }

    FOR(i, n)
    {
        _stack.push(i);
        while (_stack.size() >= 3)
        {
            int i_top = _stack.top(); Point fresh = _hull[i_top];
            int i_next_top = _stack.top_next(); Point next_to_top = _hull[i_next_top];
            int i_next2_top = _stack.top_next_next(); Point next2_to_top = _hull[i_next2_top];
            if (is_left(next2_to_top, next_to_top, fresh) <= 0) break;
            if (dots_inside_triangle(_hull, next2_to_top, next_to_top, fresh)) break;
            _stack.pop();
            _stack.pop();
            _stack.push(i);
            cout << i_next2_top + 1 << " " << i_next_top + 1 << " " << i_top + 1 << endl;
        }
    }
    return 0;
}