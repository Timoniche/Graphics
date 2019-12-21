#include "dgl_object.h"

dgl_object::dgl_object()
{

}

dgl_object::~dgl_object()
{

}

dgl_cube::dgl_cube(vec3f q[6][4], BMP _bmp)
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            quads[i][j] = q[i][j];
        }
    }
    bmp = _bmp;
}

dgl_cube::~dgl_cube()
{}
