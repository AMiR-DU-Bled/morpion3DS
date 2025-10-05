#include <button.h>
#include <3ds.h>
#include <citro2d.h>
    short checkinputtouch(touchPosition touch){
        short cellWidth  = 320 / 3; // 106
        short cellHeight = 240 / 3; // 80

        short col, row;

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