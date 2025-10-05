#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <3ds.h>
#include <citro2d.h>
#include <button.h>
#include <keyboardClass.h>
void Keyboard::initkeyboard( SwkbdType type, int numButtons, int maxTextLength){
    swkbdInit(&swkbd, type, numButtons, maxTextLength);  // Normal keyboard
}
void Keyboard::setinitialtext(const char* initialtext){
    swkbdSetInitialText(&swkbd,initialtext);          // Optional: text already in box
}
void Keyboard::sethinttext(const char* hinttext){
    swkbdSetHintText(&swkbd,hinttext);          // Optional: hint text
}
bool Keyboard::showkeyboard(){
   return swkbdInputText(&swkbd, mybuf, sizeof(mybuf)); 
}