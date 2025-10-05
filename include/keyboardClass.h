#ifndef KEYBOARD_CLASS_H
#define KEYBOARD_CLASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <3ds.h>
#include <citro2d.h>
class Keyboard{
    private : 
    SwkbdState swkbd;  // The keyboard itself
    public :
    char mybuf [512]; // Buffer to store the text the user types
    void initkeyboard(SwkbdType type, int numButtons, int maxTextLength);
    void setinitialtext(const char* initialtext);
    void sethinttext(const char* hinttext);
    bool showkeyboard();
};
#endif