#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <limits>

#define FOR(i, n) for (int i = 0; i < n; i++)

using namespace std;

typedef long double ld;
const int MAXN = 6 * 100000;
const ld eps = 1e-7;

int sgn(ld x)
{
    if (fabs(x) < eps)
    { return 0; }
    else if (x > eps)
    { return 1; }
    else
    { return -1; }
}

struct Point
{
    ld x{}, y{};

    Point() = default;

    Point(ld a, ld b)
    {
        x = a;
        y = b;
    }

    long double operator*(const Point &A)
    { return x * A.x + y * A.y; }

    Point operator/(const ld &A)
    { return {x / A, y / A}; }

    ld operator^(const Point &A)
    { return x * A.y - A.x * y; }

    bool operator==(const Point &a) const
    { return (sgn(x - a.x) == 0) && (sgn(y - a.y) == 0); }

    bool operator<(const Point &a) const
    { return (sgn(x - a.x) == 0) ? (sgn(y - a.y) < 0) : (x < a.x); }

    friend Point operator-(const Point lhs, const Point A)
    { return {lhs.x - A.x, lhs.y - A.y}; }

    friend Point operator+(const Point lhs, const Point &A)
    { return {lhs.x + A.x, lhs.y + A.y}; }
};

struct Line
{
    Point a, b;

    Line() = default;

    Line(Point _a, Point _b)
    {
        a = _a;
        b = _b;
    }
} L[MAXN]; //NOLINT

Point borders[6], P[MAXN];

ld cross(Point c, Point a, Point b)
{
    return (a - c) ^ (b - c);
}

bool isLeft(Point c, Line l)
{
    return sgn(cross(l.a, c, l.b)) <= 0; //NOLINT
}


class angle_sort
{
    Point m_origin;
    Point m_dreference;

    static ld xp(Point a, Point b)
    { return a.x * b.y - a.y * b.x; }

public:
    angle_sort(const Point origin, const Point reference) : m_origin(origin), m_dreference(reference - origin)
    {}

    bool operator()(const Point a, const Point b) const
    {
        const Point da = a - m_origin, db = b - m_origin;
        const ld detb = xp(m_dreference, db);
        if (sgn(detb) == 0 && db.x * m_dreference.x + db.y * m_dreference.y >= 0) return false;
        const ld deta = xp(m_dreference, da);
        if (sgn(deta) == 0 && da.x * m_dreference.x + da.y * m_dreference.y >= 0) return true;
        if (deta * detb >= 0)
        {
            return sgn(xp(da, db)) > 0;
        }
        return sgn(deta) > 0;
    }
} sorter{{0, 0},
         {-1, 0}};

bool cmp(Line l1, Line l2)
{
    Point a = l1.b - l1.a;
    Point b = l2.b - l2.a;
    return ((a ^ b) == 0 && (a * b) >= 0) ? isLeft(l1.a, l2) : sorter(a, b);
}

Point lineLineIntersect(Point A, Point B, Point C, Point D)
{
    ld t1 = cross(A, C, B), t2 = cross(A, B, D);
    return (C + (D - C) / (1 / (t1 / (t1 + t2))));
}

Point lineLineIntersect(Line A, Line B)
{
    return lineLineIntersect(A.a, A.b, B.a, B.b);
}

Point lineLineIntersectNoBorders(Line A, Line B)
{
    Point v1 = A.b - A.a;
    Point v2 = B.b - B.a;
    if ((v1 ^ v2) == 0) return {numeric_limits<double>::max(), numeric_limits<double>::max()};
    return lineLineIntersect(A.a, A.b, B.a, B.b);
}


int halfPlaneIntersection(int n, Point *P)
{
    sort(L + 1, L + n + 1, cmp);
    Line q[n + 2];
    Point ans[n + 2];
    int head0 = 1, tail0 = 0;
    int head1 = 1, tail1 = 0;
    q[++tail0] = L[1];
    for (int i = 2; i <= n; ++i)
    {
        Point v1 = L[i].b - L[i].a;
        Point v2 = L[i - 1].b - L[i - 1].a;
        if (sgn(v1 ^ v2) == 0 && (v1 * v2) >= 0) continue;

        while (head1 <= tail1 && !isLeft(ans[tail1], L[i]))
        {
            --tail1;
            --tail0;
        }
        while (head1 <= tail1 && !isLeft(ans[head1], L[i]))
        {
            ++head1;
            ++head0;
        }

        ans[++tail1] = lineLineIntersect(q[tail0], L[i]);
        q[++tail0] = L[i];
    }
    while (head1 <= tail1 && !isLeft(ans[tail1], q[head0]))
    {
        --tail1;
        --tail0;
    }
    while (head1 <= tail1 && !isLeft(ans[head1], q[tail0]))
    {
        ++head1;
        ++head0;
    }
    ans[++tail1] = lineLineIntersect(q[tail0], q[head0]);
    if (tail1 - head1 + 1 <= 2)
    {
        return 0;
    }
    int m = 0;
    for (int i = head1; i <= tail1; ++i) P[++m] = ans[i];
    return m;
}

