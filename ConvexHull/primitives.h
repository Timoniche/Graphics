#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>

#define FOR(i, n) for (int i = 0; i < n; i++)


namespace PRIMITIVES {
struct Point;
struct Half_edge;
struct Face;

const int INF = std::numeric_limits<int>::max();
const double EPSILON = 0.000001;

template<typename T>
bool equal(T a, T b);


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
    std::vector<int> _data;
};

struct Point
{
    double x = -1;
    double y = -1;
    Half_edge *half_edge = nullptr;

    Point();

    Point(double x, double y, Half_edge *half_edge = nullptr);

    Point(Point const &p);

    Point(Point &&p);

    Point &operator=(Point const &p);

    Point &operator=(Point &&p);

    void swap(Point &p);

    friend std::ostream &operator<<(std::ostream &os, const Point &dt);

    friend bool operator==(Point const &lhs, Point const &rhs);

    friend bool operator!=(Point const &lhs, Point const &rhs);
};


//______________________________________________________________________________________________________________________

struct Triangle
{
    Point a;
    Point b;
    Point c;

    Triangle(Point a, Point b, Point c);
};
}


#endif // PRIMITIVES_H
