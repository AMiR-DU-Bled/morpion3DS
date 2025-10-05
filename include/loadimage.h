#ifndef LOADIMAGE_H
#define LOADIMAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <3ds.h>
#include <citro2d.h>

struct Image3DS {
    C3D_Tex tex;            // GPU texture
    Tex3DS_SubTexture subtex; // Subtexture info
    C2D_Image img;           // Citro2D image
};
bool LoadT3X(const char* path, Image3DS &outImage);
#endif