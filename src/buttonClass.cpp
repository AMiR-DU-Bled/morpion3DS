#include <buttonClass.h>
#include <3ds.h>
#include <citro2d.h>
    short checkinputtouch(touchPosition touch){
        int cellWidth  = 320 / 3; // 106
        int cellHeight = 240 / 3; // 80

        int col, row;

        // Handle X (remainder goes to last column)
        if (touch.px < 2 * cellWidth)
            col = touch.px / cellWidth;
        else
            col = 2;

        // Handle Y (perfectly divisible, no remainder problem)
        row = touch.py / cellHeight;

        // Convert (row,col) into a single number 0–8
        return row * 3 + col;

    }