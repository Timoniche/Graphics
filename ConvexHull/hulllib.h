#ifndef HULLLIB_H
#define HULLLIB_H

#include "hulllib_global.h"
#include <vector>

#include "primitives.h"

const float PI_F = 3.14159265358979f;
const float PI_F_2 = 3.14159265358979f / 2;

using namespace PRIMITIVES;
typedef std::vector<Point> vector_of_points;

class /**HULLLIBSHARED_EXPORT**/ HullLib
{

public:
    HullLib();
    /**
     * O(n*logn)
     */
    vector_of_points const& graham(vector_of_points const &points);

    /**
     * O(n^2)
     */
    vector_of_points const& gift_wrapping(vector_of_points const &points);



private:
    constexpr static float eps = float(0.00000001);
    float distance_pow2(float, float, float, float) const;
    bool left_directed(size_t, size_t, size_t, vector_of_points const&) const;
    bool equal(float, float) const;
    bool equal(float, float, float, float) const;
    float a_tan2(float, float, bool) const;
    float a_tan2_from_0_to_2pi(float, float) const;
private:
    StackExtended _stack;
};

#endif // HULLLIB_H
