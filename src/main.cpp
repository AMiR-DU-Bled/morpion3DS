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
#include <map>
#include <fstream>
#include <malloc.h>
#include <dirent.h>
#include <ctype.h>
//my headers
#include <button.h>
#include <keyboardClass.h>
#include <gros_crane.h>
#include <morpionlogic.h>
#include <loadImage.h>
#include <audiohelp.hpp>
#include <user_music.h>
#include <textsystem.h>

std::map<std::string, std::string> translations;
C2D_TextBuf textBuf = C2D_TextBufNew(4096), fpsbuf = C2D_TextBufNew(4096);
C2D_Text gameNameC2D, startC2D, quitC2D, player1WinC2D, player2WinC2D, player1PlayC2D, player2PlayC2D, drawC2D , fpsconter, replayC2D;
C3D_RenderTarget *bottom, *top;
C2D_SpriteSheet cirlceSprites;
C2D_Sprite circle[5];
C2D_SpriteSheet crossSprites;
C2D_Sprite cross[4];
C2D_Sprite randC2D[9];
u32 kDown;
std::vector<Image3DS> images(2); // reserve space for 2 images
MusicPlayer * bloon_is_peak = nullptr;
int currentSong = 0;
bool playbloonmusic=true;
MusicPlayer* user_music = nullptr;
std::vector<std::string> wavFiles;
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

void interpretinput(short& x, short& y, short cell) {
    if (cell < 0 || cell > 8) {
        x = y = -1; // optional: indicate invalid cell
        return;
    }
    x = cell / 3;
    y = cell % 3;
}

