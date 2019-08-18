#include <GL/glut.h>
//#include <GL/glaux.h>
#include <stdio.h>

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
    if (file == NULL) return 0;
    width = 256;
    height = 256;
    data = (unsigned char *) malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
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
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glEnd();
}

bool init()
{
    texture[0] = LoadTexture("C:/Users/Timoniche/Desktop/Graphics/CubeModel/TallGreenGrass.bmp");
    glEnable(GL_TEXTURE_2D);

    glClearColor(0.93f, 0.93f, 0.93f, 0.0f);

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
            0.0f, 0.0f, 3.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f);

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

    gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);

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