int halfPlaneIntersectionNoBorders(int n, Point *P)
{
    sort(L + 1, L + n + 1, cmp);
    Line q[n + 2];
    Point ans[n + 2];
    int head0 = 1, tail0 = 0;
    int head1 = 1, tail1 = 0;
    q[++tail0] = L[1];
    for (int i = 2; i <= n; ++i)
    {
        Point v1 = L[i].b - L[i].a;
        Point v2 = L[i - 1].b - L[i - 1].a;
        if (sgn(v1 ^ v2) == 0 && (v1 * v2) >= 0) continue;

        while (head1 <= tail1 && !isLeft(ans[tail1], L[i]))
        {
            --tail1;
            --tail0;
        }
        while (head1 <= tail1 && !isLeft(ans[head1], L[i]))
        {
            ++head1;
            ++head0;
        }
//4 0 1 0 0 -1 3 -1 0 0 1 0 3
//3 0 1 0 0 -1 3 1 0 0 1 0 3
        ans[++tail1] = lineLineIntersectNoBorders(q[tail0], L[i]);
        if (ans[tail1] == Point{numeric_limits<double>::max(),numeric_limits<double>::max()}) return 0;
        q[++tail0] = L[i];
    }
    while (head1 <= tail1 && !isLeft(ans[tail1], q[head0]))
    {
        --tail1;
        --tail0;
    }
    while (head1 <= tail1 && !isLeft(ans[head1], q[tail0]))
    {
        ++head1;
        ++head0;
    }
    ans[++tail1] = lineLineIntersectNoBorders(q[tail0], q[head0]);
    if (ans[tail1] == Point{numeric_limits<double>::max(),numeric_limits<double>::max()}) return 0;
    if (tail1 - head1 + 1 <= 2)
    {
        return 0;
    }
    int m = 0;
    for (int i = head1; i <= tail1; ++i) P[++m] = ans[i];
    return m;
}

ld countSquare(Point *p, int n)
{
    ld res = 0;
    p[n + 1] = p[1];
    for (int i = 1; i <= n; ++i)
        res += (p[i] ^ p[i + 1]);
    res = fabs(res) / 2;
    //if (res < eps) return 0;
    return res;
}

long double is_left(const Point &p1, const Point &p2, const Point &q)
{
    Point u = {p2.x - p1.x,
               p2.y - p1.y};
    Point v = {q.x - p2.x,
               q.y - p2.y};
    return u.x * v.y - u.y * v.x;
}


bool belongs(const Point &p1, const Point &p2, const Point &q)
{
    long double rot = is_left(p1, p2, q);
    if (fabs(rot) < eps)
    {
        long double low = min(p1.y, p2.y);
        long double high = max(p1.y, p2.y);
        long double left = min(p1.x, p2.x);
        long double right = max(p1.x, p2.x);
        return q.x >= left - eps && q.x <= right + eps &&
               q.y >= low - eps && q.y <= high + eps;
    }
    return false;
}

ld dist(Point& a, Point& b)
{
    return sqrt(a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

bool counter(Line l1, Line l2)
{
    Point v1 = l1.b - l1.a;
    Point v2 = l2.b - l2.a;
    //check for parallelism and direction
    if ((v1 ^ v2) == 0 && sgn(v1 * v2) < 0)
    {
        //check on one line
        if (sgn(cross(l1.a, l1.b, l2.a)) == 0) //NOLINT
        {
            return true;
        }
    }
    return false;
}


int main()
{
    int bsz = 10'000'000;
    borders[0] = Point(-bsz - 5, -bsz);
    borders[1] = Point(bsz + 5, -bsz);
    borders[2] = Point(bsz + 5, bsz);
    borders[3] = Point(-bsz - 5, bsz);
    borders[4] = borders[0];
    int n;
    cin >> n;
    int cnt = 0;
    FOR(i, n)
    {
        //ax + by + c > 0
        ld a, b, c;
        cin >> a >> b >> c;
        Point p;
        Point v = Point(b, -a);
        if (fabs(a) > fabs(b))
        { p = Point(-c / a, 0); }
        else
        { p = Point(0, -c / b); }
        L[++cnt] = Line(p, p + v);
    }

    //searching for counter parallel lines N^2
    for (int i = 1; i <= cnt; ++i)
    {
        for (int j = i + 1; j <= cnt; ++j)
        {
            auto line1 = L[i];
            auto line2 = L[j];
            if (counter(line1, line2))
            {
                cout << 0 << endl;
                return 0;
            }
        }
    }

    int sz = halfPlaneIntersectionNoBorders(cnt, P);
    if (sz > 0)
    {
        ld sq = countSquare(P, sz);
        cout << sq << endl;
        return 0;
    }
    for (int j = 0; j < 4; ++j) L[++cnt] = Line(borders[j], borders[j + 1]);
    sz = halfPlaneIntersection(cnt, P);
    cout.precision(numeric_limits<ld>::max_digits10);
    if (sz == 0)
    { cout << 0 << endl; }
    else
    {
        for (int i = 1; i <= sz; ++i)
        {
            if (belongs(borders[0], borders[1], P[i]) ||
                belongs(borders[1], borders[2], P[i]) ||
                belongs(borders[2], borders[3], P[i]) ||
                belongs(borders[3], borders[4], P[i]))
            {
                    cout << -1 << endl;
                    return 0;
            }
        }

        ld sq = countSquare(P, sz);
        cout << sq << endl;

    }
    return 0;
}

//4 1 1 1 -1 1 1 1 -1 1 -1 -1 1 == 2