short xytocell(short x, short y) {
    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return -1; // out of bounds
    return x * 3 + y;
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
                    cell = xytocell(i,j);
                    if (cell != drawingcell){
                    C2D_SpriteFromSheet(&circle[circledrawn], cirlceSprites, 5);
                    C2D_SpriteSetPos(&circle[circledrawn],positions[cell][0],positions[cell][1]);       // position in grid   
                    C2D_DrawSprite(&circle[circledrawn]);
                    circledrawn++;
                    }
                }
                if (grid[i][j] ==2){
                    cell = xytocell(i,j);
                    if (cell != drawingcell){
                    C2D_SpriteFromSheet(&cross[crossdrawn], crossSprites, 5);
                    C2D_SpriteSetPos(&cross[crossdrawn],positions[cell][0],positions[cell][1]);       // position in grid   
                    C2D_DrawSprite(&cross[crossdrawn]);
                    crossdrawn++;
                    }
                }
            }
        }
    }
}
void keepdraw(short grid[3][3]){
    short cell;
    short circledrawn=0,crossdrawn=0;
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
            if (grid[i][j] !=0){
                if (grid[i][j] ==1){
                    circledrawn++;
                    cell = xytocell(i,j);
                    C2D_SpriteFromSheet(&circle[circledrawn], cirlceSprites, 5);
                    C2D_SpriteSetPos(&circle[circledrawn],positions[cell][0],positions[cell][1]);       // position in grid   
                    C2D_DrawSprite(&circle[circledrawn]);
                }
                if (grid[i][j] ==2){
                    crossdrawn++;
                    cell = xytocell(i,j);
                    C2D_SpriteFromSheet(&cross[crossdrawn], crossSprites, 5);
                    C2D_SpriteSetPos(&cross[crossdrawn],positions[cell][0],positions[cell][1]);       // position in grid   
                    C2D_DrawSprite(&cross[crossdrawn]);
                }
            }
        }
    }
}
void player1(short grid[3][3],short x,short y){
    short cell=10, circleindex=0,framecount=0,numcircle=0;
    touchPosition touch;
    bool annimation=false;
    while (aptMainLoop()){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        hidTouchRead(&touch);
        C2D_SceneBegin(top);
        C2D_TargetClear(top, C2D_Color32(255,255,255,255)); // clear to white
        C2D_DrawText(&player1PlayC2D, C2D_WithColor, 100, 30, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,255,255)); // red
        C2D_DrawText(&quitC2D, C2D_WithColor, 40, 120, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,0,255)); // red
        if (playbloonmusic)bloon_is_peak->update();
        C2D_SceneBegin(bottom);
        hidScanInput();
        kDown = hidKeysDown();
        if (kDown & KEY_B) {
            if (playbloonmusic) {
                if (bloon_is_peak) {
                    bloon_is_peak->stop();
                    delete bloon_is_peak;
                    bloon_is_peak = nullptr;
                }
                if (user_music) {
                    user_music->stop();
                    delete user_music;
                }
                user_music = new MusicPlayer(wavFiles[currentSong].c_str(), 1, false);
                user_music->play();
                playbloonmusic = false;
            } else {
                if (user_music) {
                    user_music->stop();
                    delete user_music;
                    user_music = nullptr;
                }
                if (bloon_is_peak) {
                    bloon_is_peak->stop();
                    delete bloon_is_peak;
                }
                bloon_is_peak = new MusicPlayer("romfs:/audio/BloonsDiscoParty.wav", 0, true); 
                bloon_is_peak->play();
                playbloonmusic = true;
            }
        }
        C2D_TargetClear(bottom, C2D_Color32(255,255,255,255));
        drawFullScreenGrid(3, 3);
        keepdraw(grid,cell);
		if (kDown&KEY_START){
            C3D_FrameEnd(0);
            C2D_SpriteSheetFree(cirlceSprites);
            C2D_SpriteSheetFree(crossSprites);
            C3D_TexDelete(&images[0].tex);
            C3D_TexDelete(&images[0].tex);
            C2D_Fini();
            C3D_Fini();
            ndspExit();
            gfxExit();
            exit(0);
        }
        if ((touch.px!=0||touch.py!=0 )&& annimation==false){
        cell=checkinputtouch(touch);
        interpretinput(x,y,cell);
        if (grid[x][y]==0 && x<3 && y<3){
            grid[x][y]=1;
            numcircle = assignnumbertobject(grid,false);
            annimation=true;
            C3D_FrameEnd(0);
        }
        else cell=10;
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
    short cell=10, crossindex=0,framecount=0,numcross=0;;
    touchPosition touch;
    bool annimation=false;
    while (aptMainLoop()){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        hidTouchRead(&touch);
        C2D_SceneBegin(top);
        C2D_TargetClear(top, C2D_Color32(255,255,255,255)); // clear to white
        C2D_DrawText(&player2PlayC2D, C2D_WithColor, 100, 30, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,255,255)); // red
        C2D_DrawText(&quitC2D, C2D_WithColor, 40, 120, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,0,255)); // red
        C2D_SceneBegin(bottom);
        if (playbloonmusic)bloon_is_peak->update();
        else updateUserMusic(user_music,wavFiles, currentSong);
        hidScanInput();
        kDown = hidKeysDown();
        if (kDown & KEY_B) {
            if (playbloonmusic) {
                if (bloon_is_peak) {
                    bloon_is_peak->stop();
                    delete bloon_is_peak;
                    bloon_is_peak = nullptr;
                }
                if (user_music) {
                    user_music->stop();
                    delete user_music;
                }
                user_music = new MusicPlayer(wavFiles[currentSong].c_str(), 1, false);
                user_music->play();
                playbloonmusic = false;
            } else {
                if (user_music) {
                    user_music->stop();
                    delete user_music;
                    user_music = nullptr;
                }
                if (bloon_is_peak) {
                    bloon_is_peak->stop();
                    delete bloon_is_peak;
                }
                bloon_is_peak = new MusicPlayer("romfs:/audio/BloonsDiscoParty.wav", 0, true); 
                bloon_is_peak->play();
                playbloonmusic = true;
            }
        }
        C2D_TargetClear(bottom, C2D_Color32(255,255,255,255));
        drawFullScreenGrid(3, 3);
        keepdraw(grid,cell);
		if (kDown&KEY_START){
            C3D_FrameEnd(0);
            C2D_SpriteSheetFree(cirlceSprites);
            C2D_SpriteSheetFree(crossSprites);
            C3D_TexDelete(&images[0].tex);
            C3D_TexDelete(&images[0].tex);
            C2D_Fini();
            C3D_Fini();
            ndspExit();
            gfxExit();
            romfsExit();
            exit(0);
        }
        if ((touch.px!=0||touch.py!=0 ) && annimation==false){
            cell=checkinputtouch(touch);
            interpretinput(x,y,cell);
            if (grid[x][y]==0 && x<3 && y<3){
                grid[x][y]=2;
                numcross = assignnumbertobject(grid,true);
                annimation=true;
                C3D_FrameEnd(0);
            }
            else cell=10;
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
    bool test =false, replay = false;
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
    ndspInit();
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    fsInit();
    Result rc = romfsInit();
    if (R_FAILED(rc)) {
        printf("romfsInit failed: 0x%08lX\n", rc);
        svcSleepThread(10*1000000000LL);
        return -1;
    }
    const char* musicPath = "sdmc:/3ds/morpion3ds/music";
    //PrintConsole top;
	//consoleInit(GFX_TOP, &top);
	//consoleSelect(&top);
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    u8 lang = CFG_LANGUAGE_EN;    // default
    CFGU_GetSystemLanguage(&lang);
    std::string langFile = "romfs:/text/en.txt"; // default
    if (lang == CFG_LANGUAGE_FR){ langFile = "romfs:/text/fr.txt"; test = true; }
    else if (lang == CFG_LANGUAGE_EN) langFile = "romfs:/text/en.txt";
   // langFile = "romfs:/text/fr.txt";
    // ... add more languages if needed
    loadLanguage(langFile);
    
    const char* gameNameText = tr("GAME_NAME");
    const char* pressStartText = tr("GAME_START");
    const char* quitText = tr("QUIT");
    const char * player1Play = tr("PLAYER1_PLAY");
    const char * player2Play = tr("PLAYER2_PLAY");
    const char * player1Win = tr("PLAYER1_WIN");
    const char * player2Win = tr("PLAYER2_WIN");
    const char * draw = tr("DRAW");
    const char * replayText = tr("REPLAY");
    //prepare text pour affichage
    C2D_TextParse(&gameNameC2D, textBuf, gameNameText);
    C2D_TextParse(&startC2D, textBuf, pressStartText);
    C2D_TextParse(&quitC2D, textBuf, quitText);
    C2D_TextParse(&player1PlayC2D, textBuf, player1Play);
    C2D_TextParse(&player2PlayC2D, textBuf, player2Play);
    C2D_TextParse(&player1WinC2D, textBuf, player1Win);
    C2D_TextParse(&player2WinC2D, textBuf, player2Win);
    C2D_TextParse(&drawC2D, textBuf, draw);
    C2D_TextParse(&replayC2D, textBuf, replayText);
    C2D_TextOptimize(&gameNameC2D);
    C2D_TextOptimize(&drawC2D);
    C2D_TextOptimize(&startC2D);
    C2D_TextOptimize(&quitC2D);
    C2D_TextOptimize(&player1PlayC2D);
    C2D_TextOptimize(&player2PlayC2D);
    C2D_TextOptimize(&player1WinC2D);
    C2D_TextOptimize(&player2WinC2D);
    C2D_TextOptimize(&replayC2D);
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
    bloon_is_peak = new MusicPlayer("romfs:/audio/BloonsDiscoParty.wav", 0, true);
    bloon_is_peak->setVolume(1.0f,1.0f);
    ensureMusicFolderExists(musicPath);
    listWavFiles(musicPath, wavFiles);  
    if (!wavFiles.empty()) {
        user_music = new MusicPlayer(wavFiles[currentSong].c_str(), 1, false); // channel 1
    }
	short grid[3][3];
	short y=4,x=4,winner=0;
    float sizetitle=0.0f;
    bool turn=false;//false mean p1 true mean p2
    bool game_end=false, titlesmalling = true;
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
            grid[i][j]=0;
        }
    }
    u64 lastTime = osGetTime();   // time in milliseconds since app start
    int frames = 0;               // how many frames have passed
    float fps = 0.0f;             // result to display
    srand((unsigned)(osGetTime() & 0xFFFFFFFF));
    short randindex[9];
    bool randspriteidex[9];
    for (short i=0; i <9; i++){
        if (rand()%2) {
            randindex[i] = rand()%6;
            randspriteidex[i] = true;
        }
        else {
            randindex[i] = rand()%6;
            randspriteidex[i] = false;
        }
    }
    short framecount = 0;
    if (playbloonmusic)bloon_is_peak->play();
    while (aptMainLoop()){
        if (playbloonmusic)bloon_is_peak->update();
        else updateUserMusic(user_music,wavFiles, currentSong);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);   // begin frame
        C2D_SceneBegin(bottom);
        C2D_TargetClear(bottom,C2D_Color32(255,255,255,255));
        drawFullScreenGrid(3,3);
        for (short i=0; i<9;i++) {
            C2D_SpriteFromSheet(&randC2D[i],randspriteidex[i] ? cirlceSprites : crossSprites, randindex[i]);
            C2D_SpriteSetPos(&randC2D[i],positions[i][0],positions[i][1]);       // position in grid  
            C2D_DrawSprite(&randC2D[i]);
        }
        C2D_SceneBegin(top);
        C2D_TargetClear(top,C2D_Color32(255,255,255,255));
        C2D_DrawText(&startC2D, C2D_WithColor, 0, 20, 0.5f, sizetitle, sizetitle , C2D_Color32(0,0,255,255)); // blue
        C2D_DrawText(&gameNameC2D, C2D_WithColor, 200, 80, 0.5f, 1.0f, 1.0f, C2D_Color32(0,0,255,255));
        char fpsString[32];
        snprintf(fpsString, sizeof(fpsString), "FPS: %.2f", fps);
        C2D_TextBufClear(fpsbuf);
        C2D_TextParse(&fpsconter, fpsbuf, fpsString);
        C2D_TextOptimize(&fpsconter);
        C2D_DrawText(&fpsconter, C2D_WithColor, 100, 200, 0.0f, 0.6f, 0.6f, C2D_Color32(255,150,0,255));
        if (sizetitle <= 1.28f && titlesmalling){
            sizetitle += 0.01f;
            titlesmalling = true;
        }
        else { sizetitle -= 0.01f; titlesmalling = false;}
        if (sizetitle <= 0.5f) titlesmalling = true;
        framecount++;
        if (framecount >= 10) {
            framecount = 0;
            for (short i =0 ; i<9; i++) randindex[i]++;
        }
        for (short i =0 ; i<9; i++) if (randindex[i]>6)randindex[i]=0;
        hidScanInput();
		kDown = hidKeysDown();
        if (!(kDown & KEY_START) && !(kDown & KEY_TOUCH) && kDown != 0)break;
        if (kDown&KEY_START){
            C2D_SpriteSheetFree(cirlceSprites);
            C2D_SpriteSheetFree(crossSprites);
            C3D_TexDelete(&images[0].tex);
            C3D_TexDelete(&images[0].tex);
            C2D_Fini();
            C3D_Fini();
            ndspExit();
            gfxExit();
            romfsExit();
            exit(0);
        }
        C3D_FrameEnd(0);
        frames++;                 // count one more frame
        // calculate every 1000 ms (1 second)
        u64 currentTime = osGetTime();
        if (currentTime - lastTime >= 1000) {
            fps = frames * 1000.0f / (currentTime - lastTime);
            frames = 0;                  // reset counter
            lastTime = currentTime;      // reset timer
        }     
    }
    do{
        while (aptMainLoop()&&!game_end){
            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);   // begin frame
            C2D_SceneBegin(top);
            C2D_TargetClear(top, C2D_Color32(255,255,255,255)); // clear to white
            C2D_DrawText(&quitC2D, C2D_WithColor, 40, 120, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,0,255)); // red
            if (playbloonmusic)bloon_is_peak->update();
            else updateUserMusic(user_music,wavFiles, currentSong);
            if (turn==false){
                C2D_DrawText(&player1PlayC2D, C2D_WithColor, 100, 30, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,255,255));
            }
            else {
                C2D_DrawText(&player2PlayC2D, C2D_WithColor, 100, 30, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,255,255)); 
            }
            hidScanInput();
            kDown = hidKeysDown();
            if (kDown&KEY_START){
                C2D_SpriteSheetFree(cirlceSprites);
                C2D_SpriteSheetFree(crossSprites);
                C3D_TexDelete(&images[0].tex);
                C3D_TexDelete(&images[0].tex);
                C2D_Fini();
                C3D_Fini();
                ndspExit();
                gfxExit();
                romfsExit();
                exit(0);
            }
            if (kDown & KEY_B) {
                if (playbloonmusic) {
                    if (bloon_is_peak) {
                        bloon_is_peak->stop();
                        delete bloon_is_peak;
                        bloon_is_peak = nullptr;
                    }
                    if (user_music) {
                        user_music->stop();
                        delete user_music;
                    }
                    user_music = new MusicPlayer(wavFiles[currentSong].c_str(), 1, false);
                    user_music->play();
                    playbloonmusic = false;
                } else {
                    if (user_music) {
                        user_music->stop();
                        delete user_music;
                        user_music = nullptr;
                    }
                    if (bloon_is_peak) {
                        bloon_is_peak->stop();
                        delete bloon_is_peak;
                    }
                    bloon_is_peak = new MusicPlayer("romfs:/audio/BloonsDiscoParty.wav", 0, true); 
                    bloon_is_peak->play();
                    playbloonmusic = true;
                }
            }
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom,C2D_Color32(255,255,255,255));
            drawFullScreenGrid(3,3);
            keepdraw(grid);
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
            winner=verify(grid);
            if (!winner==0){
            game_end=true;
            }
            C3D_FrameEnd(0); 
        }
        short circleindex=0,crossindex=0;
        framecount=0;
        turn = false;
        while (aptMainLoop()){
            hidScanInput();
            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_SceneBegin(top);
            C2D_TargetClear(top, C2D_Color32(255,255,255,255)); // clear to white
            if (bloon_is_peak)bloon_is_peak->update();
            else updateUserMusic(user_music,wavFiles, currentSong);
            kDown = hidKeysDown();
            if (kDown & KEY_B) {
                if (playbloonmusic) {
                    if (bloon_is_peak) {
                        bloon_is_peak->stop();
                        delete bloon_is_peak;
                        bloon_is_peak = nullptr;
                    }
                    if (user_music) {
                        user_music->stop();
                        delete user_music;
                    }
                    user_music = new MusicPlayer(wavFiles[currentSong].c_str(), 1, false);
                    user_music->play();
                    playbloonmusic = false;
                } else {
                    if (user_music) {
                        user_music->stop();
                        delete user_music;
                        user_music = nullptr;
                    }
                    if (bloon_is_peak) {
                        bloon_is_peak->stop();
                        delete bloon_is_peak;
                    }
                    bloon_is_peak = new MusicPlayer("romfs:/audio/BloonsDiscoParty.wav", 0, true); 
                    bloon_is_peak->play();
                    playbloonmusic = true;
                }
            }
            if (test==false)C2D_DrawText(&quitC2D, C2D_WithColor, 150, 60, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,0,255)); // red
            else C2D_DrawText(&quitC2D, C2D_WithColor, 40, 0, 0.5f, 1.0f, 1.0f, C2D_Color32(255,255,0,255));
            if (winner==3){
                C2D_DrawText(&drawC2D, C2D_WithColor, 100, 0, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,0,255)); // red
            }
            if (winner == 1){
                C2D_DrawText(&player1WinC2D, C2D_WithColor, 100, 0, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,0,255)); // red
            }
            if (winner == 2){
                C2D_DrawText(&player2WinC2D, C2D_WithColor, 100, 0, 0.5f, 1.0f, 1.0f, C2D_Color32(255,0,0,255)); // red
            }
            C2D_DrawText(&replayC2D, C2D_WithColor, 15, 135, 0.5f, 0.75f, 0.75f);
            char fpsString[32];
            snprintf(fpsString, sizeof(fpsString), "FPS: %.2f", fps);
            C2D_TextBufClear(fpsbuf);
            C2D_TextParse(&fpsconter, fpsbuf, fpsString);
            C2D_TextOptimize(&fpsconter);
            C2D_DrawText(&fpsconter, C2D_WithColor, 100, 200, 0.0f, 0.6f, 0.6f, C2D_Color32(0,0,0,255));
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(255,255,120,255)); // clear
            bloon_is_peak->update();
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
            framecount++;
            if (framecount >= 10) {
                framecount = 0;
                circleindex++;
                crossindex++;
            }
            if (circleindex>6)circleindex=0;
            if (crossindex>6)crossindex=0;
            if (kDown&KEY_START) { replay = false; break; }
            if (kDown & KEY_SELECT) {replay = true; break; }
            C3D_FrameEnd(0); 
            frames++;                 // count one more frame
            // calculate every 1000 ms (1 second)
            u64 currentTime = osGetTime();
            if (currentTime - lastTime >= 1000) {
                fps = frames * 1000.0f / (currentTime - lastTime);
                frames = 0;                  // reset counter
                lastTime = currentTime;      // reset timer
            }
        }
        game_end = false;
        for (short i=0;i<3;i++){
            for (short j=0;j<3;j++){
                grid[i][j]=0;
            }
        }
    }  while (replay);
    //delete bloon_is_peak;
    C2D_TextBufDelete(textBuf);
    C2D_TextBufDelete(fpsbuf);
    C2D_SpriteSheetFree(cirlceSprites);
    C2D_SpriteSheetFree(crossSprites);
    C3D_TexDelete(&images[0].tex);
    C3D_TexDelete(&images[0].tex);
	C2D_Fini();
    C3D_Fini();
    ndspExit();
	gfxExit();
    romfsExit();
	return 0;
}