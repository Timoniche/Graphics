#include "readerbmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned short ReaderBMP::read_u16(FILE *fp)
{
    unsigned char b0, b1;

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
}

unsigned int ReaderBMP::read_u32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


int ReaderBMP::read_s32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

ReaderBMP::ReaderBMP()
{
    FILE * pFile = fopen("C:/Users/Timoniche/Desktop/Graphics/CubeBresenham/cubeQT/TallGreenGrass.bmp", "rb");

    if (pFile == nullptr)
    { return; }
    // считываем заголовок файла
    header.bfType      = read_u16(pFile);
    header.bfSize      = read_u32(pFile);
    header.bfReserved1 = read_u16(pFile);
    header.bfReserved2 = read_u16(pFile);
    header.bfOffBits   = read_u32(pFile);

    // считываем заголовок изображения
    bmiHeader.biSize          = read_u32(pFile);
    bmiHeader.biWidth         = read_s32(pFile);
    bmiHeader.biHeight        = read_s32(pFile);
    bmiHeader.biPlanes        = read_u16(pFile);
    bmiHeader.biBitCount      = read_u16(pFile);
    bmiHeader.biCompression   = read_u32(pFile);
    bmiHeader.biSizeImage     = read_u32(pFile);
    bmiHeader.biXPelsPerMeter = read_s32(pFile);
    bmiHeader.biYPelsPerMeter = read_s32(pFile);
    bmiHeader.biClrUsed       = read_u32(pFile);
    bmiHeader.biClrImportant  = read_u32(pFile);


    rgb = new RGBQUAD*[bmiHeader.biWidth];
    for (int i = 0; i < bmiHeader.biWidth; i++) {
        rgb[i] = new RGBQUAD[bmiHeader.biHeight];
    }

    //BGR format!
    for (int i = 0; i < bmiHeader.biWidth; i++) {
        for (int j = 0; j < bmiHeader.biHeight; j++) {
            rgb[i][j].rgbBlue = getc(pFile);
            rgb[i][j].rgbGreen = getc(pFile);
            rgb[i][j].rgbRed = getc(pFile);
        }

        // пропускаем последний байт в строке
        getc(pFile);
    }

    // выводим результат
//    std::cout << bmiHeader.biWidth << " " << bmiHeader.biHeight << std::endl;
//    for (int i = 0; i < bmiHeader.biWidth; i++) {
//        for (int j = 0; j < bmiHeader.biHeight; j++) {
//            printf("%d %d %d\n", rgb[i][j].rgbRed, rgb[i][j].rgbGreen, rgb[i][j].rgbBlue);
//        }
//        printf("\n");
//    }

    fclose(pFile);
}

DGL::vec3i ReaderBMP::get_color(int i, int j)
{
    DGL::vec3i ans = DGL::vec3i{rgb[i][j].rgbRed, rgb[i][j].rgbGreen, rgb[i][j].rgbBlue};
    if (ans == DGL::vec3i{-1, -1, -1}) return {0, 0, 0};
    return ans;
}

