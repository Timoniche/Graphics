#ifdef __APPLE__
#include <GLUT/glut.h>
#include <iostream>
#else
#include <GL/glut.h>
#endif

#include "Graphics.h"

void drawScene(void) {
    clearWindow();
    setColor(YELLOW);
    drawFilledTriangle(200,125,100,375,200,375);
    setColor(BLACK);
    drawLine(200,200,400,400);
    setColor(RED);
    drawFilledCircle(100,100,100);
    setColor(MAGENTA);
    drawFilledBox(300,300,400,400);
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char ** argv) {
    graphicsSetup(argc, argv);
    glutDisplayFunc(drawScene);
    glutMainLoop();
}