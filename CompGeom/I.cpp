#include <iostream>
#include <vector>
#include <algorithm>

#define FOR(i, n) for (int i = 0; i < n; i++)

using namespace std;

typedef long long ll;

/**
3 0 0
1 0
2 0
2 2
 */
struct Point
{
    ll x, y;

    Point(ll x, ll y) : x(x), y(y)
    {}
};

typedef vector<Point> vector_of_points;


ll is_left(const Point &p1, const Point &p2, const Point &q)
{
    Point u = {p2.x - p1.x,
               p2.y - p1.y};
    Point v = {q.x - p2.x,
               q.y - p2.y};
    return u.x * v.y - u.y * v.x;
}

bool belongs(const Point &ebeg, const Point &eend, const Point &q)
{
    ll rot = is_left(ebeg, eend, q);
    if (rot == 0)
    {
        ll low = min(ebeg.y, eend.y);
        ll high = max(ebeg.y, eend.y);
        ll left = min(ebeg.x, eend.x);
        ll right = max(ebeg.x, eend.x);
        return q.x >= left && q.x <= right &&
               q.y >= low && q.y <= high;
    }
    return false;
}


/**
 * Pre: hull.size() >= 3
 */
vector_of_points correct_hull(const vector_of_points &tmp_hull, int index_min_y)
{
    vector_of_points hull_counter_clock;
    for (int i = index_min_y; i < tmp_hull.size(); ++i)
    {
        hull_counter_clock.push_back(tmp_hull[i]);
    }
    for (int i = 0; i < index_min_y; ++i)
    {
        hull_counter_clock.push_back(tmp_hull[i]);
    }
    if (is_left(hull_counter_clock[0], hull_counter_clock[1], hull_counter_clock[2]) < 0)
    {
        reverse(hull_counter_clock.begin() + 1, hull_counter_clock.end());
    }
    return hull_counter_clock;
}

/**
 ray [q.x, INF)
 */
bool dot_in(const vector_of_points &hull, Point q)
{
    int intersections = 0;
    int n = hull.size();
    for (int i = 0; i < n; ++i)
    {
        Point ebeg = hull[i];
        Point eend = hull[(i + 1) % n];
        if (belongs(ebeg, eend, q)) return true;
        if (ebeg.y > eend.y) swap(ebeg, eend);
        if (q.y < ebeg.y || q.y >= eend.y) continue;
        ll rot = is_left(ebeg, eend, q);
        if (rot > 0) intersections++;
    }
    return intersections % 2 != 0;
}

int main()
{
    ll n, x0, y0;
    cin >> n >> x0 >> y0;
    vector_of_points _hull;
    int index_min_y = 0;
    FOR(i, n)
    {
        ll x, y;
        cin >> x >> y;
        _hull.emplace_back(x, y);
        if (_hull[i].y < _hull[index_min_y].y)
        { index_min_y = i; }
    }
    vector_of_points hull_counter_clock = correct_hull(_hull, index_min_y);
    if (dot_in(hull_counter_clock, {x0, y0}))
    {
        cout << "YES" << endl;
    } else
    {
        cout << "NO" << endl;
    }
    return 0;
}