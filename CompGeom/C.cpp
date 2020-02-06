#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>

#define x first
#define y second

using namespace std;

typedef long long ll;

struct Point
{
    ll x = 0;
    ll y = 0;

    Point() = default;

    Point(ll a, ll b) : x(a), y(b)
    {}

    friend bool operator<(Point const &a, Point const &b)
    {
        if (a.x == b.x) return a.y < b.y;
        return a.x < b.x;
    }

    friend Point operator-(Point const &a, Point const &b)
    {
        return {a.x - b.x, a.y - b.y};
    }
};

tuple<Point, Point, double> ans{}; //NOLINT

double distance(Point const &a, Point const &b)
{
    Point dist = a - b;
    return hypot(dist.x, dist.y);
}

ll dist_x(Point const &a, ll bx)
{
    return abs(bx - a.x);
}

void update_ans(Point const &a, Point const &b, double dist)
{
    get<0>(ans) = a;
    get<1>(ans) = b;
    get<2>(ans) = dist;
}

void update_brute_force(size_t l, size_t r, vector<Point> &points)
{
    for (size_t i = l; i <= r; ++i)
    {
        for (size_t j = i + 1; j <= r; ++j)
        {
            double run = distance(points[i], points[j]);
            if (run < get<2>(ans))
            {
                update_ans(points[i], points[j], run);
            }
        }
    }
}

void find(size_t l, size_t r, vector<Point> &points, vector<Point> &points_maintaining_y)
{
    if (r - l <= 3)
    {
        update_brute_force(l, r, points);
        sort(points.begin() + l,
             points.begin() + r + 1, [](Point const &a, Point const &b)
             {
                 return a.y < b.y;
             });
        return;
    }
    size_t m = l + (r - l) / 2;
    ll m_x = points[m].x; //store now, as points will be mixed
    find(l, m, points, points_maintaining_y);
    find(m + 1, r, points, points_maintaining_y);
    merge(points.begin() + l, points.begin() + m + 1,
          points.begin() + m + 1, points.begin() + r + 1,
          points_maintaining_y.begin(), [](Point const &a, Point const &b)
          {
              return a.y < b.y;
          });
    copy(points_maintaining_y.begin(),
         points_maintaining_y.begin() + r - l + 1, points.begin() + l);

    size_t sz = 0;
    for (size_t i = l; i <= r; ++i) //pi from B
    {
        //finding B: ai.x - med.x < dist
        if (dist_x(points[i], m_x) < get<2>(ans))
        {
            //finding in pj = C(pi from B): yi - h < yj <= yi
            for (ll j = sz - 1;
                 j >= 0 && points[i].y - points_maintaining_y[j].y < get<2>(ans);
                 j--)
            {
                double run = distance(points[i], points_maintaining_y[j]);
                if (run < get<2>(ans))
                {
                    update_ans(points[i], points_maintaining_y[j], run);
                }
            }
            points_maintaining_y[sz++] = points[i];
        }
    }
}

int main()
{
    freopen("rendezvous.in", "r", stdin);
    freopen("rendezvous.out", "w", stdout);
    get<2>(ans) = numeric_limits<double>::max();
    size_t n;
    cin >> n;
    vector<Point> points;
    vector<Point> points_maintaining_y;

    for (size_t i = 0; i < n; ++i)
    {
        size_t x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
    }
    sort(points.begin(), points.end());
    points_maintaining_y.resize(n);
    find(0, n - 1, points, points_maintaining_y);
    cout << get<0>(ans).x << " " << get<0>(ans).y << " " <<
         get<1>(ans).x << " " << get<1>(ans).y << endl;
    return 0;
}