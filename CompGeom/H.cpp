#include <algorithm>
#include <iostream>
#include <vector>

typedef long long ll;

using namespace std;

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

bool belongs(const Point &p1, const Point &p2, const Point &q)
{
    ll rot = is_left(p1, p2, q);
    if (rot == 0)
    {
        ll low = min(p1.y, p2.y);
        ll high = max(p1.y, p2.y);
        ll left = min(p1.x, p2.x);
        ll right = max(p1.x, p2.x);
        return q.x >= left && q.x <= right &&
               q.y >= low && q.y <= high;
    }
    return false;
}

/**
 * Pre hull: no 3 points in a line
 * 0 - border
 * 1 - inside
 * -1 - outside
 */
int check_inside(const vector_of_points &_hull, const Point &q)
{
    int n = _hull.size();
    ll edgeR = is_left(_hull[0], _hull[1], q);
    ll edgeL = is_left(_hull[n - 1], _hull[0], q);
    if (edgeR < 0) return -1;
    if (edgeL < 0) return -1;
    if (belongs(_hull[0], _hull[1], q)) return 0;
    if (belongs(_hull[0], _hull[n - 1], q)) return 0;
    int l = 1, r = n;
    while (l < r - 1)
    {
        int m = l + (r - l) / 2;
        is_left(_hull[0], _hull[m], q) > 0 ? l = m : r = m;
    }
    Point incl = _hull[l], excl = _hull[l + 1];
    ll segment = is_left(incl, excl, q);
    if (segment == 0) return 0;
    if (segment > 0) return 1;
    return -1;
}

int main()
{
    int n;
    cin >> n;
    vector_of_points tmp_hull;
    int index_min_y = 0;
    for (int i = 0; i < n; ++i)
    {
        ll x, y;
        cin >> x >> y;
        tmp_hull.emplace_back(x, y);
        if (tmp_hull[i].y < tmp_hull[index_min_y].y)
        { index_min_y = i; }
    }

    vector_of_points hull_counter_clock = correct_hull(tmp_hull, index_min_y);
    int k;
    cin >> k;
    for (int i = 0; i < k; ++i)
    {
        ll x, y;
        cin >> x >> y;
        Point q{x, y};
        int ans = check_inside(hull_counter_clock, q);
        switch (ans)
        {
            case 0:
                cout << "BORDER" << endl;
                break;
            case -1:
                cout << "OUTSIDE" << endl;
                break;
            case 1:
                cout << "INSIDE" << endl;
                break;
            default:
                break;
        }
    }
    return 0;
}