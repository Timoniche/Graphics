//
// Created by Timoniche on 11/14/2019.
//

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


vector<pll> points;
tuple<pll, pll, long double> ans = {{}, {}, numeric_limits<long double>::max()};

long double dist(pll const &a, pll const &b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void find(ll l, ll r)
{
    if (r - l <= 3)
    {
        for (ll i = l; i <= r; ++i)
        {
            for (ll j = i + 1; j <= r; ++j)
            {
                long double run = dist(points[i], points[j]);
                if (run < get<2>(ans))
                {
                    get<0>(ans) = points[i];
                    get<1>(ans) = points[j];
                    get<2>(ans) = run;
                }
            }
        }
        return;
    }
    ll m = l + (r - l) / 2;
    find(l, m);
    find(m + 1, r);
    ll left_index = m, right_index = m + 1;
    for (; left_index >= l; --left_index)
    {
        if (dist(points[left_index], points[m]) > get<2>(ans))
        {
            break;
        }
    }
    left_index = max(l, left_index);

    for (; right_index <= r; right_index++)
    {
        if (dist(points[right_index], points[m + 1]) > get<2>(ans))
        {
            break;
        }
    }
    right_index = min(r, right_index);

    for (ll i = left_index; i <= m; ++i)
    {
        for (ll j = m + 1; j <= right_index; ++j)
        {
            long double run = dist(points[i], points[j]);
            if (run < get<2>(ans))
            {
                get<0>(ans) = points[i];
                get<1>(ans) = points[j];
                get<2>(ans) = run;
            }
        }
    }
}

int main()
{
    freopen("rendezvous.in", "r", stdin);
    freopen("rendezvous.out", "w", stdout);
    ll n;
    cin >> n;
    vector<pll> *rpoints = &points;
    tuple<pll, pll, long double> *rans = &ans;

    for (ll i = 0; i < n; ++i)
    {
        ll x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
    }
    sort(points.begin(), points.end());
    find(0, n - 1);
    cout << get<0>(ans).x << " " << get<0>(ans).y
         << " " << get<1>(ans).x << " " << get<1>(ans).y;
    return 0;
}







