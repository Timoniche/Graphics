#include <GL/glut.h>
#include <stdio.h>
#include "Graphics.h"
#include <GL/glu.h>
#include <iomanip>
#include <iostream>
#include <array>
#include <vector>

using namespace std;

bool fullscreen = false;
bool mouseDown = false;

float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
GLuint texture[1];

int LoadBitmap(char *filename)
{

    unsigned char *l_texture;
    int i, j=0;
    FILE *l_file;
    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;
    int num_texture;

    if( (l_file = fopen(filename, "rb"))==NULL) return (-1);
    fread(&fileheader, sizeof(fileheader), 1, l_file);
    fseek(l_file, sizeof(fileheader), SEEK_SET);
    fread(&infoheader, sizeof(infoheader), 1, l_file);

    l_texture = (BYTE *) malloc(infoheader.biWidth * infoheader.biHeight * 4);
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

    for (i=0; i < infoheader.biWidth*infoheader.biHeight; i++)
    {
        fread(&rgb, sizeof(rgb), 1, l_file);

        l_texture[j+0] = rgb.rgbtRed; // Red component
        l_texture[j+1] = rgb.rgbtRed; // Green component
        l_texture[j+2] = rgb.rgbtBlue; // Blue component
        l_texture[j+3] = 255; // Alpha value
        j += 4; // Go to the next position
    }
    fclose(l_file);
    glBindTexture(GL_TEXTURE_2D, num_texture);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, infoheader.biWidth, infoheader.biHeight, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
    free(l_texture);
    return num_texture;

}

//GLuint LoadTexture(const char *filename)
//{
//    GLuint texture;
//    int width, height;
//    unsigned char *data;
//    FILE *file;
//    file = fopen(filename, "rb");
//    if (file == nullptr) return 0;
//    width = 256;
//    height = 256;
//    data = (unsigned char *) malloc(static_cast<size_t>(width * height * 3));
//    fread(data, static_cast<size_t>(width * height * 3), 1, file);
//    fclose(file);
//
//    for (int i = 0; i < width * height; ++i)
//    {
//        int index = i * 3;
//        unsigned char B, R;
//        B = data[index];
//        R = data[index + 2];
//
//        data[index] = R;
//        data[index + 2] = B;
//
//    }
//
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
//    free(data);
//
//    return texture;
//}

void drawBox()
{
    //glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);

    glColor3f(.0f, 1.f, .0f);
    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    //glTexCoord2f(1.f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    //glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    //glTexCoord2f(0.0f, 1.f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    //glTexCoord2f(1.f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    //glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    //glTexCoord2f(0.0f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    //glTexCoord2f(0.0f, 1.f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    //glTexCoord2f(1.f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    //glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    //glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    //glTexCoord2f(0.0f, 1.f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    //glTexCoord2f(1.f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    //glTexCoord2f(1.f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    //glTexCoord2f(1.f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    //glTexCoord2f(0.0f, 1.f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    //glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    //glTexCoord2f(1.f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    //glTexCoord2f(1.f, 1.f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    //glTexCoord2f(0.0f, 1.f);
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

    //texture[0] = LoadTexture("./one.bmp");
    //texture[0] = static_cast<GLuint>(LoadBitmap(const_cast<char *>("./one.bmp")));


   // glEnable(GL_TEXTURE_2D);
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
            0.2f, 0.5f, 0.3f,
            0.0f, 1.0f, 0.0f);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);


    drawBox();

    glFlush();

    freopen("C:/Users/Timoniche/Desktop/Graphics/CubeModel/screen", "w", stdout);
    GLubyte *data = (GLubyte *) (malloc(4 * 300 * 300));
    if (data)
    {
        glReadPixels(0, 0, 300, 300, GL_RGBA, GL_UNSIGNED_BYTE, data);
        for (int i = 0; i < 4 * 300 * 300; i += 4)
        {
            if (data[i + 0] != 0 || data[i + 1] != 0 || data[i + 2] != 0)
            {
                int xi = (i / 4) / 300;
                int yi = (i / 4) % 300;
                cout << xi << " " << yi;
//                cout << "[" << xi << " " << yi << "] ";
//                cout << "r: " << (int) data[i + 0] << " ";
//                cout << "g: " << (int) data[i + 1] << " ";
//                cout << "b: " << (int) data[i + 2] << " ";
//                cout << "a: " << (int) data[i + 3] << " ";
                std::cout << std::endl;
            }
        }
    }

    glutSwapBuffers();
}

void resize(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int min_side = std::min(w, h);
    int delta_w = std::abs(w - min_side) / 2;
    int delta_h = std::abs(h - min_side) / 2;
    glViewport(delta_w, delta_h, min_side, min_side);

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


void get_pixel(int x, int y)
{
    std::cout << "x " << x << " y " << y << " ";
    std::vector< unsigned char > pixels( 1 * 1 * 4 );
    glReadPixels
            (
                    x, glutGet( GLUT_WINDOW_HEIGHT ) - y,
                    1, 1,
                    GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]
            );

    cout << "r: " << (int)pixels[0] << " ";
    cout << "g: " << (int)pixels[1] << " ";
    cout << "b: " << (int)pixels[2] << " ";
    cout << "a: " << (int)pixels[3] << " ";
    cout << endl;
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
            glutReshapeWindow(300, 300);
            glutPositionWindow(50, 50);
        }
    }
    if (key == GLUT_KEY_F2)
    {


//        get_pixel(229, 260);
//        for ( int row = 0 ; row < 500 ; ++row )
//            for ( int col = 0 ; col < 500 ; ++col )
//            {
//                get_pixel(col, row);
//            }
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
    resize(300, 300);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(300, 300);

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