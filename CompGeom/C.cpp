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
typedef pair<ll, ll> pll;

pll a1{};
pll b1{};
double dist = numeric_limits<double>::max();

double distance(pll const &a, pll const &b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

ll dist_x(pll const &a, ll bx)
{
    return abs(bx - a.x);
}

void find(size_t l, size_t r, vector<pll> &points, vector<pll> &points_maintaining_y)
{
    if (r - l <= 3)
    {
        for (size_t i = l; i <= r; ++i)
        {
            for (size_t j = i + 1; j <= r; ++j)
            {
                double run = distance(points[i], points[j]);
                if (run < dist)
                {
                    a1 = points[i];
                    b1 = points[j];
                    dist = run;
                }
            }
        }
        sort(points.begin() + l,
             points.begin() + r + 1, [](pll const &a, pll const &b)
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
          points_maintaining_y.begin(), [](pll const &a, pll const &b)
          {
              return a.y < b.y;
          });
    copy(points_maintaining_y.begin(),
         points_maintaining_y.begin() + r - l + 1, points.begin() + l);

    size_t sz = 0;
    for (size_t i = l; i <= r; ++i) //pi from B
    {
        //finding B: ai.x - med.x < dist
        if (dist_x(points[i], m_x) < dist)
        {
            //finding in pj = C(pi from B): yi - h < yj <= yi
            for (ll j = sz - 1;
                 j >= 0 && points[i].y - points_maintaining_y[j].y < dist;
                 j--)
            {
                double run = distance(points[i], points_maintaining_y[j]);
                if (run < dist)
                {
                    a1 = points[i];
                    b1 = points_maintaining_y[j];
                    dist = run;
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
    size_t n;
    cin >> n;
    vector<pll> points;
    vector<pll> points_maintaining_y;

    for (size_t i = 0; i < n; ++i)
    {
        size_t x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
    }
    sort(points.begin(), points.end());
    points_maintaining_y.resize(n);
    find(0, n - 1, points, points_maintaining_y);
    cout << a1.x << " " << a1.y << " " << b1.x << " " << b1.y << endl;
    return 0;
}







