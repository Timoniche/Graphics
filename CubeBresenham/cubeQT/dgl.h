#ifndef DGL_H
#define DGL_H

#include "dglgeometry.h"

/**
 * own gl library
 */

using namespace DGL;

struct dgl_vertex
{
public:
    vec3f world_coordinates;
    vec3f final_coordinates;

    explicit dgl_vertex(vec3f world);
};

class dgl
{
public:
    dgl();
};

#endif // DGL_H
