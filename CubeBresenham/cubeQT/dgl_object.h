#ifndef DGL_OBJECT_H
#define DGL_OBJECT_H

#include "dglgeometry.h"
#include "bmp.h"

#include <vector>

using namespace DGL;
class dgl_object
{
public:

    dgl_object();
    virtual ~dgl_object();
};

class dgl_cube : public dgl_object
{
public:
    BMP bmp{"C:/Users/Timoniche/Desktop/Graphics/CubeBresenham/cubeQT/one.bmp"};
    vec3f quads[6][4];
    vec2f textures[6][4]
    {
        {
            vec2f{0, 0},
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
        },
        {
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
            vec2f{0, 0},
        },
        {
            vec2f{0, 1},
            vec2f{0, 0},
            vec2f{1, 0},
            vec2f{1, 1},
        },
        {
            vec2f{1, 1},
            vec2f{0, 1},
            vec2f{0, 0},
            vec2f{1, 0},
        },
        {
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
            vec2f{0, 0},
        },
        {
            vec2f{0, 0},
            vec2f{1, 0},
            vec2f{1, 1},
            vec2f{0, 1},
        }
    };
    dgl_cube(vec3f q[6][4], BMP _bmp);
    ~dgl_cube() override;
};

#endif // DGL_OBJECT_H
