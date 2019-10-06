#include <GL/glut.h>
#include <stdio.h>
#include "Graphics.h"
#include <GL/glu.h>
#include <iomanip>
#include <iostream>
#include <array>
bool fullscreen = false;
bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
GLuint texture[1];

GLuint LoadTexture(const char *filename)
{
    GLuint texture;
    int width, height;
    unsigned char *data;
    FILE *file;
    file = fopen(filename, "rb");
    if (file == nullptr) return 0;
    width = 256;
    height = 256;
    data = (unsigned char *) malloc(static_cast<size_t>(width * height * 3));
    fread(data, static_cast<size_t>(width * height * 3), 1, file);
    fclose(file);

    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
}

void drawBox()
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    glTexCoord2f(1.f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glTexCoord2f(1.f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();
}

bool init()
{
    //
    static float amb[] =  {0.5, 0.5, 0.5, 1};
    static float dif[] =  {1.0, 1.0, 0.0, 1.0};

    float light_diffuse[] = {100.0, 0.0, 100.0, 100.0};
    float light_position[] = {-100.0f, 100.0, 1.0, 0.0};

//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
//    glEnable(GL_LIGHTING);
//    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    //

    texture[0] = LoadTexture("./TallGreenGrass.bmp");

    glEnable(GL_TEXTURE_2D);
    glClearColor(BLACK, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0f);
    return true;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(
            2, 2, 1, //eyeX eyeY eyeZ
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);
//    float proj[16];
//    glGetFloatv(GL_MODELVIEW_MATRIX, proj);
//
//    //std::cout << "w:" << w << " h:" << h << std::endl;
//    std::cout << "ModelView\n";
//    for (int i = 0; i < 16; ++i)
//    {
//        std::cout << proj[i] << " ";
//    }

    GLdouble model_view[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

    GLdouble projection[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble pos3D_x, pos3D_y, pos3D_z;
    double winX, winY, winZ;
    std::cout << "MODELVIEW" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        std::cout << model_view[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "PROJ" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        std::cout << projection[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Viewport" << std::endl;
    for (int i = 0; i < 4; ++i)
    {
        std::cout << viewport[i] << " ";
    }
    std::cout << std::endl;

    float cube[2][4][3]
            {
                    {
                            {-0.5f, -0.5f, 0.5f},
                            {0.5f, -0.5f, 0.5f},
                            {0.5f, 0.5f, 0.5f},
                            {-0.5f, 0.5f, 0.5f},
                    },
                    {
                            {-0.5f, -0.5f, -0.5f},
                            {-0.5f, 0.5f, -0.5f},
                            {0.5f, 0.5f, -0.5f},
                            {0.5f, -0.5f, -0.5f},
                    }
            };

    float a, b, c;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            a = cube[i][j][0];
            b = cube[i][j][1];
            c = cube[i][j][2];
            std::cout << "abc: " << a << " " << b << " " << c << std::endl;
            gluProject(a, b, c,
                       model_view, projection, viewport,
                       &winX, &winY, &winZ);
            std::cout << std::setprecision(9) << winX << " " << winY << " " << winZ << std::endl;
        }
    }


    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    drawBox();

    glFlush();
    glutSwapBuffers();
}

void resize(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glViewport(0, 0, w, h);

    gluPerspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    std::cout << std::endl << std::endl;
    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
            exit(1);
        default:
            break;
    }
}

void specialKeyboard(int key, int x, int y)
{
    if (key == GLUT_KEY_F1)
    {
        fullscreen = !fullscreen;

        if (fullscreen)
            glutFullScreen();
        else
        {
            glutReshapeWindow(500, 500);
            glutPositionWindow(50, 50);
        }
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseDown = true;

        xdiff = x - yrot;
        ydiff = -y + xrot;
    } else
        mouseDown = false;
}

void mouseMotion(int x, int y)
{
    if (mouseDown)
    {
        yrot = x - xdiff;
        xrot = y + ydiff;

        glutPostRedisplay();
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    resize(1 << 10, 1 << 9);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(500, 500);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("cube");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(resize);

    if (!init())
    {
        return 1;
    }

    glutMainLoop();

    return 0;
}