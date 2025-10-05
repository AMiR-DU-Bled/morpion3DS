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
#include <button.h>
#include <keyboardClass.h>
#include <gros_crane.h>
#include <morpionlogic.h>
#include <loadImage.h>
C3D_RenderTarget* bottom;
C2D_SpriteSheet cirlceSprites;
C2D_Sprite circle[5];
C2D_SpriteSheet crossSprites;
C2D_Sprite cross[4];
float positions[9][2] = {
    {21.33f, 8.0f},    {128.0f, 8.0f},    {234.66f, 8.0f},
    {21.33f, 88.0f},   {128.0f, 88.0f},   {234.66f, 88.0f},
    {21.33f, 168.0f},  {128.0f, 168.0f},  {234.66f, 168.0f}
    };
void drawFullScreenGrid(short rows, short cols) {
    short screenWidth = 320;
    short screenHeight = 240;

    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;

    for (short i = 1; i < cols; i++) {
        float x = i * cellWidth;
        C2D_DrawRectSolid(x, 0, 0.0f, 4, screenHeight, C2D_Color32(0,0,0,255));
    }
    for (short i = 1; i < rows; i++) {
        float y = i * cellHeight;
        C2D_DrawRectSolid(0, y, 0.0f, screenWidth, 4, C2D_Color32(0,0,0,255));
    }
}

void clearscreen(){
    std::cout << "\033[6;1H";
    std::cout<<"\x1b[0J";
}
void clearendscreen(){
    std::cout << "\033[12;1H";
    std::cout<<"\x1b[0J";
}


