#include "worker.h"
#include "matrix.h"
#include "dglgeometry.h"
#include "shader.h"

Worker::Worker(Matrix<float> const *model_view,
               Matrix<float> const *proj,
               Matrix<float> const *viewport) : model_view(model_view),
                                                proj(proj), viewport(viewport)
{
    qRegisterMetaType<vec3f>("vec3f");
    MVP = (*viewport) * (*proj) * (*model_view);
}

void Worker::draw_cube()
{

//    Shader shader(model_view, proj, viewport);
//    vec3f
//    cube[6][4]
//    {
//        {
//            vec3f{-0.5f, -0.5f, 0.5f},
//                    vec3f{0.5f, -0.5f, 0.5f},
//                    vec3f{0.5f, 0.5f, 0.5f},
//                    vec3f{-0.5f, 0.5f, 0.5f},
//        },
//        {
//            vec3f{-0.5f, -0.5f, -0.5f},
//                    vec3f{-0.5f, 0.5f, -0.5f},
//                    vec3f{0.5f, 0.5f, -0.5f},
//                    vec3f{0.5f, -0.5f, -0.5f},
//        },
//        {
//            vec3f{-0.5f, 0.5f, -0.5f},
//                    vec3f{-0.5f, 0.5f, 0.5f},
//                    vec3f{0.5f, 0.5f, 0.5f},
//                    vec3f{0.5f, 0.5f, -0.5f},
//        },
//        {
//            vec3f{-0.5f, -0.5f, -0.5f},
//                    vec3f{0.5f, -0.5f, -0.5f},
//                    vec3f{0.5f, -0.5f, 0.5f},
//                    vec3f{-0.5f, -0.5f, 0.5f},
//        },
//        {
//            vec3f{0.5f, -0.5f, -0.5f},
//                    vec3f{0.5f, 0.5f, -0.5f},
//                    vec3f{0.5f, 0.5f, 0.5f},
//                    vec3f{0.5f, -0.5f, 0.5f},
//        },
//        {
//            vec3f{-0.5f, -0.5f, -0.5f},
//                    vec3f{-0.5f, -0.5f, 0.5f},
//                    vec3f{-0.5f, 0.5f, 0.5f},
//                    vec3f{-0.5f, 0.5f, -0.5f},
//        }
//    };

//    for (auto &i : cube)
//    {
//        for (auto &j : i)
//        {
//            j = shader.count_coordinates(j);

//        }
//    }


//    for (int i = 0; i < 6; i++)
//    {
//        emit draw_quad(cube[i][0],
//                       cube[i][1],
//                       cube[i][2],
//                       cube[i][3],
//                       0, 0, 255, 255);
//    }
//    emit finished();
}
