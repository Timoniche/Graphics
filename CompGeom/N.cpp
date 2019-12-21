#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <iomanip>

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
    double x = -1;
    double y = -1;

    Point();
    Point(double x, double y);
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

Point::Point(double x, double y) : x(x), y(y)
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

double distance_pow2(double x1, double y1, double x2, double y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

double cross(size_t next_to_top, size_t top, size_t pi, vector_of_points const &data)
{
    Point u = {data[top].x - data[next_to_top].x,
               data[top].y - data[next_to_top].y};
    Point v = {data[pi].x - data[top].x,
               data[pi].y - data[top].y};
    return u.x * v.y - u.y * v.x;
}


double cross(Point next_to_top, Point top, Point pi)
{
    Point u = {top.x - next_to_top.x,
               top.y - next_to_top.y};
    Point v = {pi.x - top.x,
               pi.y - top.y};
    return u.x * v.y - u.y * v.x;
}

bool left_directed(size_t next_to_top, size_t top, size_t pi, vector_of_points const &data)
{
    return cross(next_to_top, top, pi, data) <= 0;
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
    double x0 = data_counter_clock_from_p0[0].x;
    double y0 = data_counter_clock_from_p0[0].y;
    std::sort(data_counter_clock_from_p0.begin() + 1, data_counter_clock_from_p0.end(), [x0, y0](const Point &a,
                                                                                                 const Point &b)
    {
        Point at{a.y - y0, a.x - x0};
        Point bt{b.y - y0, b.x - x0};
        double cross = at.x * bt.y - at.y * bt.x;
        if (cross == 0)
        {
            double dis_a_p0 = distance_pow2(a.x, a.y, x0, y0);
            double dis_b_p0 = distance_pow2(b.x, b.y, x0, y0);
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
    for (size_t i = 0; i < _stack._data.size(); i++) //NOLINT
    {
        ret->push_back(data_counter_clock_from_p0[_stack._data[i]]);
    }
    _stack._data.clear();
    return *ret;
}


int main()
{
    int N;
    std::cin >> N;
    vector_of_points data;
    for (int i = 0; i < N; ++i)
    {
        double x, y;
        std::cin >> x >> y;
        data.emplace_back(x, y);
    }

    if (N == 1)
    {
        std::cout << 0 << std::endl;
        std::cout << data[0].x << " " << data[0].y;
    }

    std::sort(data.begin(), data.end(), [](const Point &a, const Point &b)
    {
        if (a.x == b.x) return a.y <= b.y;
        return a.x < b.x;
    });

    vector_of_points to;
    if (!data.empty()) to.emplace_back(data[0].x, data[0].y);
    for (int i = 1; i < data.size(); i++)
    {
        if (data[i] != data[i - 1])
        {
            to.emplace_back(data[i].x, data[i].y);
        }
    }

    const vector_of_points &ans = graham(to);
    int sz = int(ans.size());
    double dist_pow2 = 0;
    int index_low = 0;
    auto it_to_max_y_element = std::max_element(ans.begin(), ans.end(),
                                                [](const Point &a, const Point &b)
                                                {
                                                    return a.y < b.y;
                                                });
    int index_high = static_cast<int>(std::distance(ans.begin(), it_to_max_y_element));
    int iter_from_high = index_high;
    int iter_from_low = index_low;

    int max_index_low = index_low, max_index_high = index_high;
    while (iter_from_high != index_low || iter_from_low != index_high)
    {
        double dist_pow2_tmp = distance_pow2(ans[iter_from_low].x, ans[iter_from_low].y,
                                             ans[iter_from_high].x, ans[iter_from_high].y);
        //dist_pow2 = std::max(dist_pow2, dist_pow2_tmp);
        if (dist_pow2_tmp > dist_pow2)
        {
            max_index_high = iter_from_high;
            max_index_low = iter_from_low;
            dist_pow2 = dist_pow2_tmp;
        }
        /**
         *    ...<--
         *
         *
         *        -->...
         */
        Point lowv = ans[iter_from_low];
        Point lowv_next = ans[(iter_from_low + 1) % sz];

        Point upv = ans[iter_from_high];
        Point upv_next = ans[(iter_from_high + 1) % sz];
        Point delta_up = {upv_next.x - upv.x, upv_next.y - upv.y};
        Point delta_up_from_lownext = {delta_up.x + lowv_next.x, delta_up.y + lowv_next.y};
        double rot = cross(lowv, lowv_next, delta_up_from_lownext);
        if (rot < 0)
        {
            iter_from_low++;
            iter_from_low %= sz;
        }
        else if (rot > 0)
        {
            iter_from_high++;
            iter_from_high %= sz;
        }
        else
        {
            iter_from_low++;
            iter_from_low %= sz;
            iter_from_high++;
            iter_from_high %= sz;
        }
    }
    double r = sqrt(dist_pow2) / 2;
    Point a1 = ans[max_index_high];
    Point a2 = ans[max_index_low];
    Point mid = {(a1.x + a2.x) / 2, (a1.y + a2.y) / 2};
    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << r << std::endl;
    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << mid.x << " " << mid.y;
    return 0;
}