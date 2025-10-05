#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <3ds.h>
#include <citro2d.h>
#include <loadimage.h>

bool LoadT3X(const char* path, Image3DS &outImage) {
    FILE* f = fopen(path, "rb");
    if (!f) {
        printf("Failed to open %s\n", path);
        return false;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    u8* buffer = (u8*)malloc(size);
    if (!buffer) {
        fclose(f);
        printf("Memory allocation failed\n");
        return false;
    }

    fread(buffer, 1, size, f);
    fclose(f);

    Tex3DS_Texture t3xTex = Tex3DS_TextureImport(buffer, size, &outImage.tex, NULL, false);
    free(buffer);

    if (!t3xTex) {
        printf("Texture import failed: %s\n", path);
        return false;
    }

    // Setup full subtexture with 90° clockwise rotation
    outImage.subtex.left   = 0.0f;
    outImage.subtex.top    = 1.0f;  // flip top/bottom
    outImage.subtex.right  = 1.0f;
    outImage.subtex.bottom = 0.0f;

    outImage.subtex.width  = outImage.tex.width;
    outImage.subtex.height = outImage.tex.height;

    outImage.img = { &outImage.tex, &outImage.subtex };

    return true;
}


/*  load image from array
    C3D_Tex tex;
    Tex3DS_SubTexture subtex;
    Tex3DS_Texture t3xTex = Tex3DS_TextureImport(gros_crane_t3x, gros_crane_t3x_len, &tex, NULL, false);
    if (!t3xTex) {
        // handle error
    }
    subtex.left = 0.0f;
    subtex.top = 0.0f;
    subtex.right = 1.0f;
    subtex.bottom = 1.0f;
    subtex.width = tex.width;
    subtex.height = tex.height;
    C2D_Image img = { &tex, &subtex };
    float ximg = 20, yimg = 20;
    float w = 231, h = 295; // your original image size
    C2D_DrawParams params = {
        {ximg, yimg, w, h}, // pos
        {0, 0},
        0,
        0
    };*/