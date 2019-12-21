#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <functional>
#include <cmath>
#include <limits>
#include <cassert>

#define FOR(i, n) for (int i = 0; i < n; i++)

using namespace std;

struct Point;
struct Node;

//template class std::unique_ptr<Node>;

struct Point
{
    double x, y;
    int index; //-1 if is not from array

    Point(double x, double y, int index = -1) : x(x), y(y), index(index)
    {}
};

typedef vector<Point> vector_of_points;

struct Node
{
    const int MAX_POINTS_IN = 4;

    unique_ptr<Node> LB;
    unique_ptr<Node> LU;
    unique_ptr<Node> RB;
    unique_ptr<Node> RU;

    Point LBCorner;
    Point RUCorner;
    vector<Point> points{};

    Node(const Point &LBCorner, const Point &RUCorner, const vector<Point> &pointsIn = vector<Point>{}) :
            LBCorner(LBCorner), //NOLINT
            RUCorner(RUCorner)
    {
        if (pointsIn.size() <= MAX_POINTS_IN)
        {
            points = pointsIn;
            return;
        }
        /**
         * careful not to skip m
         * LU-----------RU
         *   |    |-   |
         *   |  | | |  |
         *   -----m-----
         *   |    |    |
         *   |   -|    |
         * LB-----------RB
         */
        Point med = {(LBCorner.x + RUCorner.x) / 2, (LBCorner.y + RUCorner.y) / 2};
        vector_of_points LBTransfer, LUTransfer, RBTransfer, RUTransfer;
        for (auto &point : pointsIn)
        {
            if (point.x <= med.x && point.y < med.y) LBTransfer.push_back(point);
            if (point.x < med.x && point.y >= med.y) LUTransfer.push_back(point);
            if (point.x > med.x && point.y < med.y) RBTransfer.push_back(point);
            if (point.x >= med.x && point.y >= med.y) RUTransfer.push_back(point);
        }
        LB = make_unique<Node>(LBCorner, med, /**std::move*/(LBTransfer));
        LU = make_unique<Node>(Point{LBCorner.x, med.y},
                               Point{med.x, RUCorner.y},
                /**std::move*/(LUTransfer));
        RB = make_unique<Node>(Point{med.x, LBCorner.y},
                               Point{RUCorner.x, med.y},
                /**std::move*/(RBTransfer));
        RU = make_unique<Node>(med, RUCorner, /**std::move*/(RUTransfer));
    }

    friend double distance_to_border(Node const *from, Point relative_point);
};

double distance_to_border(Node const *from, Point relative_point)
{
    Point mid = {(from->LBCorner.x + from->RUCorner.x) / 2,
                 (from->LBCorner.y + from->RUCorner.y) / 2};
    Point edge_half = {(from->RUCorner.x - from->LBCorner.x) / 2,
                       (from->RUCorner.y - from->LBCorner.y) / 2};
    Point r = {abs(relative_point.x - mid.x), abs(relative_point.y - mid.y)};
    double x_to_border = max(0.0, r.x - edge_half.x);
    double y_to_border = max(0.0, r.y - edge_half.y);
    return sqrt(x_to_border * x_to_border + y_to_border * y_to_border);
}


struct QuadTree
{
    constexpr static double _MAX = 1000 * 4;
    constexpr static double _MIN = -1000 * 4;
    unique_ptr<Node> _root;

    explicit QuadTree(const vector_of_points &points)
    {
        _root = make_unique<Node>(Point{_MIN, _MIN}, Point{_MAX, _MAX}, points);
    }

    Point closest(Point point)
    {
        double dist_min = numeric_limits<double>::max();
        Point ret{_MAX, _MAX};

        auto cmp = [point](Node const *lhs, Node const *rhs)
        {
            return distance_to_border(lhs, point) < distance_to_border(rhs, point);
        };

        //miltiset not to delete nodes with the same distance to border from point
        multiset<Node *, decltype(cmp)> nodes(cmp);
        nodes.emplace(_root.get());
        while (!nodes.empty())
        {
            Node *node = *nodes.begin();
            if (dist_min <= distance_to_border(node, point)) break;
            nodes.erase(nodes.begin());
            if (node->LB == nullptr)
            {
                for (auto &p : node->points)
                {
                    Point delta = {p.x - point.x, p.y - point.y};
                    double cur_dist = sqrt(delta.x * delta.x + delta.y * delta.y);
                    if (cur_dist < dist_min)
                    {
                        ret = p;
                        dist_min = cur_dist;
                    }
                }
            } else
            {
                nodes.emplace(node->LB.get());
                nodes.emplace(node->LU.get());
                nodes.emplace(node->RB.get());
                nodes.emplace(node->RU.get());
            }
        }
        return ret;
    }

};

int main()
{
    /**
    2 2
    10 2
    3 3
    -4 -9
    -6 -1
     */
    int n, m;
    cin >> n >> m;

    vector_of_points points;
    FOR(i, n)
    {
        // [-1000 <= x, y <= 1000]
        double x, y;
        cin >> x >> y;
        points.emplace_back(x, y, i);
    }
    QuadTree qtree(points);

    FOR(i, m)
    {
        double xquery, yquery;
        cin >> xquery >> yquery;
        Point ans = qtree.closest(Point{xquery, yquery});
        int ret = ans.index + 1;
        assert(ret != 0);
        cout << ret << endl;
    }
    return 0;
}

/**
15 1
1 1 2 2 3 3 4 4 5 5
1 2 2 3 3 4 4 5 5 6
1 3 2 4 3 5 4 6 5 7
0 3

5 1
1 1 2 2 3 3 4 4 5 5
0 3
*/