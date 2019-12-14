#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <cmath>

#include "primitives.h"

#define FOR(i, n) for (int i = 0; i < n; i++)

using namespace PRIMITIVES;

using namespace std;
namespace TRIANGULATION

{
typedef std::vector<PRIMITIVES::Point> vector_of_points;


const int INF = numeric_limits<int>::max();
const double EPSILON = 0.000001;

/**
circumCircle() :
   Return true if a Point (xp,yp) is inside the circumcircle made up
   of the points (x1,y1), (x2,y2), (x3,y3)
   The circumcircle centre is returned in (xc,yc) and the radius r
   Note : A Point on the edge is inside the circumcircle
*/

bool circumCircle(double xp, double yp, double x1, double y1, double x2,
                  double y2, double x3, double y3, double &xc, double &yc, double &r);

double is_left(const Point &p1, const Point &p2, const Point &q);

/**
 * Pre: p0, p1, p2 counter clockwise
 */
bool dots_inside_triangle(vector_of_points const &hull,
                          Point const &p0, Point const &p1, Point const &p2);

vector<Triangle> const& triangulate(vector_of_points const& _hull);
};

#endif // TRIANGULATION_H
