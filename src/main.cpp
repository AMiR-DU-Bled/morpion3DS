#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <3ds.h>
#include <citro2d.h>
#include <buttonClass.h>
#include <keyboardClass.h>
int main(){
	gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, true);  // top and bottom both BGR8, with double buffering
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	Keyboard kb;
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	//PrintConsole bottomScreen;
	//consoleInit(GFX_TOP, &bottomScreen);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	kb.initkeyboard(SWKBD_TYPE_NORMAL, 3, -1);  // Normal keyboard
	kb.sethinttext("please enter color"); // Hint shown on top
    kb.mybuf[0]='\0';
	C2D_TextBuf g_textBuf,errormsgbuff,errorconvbuff;
	C2D_Text g_text, errormsg,errorconv;
	g_textBuf = C2D_TextBufNew(4096);
	errormsgbuff = C2D_TextBufNew(4096);
	errorconvbuff = C2D_TextBufNew(4096);
	//C2D_Font font = C2D_FontLoad("romfs:/gfx/font.ttf");
	C2D_TextParse(&g_text, g_textBuf, "please input the color that you want to use for the top screen in RGBA8 format\nand seperate the numbers with ,\npress A to continue");
	C2D_TextParse(&errormsg, errormsgbuff,"invalid input please type it in right format");
	std::vector<int> rgba;
	short /*cellselected=10 ,*/ circleposy = 0,circleposx =0;
	std::string errorMsgString = ""; // dynamic error message
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(bottom, C2D_Color32(255,255,255,255)); // clear to white
        C2D_TargetClear(top, C2D_Color32(255,255,255,255));
        C2D_SceneBegin(top);

        // Draw instructions
        C2D_DrawText(&g_text, C2D_WithColor, 10, 50, 0.0f, 0.3f, 0.3f, C2D_Color32(0,0,0,255));

        // Draw error message if exists
        if (!errorMsgString.empty()) {
            C2D_TextParse(&errorconv, errorconvbuff, errorMsgString.c_str());
            C2D_TextOptimize(&errorconv);
            C2D_DrawText(&errorconv, C2D_WithColor, 50, 150, 0.0f, 0.3f, 0.3f, C2D_Color32(255,0,0,255));
        }

        if (kDown & KEY_A) {
            bool keyboardrgba = kb.showkeyboard(); // blocks until input
            if (keyboardrgba) {
                std::stringstream ss(kb.mybuf);
                std::string part;
                std::vector<int> tempRgba;
                bool valid = true;

                while (std::getline(ss, part, ',')) {
                    try {
                        int value = std::stoi(part);
                        if (value < 0) value = 0;
                        if (value > 255) value = 255;
                        tempRgba.push_back(value);
                    } catch (...) {
                        valid = false;
                        errorMsgString = "Invalid input! Not a number: " + part;
                        break; // stop parsing
                    }
                }

                if (valid && tempRgba.size() == 4) {
                    rgba = tempRgba;    // assign only if fully valid
                    errorMsgString = ""; // clear error
                    break; // exit input loop
                } else if (valid && tempRgba.size() != 4) {
                    valid = false;
                    errorMsgString = "Invalid input! Need 4 numbers.";
                }
            } else {
                errorMsgString = "Keyboard cancelled!";
            }
        }

        if (kDown & KEY_START) return 0; // exit program
        C3D_FrameEnd(0);
    }

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);   // begin frame
        C2D_TargetClear(top, C2D_Color32(rgba[0],rgba[1],rgba[2],rgba[3])); // clear to color chosen
        C2D_SceneBegin(top);
		C2D_TargetClear(bottom, C2D_Color32(255,255,255,255)); // clear to white
        C2D_SceneBegin(bottom);
		u32 kDown = hidKeysDown();
		if (kDown&KEY_START)break;
		touchPosition touch;
		hidTouchRead(&touch);
		if (touch.px!=0 ||touch.py!=0){
			circleposx=touch.px-25;
			circleposy=touch.py-25;
			C2D_DrawEllipseSolid(circleposx, circleposy, 0.0f, 50, 50, C2D_Color32(rgba[0],rgba[1],rgba[2],rgba[3]));	

    	}
		C3D_FrameEnd(0); 
	}
	C2D_TextBufDelete(g_textBuf);
	C2D_TextBufDelete(errormsgbuff);
	C2D_TextBufDelete(errorconvbuff);
	C2D_Fini();
    C3D_Fini();
	gfxExit();
	return 0;
}

/*
touchPosition touch;
hidTouchRead(&touch);
if (touch.px!=0 ||touch.py!=0){
cellselected=checkinputtouch(touch);
std::cout<<"you pressed cell : "<<cellselected<<'\n';
svcSleepThread(1000000000LL);//en nanosecondes
}*/