void interpretinput(short& x,short& y,short cell){
    switch (cell)
    {
    case 0:
        x=0;
        y=0;
    break;
    case 1:
        x=0;
        y=1;
    break;
    case 2:
        x=0;
        y=2;
    break;
    case 3:
        x=1;
        y=0;
    break;
    case 4:
        x=1;
        y=1;
    break;
    case 5:
        x=1;
        y=2;
    break;
    case 6:
        x=2;
        y=0;
    break;
    case 7:
        x=2;
        y=1;
    break;
    case 8:
        x=2;
        y=2;
    break;
    
    default:
        break;
    }
}
short xytocell(short x,short y,short grid){
    switch (x)
    {
    case 0:
        switch (y)
        {
        case 0:
            return 0;
            break;
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        }
        break;
    case 1:
        switch (y)
        {
        case 0:
            return 3;
            break;
        case 1:
            return 4;
            break;
        case 2:
            return 5;
            break;
        }
        break;
    case 2:
        switch (y)
        {
        case 0:
            return 6;
            break;
        case 1:
            return 7;
            break;
        case 2:
            return 8;
            break;
        }
        break;
    }
    return -1;
}
short assignnumbertobject(short grid[3][3],bool player/*false=p1 true=p2*/){
    short numobj=0;
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
            if (grid[i][j] != 0){
                if (grid[i][j] == 1){
                    if (player==false){
                        numobj++;
                    }
                }
                else if (grid[i][j] == 2){
                    if (player==true){
                        numobj++;
                    }
                }
            }
        }
    }
    return numobj-1;
}
void keepdraw(short grid[3][3],short drawingcell){
    short cell;
    short circledrawn=0,crossdrawn=0;
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
            if (grid[i][j] !=0){
                if (grid[i][j] ==1){
                    circledrawn++;
                    cell = xytocell(i,j,cell);
                    if (cell != drawingcell){
                    C2D_SpriteFromSheet(&circle[circledrawn], cirlceSprites, 5);
                    C2D_SpriteSetPos(&circle[circledrawn],positions[cell][0],positions[cell][1]);       // position in grid   
                    C2D_DrawSprite(&circle[circledrawn]);
                    }
                }
                if (grid[i][j] ==2){
                    crossdrawn++;
                    cell = xytocell(i,j,cell);
                    if (cell != drawingcell){
                    C2D_SpriteFromSheet(&cross[crossdrawn], crossSprites, 5);
                    C2D_SpriteSetPos(&cross[crossdrawn],positions[cell][0],positions[cell][1]);       // position in grid   
                    C2D_DrawSprite(&cross[crossdrawn]);
                    }
                }
            }
        }
    }
}
void player1(short grid[3][3],short x,short y){
    short cell=10,toomucherror=0, circleindex=0,framecount=0,numcircle=0;
    touchPosition touch;
    bool showonscreen=false, annimation=false;
    while (aptMainLoop()){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        hidTouchRead(&touch);
        C2D_SceneBegin(bottom);
        hidScanInput();
        u32 kDown = hidKeysDown();
        C2D_TargetClear(bottom, C2D_Color32(255,255,255,255));
        drawFullScreenGrid(3, 3);
        keepdraw(grid,cell);
		if (kDown&KEY_START){
            C2D_Fini();
            C3D_Fini();
            gfxExit();
            exit(0);
        }
        if (showonscreen==false)std::cout<<"please touch were you want to put the cross\n";
        showonscreen=true;
        if ((touch.px!=0||touch.py!=0 )&& annimation==false){
        cell=checkinputtouch(touch);
        interpretinput(x,y,cell);
        if (grid[x][y]==0 && x<3 && y<3){
            grid[x][y]=1;
            numcircle = assignnumbertobject(grid,false);
            annimation=true;
            C3D_FrameEnd(0);
        }
        else{
            if (toomucherror==3){
                std::cout << "\033[14;0H";
                std::cout<<"\x1b[0J";
                toomucherror=0;
            } 
            std::cout<<"invalid input\nlocation pointed to already used or does not exist\n" ;
            toomucherror++;
        }
        }
        if (annimation==true){
            C2D_SpriteFromSheet(&circle[numcircle], cirlceSprites, circleindex);
            C2D_SpriteSetPos(&circle[numcircle],positions[cell][0],positions[cell][1]);       // position in grid   
            C2D_DrawSprite(&circle[numcircle]);
            framecount++;
            if (framecount >= 10) {
                framecount = 0;
                circleindex++;
            }
            if (circleindex>5)break;
        }
        C3D_FrameEnd(0);
    }
}
void player2(short grid[3][3],short x,short y){
    short cell=10,toomucherror=0, crossindex=0,framecount=0,numcross=0;;
    touchPosition touch;
    bool showonscreen=false, annimation=false;
    while (aptMainLoop()){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        hidTouchRead(&touch);
        C2D_SceneBegin(bottom);
        hidScanInput();
        u32 kDown = hidKeysDown();
        C2D_TargetClear(bottom, C2D_Color32(255,255,255,255));
        drawFullScreenGrid(3, 3);
        keepdraw(grid,cell);
		if (kDown&KEY_START){
            C2D_Fini();
            C3D_Fini();
            gfxExit();
            exit(0);
        }
        if (showonscreen==false)std::cout<<"please touch were you want to put the circle\n";
        showonscreen=true;
        if ((touch.px!=0||touch.py!=0 ) && annimation==false){
            cell=checkinputtouch(touch);
            interpretinput(x,y,cell);
            if (grid[x][y]==0 && x<3 && y<3){
                grid[x][y]=2;
                numcross = assignnumbertobject(grid,true);
                annimation=true;
                C3D_FrameEnd(0);
            }
            else{
                if (toomucherror==3){
                std::cout << "\033[14;0H";
                std::cout<<"\x1b[0J";
                toomucherror=0;
            } 
                std::cout<<"invalid input\nlocation pointed to already used or does not exist\n" ;
                toomucherror++;
            }
        }
        if (annimation==true){
            C2D_SpriteFromSheet(&cross[numcross], crossSprites, crossindex);
            C2D_SpriteSetPos(&cross[numcross],positions[cell][0],positions[cell][1]);       // position in grid   
            C2D_DrawSprite(&cross[numcross]);
            framecount++;
            if (framecount >= 10) {
                framecount = 0;
                crossindex++;
            }
            if (crossindex>5)break;
        }
        C3D_FrameEnd(0);
    }
}
int main(){
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	PrintConsole top;
	consoleInit(GFX_TOP, &top);
	consoleSelect(&top);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    Result rc = romfsInit();
    if (R_FAILED(rc)) {
        printf("romfsInit failed: 0x%08lX\n", rc);
        svcSleepThread(10*1000000000LL);
        return -1;
    }
    cirlceSprites = C2D_SpriteSheetLoad("romfs:/images/circle_spritesheet.t3x");
    if (!cirlceSprites) {
        std::cout << "Failed to load circle sprite sheet!\n";
        svcSleepThread(10*1000000000LL);
    }
    crossSprites = C2D_SpriteSheetLoad("romfs:/images/cross_spritesheet.t3x");
    if (!crossSprites) {
        std::cout << "Failed to load cross sprite sheet!\n";
        svcSleepThread(10*1000000000LL);
    }
    std::vector<Image3DS> images(2); // reserve space for 2 images
    if (!LoadT3X("romfs:/images/gros_crane.t3x", images[0])) {
    printf("Failed to load gros_crane.t3x\n");
    svcSleepThread(10*1000000000LL);
    return 1;
    }
    if (!LoadT3X("romfs:/images/a.t3x", images[1])) {
    printf("Failed to load a.t3x\n");
    svcSleepThread(10*1000000000LL);
    return 1;
    }
	short grid[3][3];
	short y=4,x=4,winner=0;
    bool turn=false;//false mean p1 true mean p2
    bool game_end=false, quitmessage=false;
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
            grid[i][j]=0;
        }
    }
	std::cout<<"tick tac toe game!\n"<<"0 stands for empty, 1 for player 1(aka cross), and\n2 for player 2(aka circle)\n";
    std::cout<<"please touch the bottom screen in order to play\n"
    "REMEMBER : rows and colums start with 0\n";
	while (aptMainLoop()&&!game_end)
	{
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);   // begin frame
        //gfxSwapBuffers();
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown&KEY_START)break;
        C2D_SceneBegin(bottom);
        showgrid(grid);
        if (turn==false){
            C3D_FrameEnd(0);
            player1(grid,x,y);
            turn=true;
        }
        else {
            C3D_FrameEnd(0);
            player2(grid,x,y);
            turn=false;
        }
        showgrid(grid);
        winner=verify(grid);
        if (!winner==0){
        game_end=true;
        clearendscreen();
        }
        else {
        clearscreen();
        }
        C3D_FrameEnd(0); 
    }
    if (winner==3){
        std::cout<<"Its a draw !\n";
    }
    else{
    std::cout<<"Player "<<winner<<" has won !\n";
    }
    short circleindex=0,crossindex=0,framecount=0;
    while (aptMainLoop()){
        hidScanInput();
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_SceneBegin(bottom);
        C2D_TargetClear(bottom, C2D_Color32(255,255,120,255)); // clear to white
        for (short i=0;i<5;i++){
            C2D_SpriteFromSheet(&circle[i], cirlceSprites, circleindex);
            C2D_SpriteSetPos(&circle[i],positions[i][0],positions[i][1]);       // position in grid   
            C2D_DrawSprite(&circle[i]);
        }
        for (short i=0;i<4;i++){
            C2D_SpriteFromSheet(&cross[i], crossSprites, crossindex);
            C2D_SpriteSetPos(&cross[i],positions[i+5][0],positions[i+5][1]);       // position in grid   
            C2D_DrawSprite(&cross[i]);
        }
        drawFullScreenGrid(3,3);
        u32 kDown = hidKeysDown();
        framecount++;
        if (framecount >= 10) {
            framecount = 0;
            circleindex++;
            crossindex++;
        }
        if (circleindex>6)circleindex=0;
        if (crossindex>6)crossindex=0;
		if (kDown&KEY_START)break;
        if (quitmessage==false)std::cout<<"\nPlease press start to quit.\n";
        quitmessage=true;
        C3D_FrameEnd(0); 
    }
	C2D_Fini();
    C3D_Fini();
	gfxExit();
	return 0;
}