#ifndef HULLLIB_H
#define HULLLIB_H

#include "hulllib_global.h"
#include <vector>

struct HULLLIBSHARED_EXPORT StackExtended
{
public:
    void push(size_t x);
    int top();
    int top_next();
    int pop();
public:
    std::vector<size_t> _data;
};

struct HULLLIBSHARED_EXPORT Point
{
    float x;
    float y;

    Point(float x, float y);
};

typedef std::vector<Point> vector_of_points;

class HULLLIBSHARED_EXPORT HullLib
{

public:
    HullLib();
    friend struct Point;
    friend struct StackExtended;
    /**
     * O(n*logn)
     */
    vector_of_points const& graham(vector_of_points const &points);

private:
    constexpr static float eps = float(0.00000001);
    float distance_pow2(float, float, float, float) const;
    bool left_directed(size_t, size_t, size_t, vector_of_points const&) const;
    bool equal(float, float) const;
    bool equal(float, float, float, float) const;
    float a_tan2(float, float, bool) const;
    float a_tan2_from_0_to_pi(float, float) const;
    std::vector<bool> used_points;
private:
    StackExtended _stack;
};

#endif // HULLLIB_H
