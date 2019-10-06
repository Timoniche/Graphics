#ifndef READERBMP_H
#define READERBMP_H

#include <iostream>
#include "dglgeometry.h"

class ReaderBMP
{
public:
    ReaderBMP();
    typedef struct
    {
        unsigned int    bfType;
        unsigned long   bfSize;
        unsigned int    bfReserved1;
        unsigned int    bfReserved2;
        unsigned long   bfOffBits;
    } BITMAPFILEHEADER;

    typedef struct
    {
        unsigned int    biSize;
        int             biWidth;
        int             biHeight;
        unsigned short  biPlanes;
        unsigned short  biBitCount;
        unsigned int    biCompression;
        unsigned int    biSizeImage;
        int             biXPelsPerMeter;
        int             biYPelsPerMeter;
        unsigned int    biClrUsed;
        unsigned int    biClrImportant;
    } BITMAPINFOHEADER;

    typedef struct
    {
        int   rgbBlue;
        int   rgbGreen;
        int   rgbRed;
        int   rgbReserved;
    } RGBQUAD;

    BITMAPINFOHEADER bmiHeader;
    //BITMAPFILEHEADER header __attribute__((unused));
    BITMAPFILEHEADER header;
    RGBQUAD **rgb;
    static unsigned short read_u16(FILE *fp);
    static unsigned int   read_u32(FILE *fp);
    static int            read_s32(FILE *fp);
    DGL::vec3i get_color(int i, int j);
};

#endif // READERBMP_H
