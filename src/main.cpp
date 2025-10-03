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
#include <buttonClass.h>
#include <keyboardClass.h>
#include <gros_crane.h>
C3D_RenderTarget* bottom;
struct Point {
    int xstart; // x-coordinate
    int ystart; // y-coordinate
    int xend;
    int yend;
};
struct Circle {
    int centerX, centerY, radius;
    std::vector<Point> points;
};
std::vector<Circle> circles;
void drawFullScreenGrid(int rows, int cols) {
    int screenWidth = 320;
    int screenHeight = 240;

    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;

    for (int i = 1; i < cols; i++) {
        float x = i * cellWidth;
        C2D_DrawRectSolid(x, 0, 0.0f, 4, screenHeight, C2D_Color32(0,0,0,255));
    }

    for (int i = 1; i < rows; i++) {
        float y = i * cellHeight;
        C2D_DrawRectSolid(0, y, 0.0f, screenWidth, 4, C2D_Color32(0,0,0,255));
    }
}


void showgrid(int grid[3][3]){
    std::cout<<" y";
    for (int i=0;i<3;i++){
        if (i==0){
            std::cout<<"    "<<i;
        }
    else {
    std::cout<<"     "<<i;
    }
    }
    std::cout<<"\nx\n";
    for (int i=0;i<3;i++){
        std::cout<<i<<"\n";
    }
    std::cout << "\033[8;7H";
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (i!=0){
                if (i==1){
                    if (j==0){
                    std::cout << "\033[9;7H";
                    std::cout<<grid[i][j];
                    }
                    else {
                        std::cout<<"     "<<grid[i][j];
                    }
                }
                else if(i==2){
                    if (j==0){
                        std::cout << "\033[10;7H";
                        std::cout<<grid[i][j];
                    }
                    else {
                        std::cout<<"     "<<grid[i][j];
                    }
                }
            }
            else{
                if (j==0){
                    std::cout<<grid[i][j];
                }
                else {
                    std::cout<<"     "<<grid[i][j];
                }
                
            }
            }
        }
        std::cout<<std::endl<<std::endl;
    }
	short checkwin(int grid[3][3],short x,short y){
    short team;
    team=grid[x][y];
    if (x==0 && y==0){
        if (grid[x+1][y]==team){
            if (grid[x+2][y]==team){
                return team;
            }
        }
        if (grid[x+1][y+1]==team){
            if (grid[x+2][y+2]==team){
                return team;
            }
        }
        if (grid[x][y+1]==team){
            if (grid[x][y+2]==team){
                return team;
            }
        }
    }
    if (x==1 && y==0){
        if (grid[x-1][y]==team){
            if (grid[x+1][y]==team){
                return team;
            }
        }
        if (grid[x+1][y]==team){
            if (grid[x-1][y]==team){
                return team;
            }
        }
        if (grid[x][y+1]==team){
            if (grid[x][y+2]==team){
                return team;
            }
        }
    }
    if (x==2 && y==0){
        if (grid[x-1][y]==team){
            if (grid[x-2][y]==team){
                return team;
            }
        }
        if (grid[x-1][y+1]==team){
            if (grid[x-2][y+2]==team){
                return team;
            }
        }
        if (grid[x][y+1]==team){
            if (grid[x][y+2]==team){
                return team;
            }
        }
    }
    if (x==0 && y==1){
        if (grid[x][y-1]==team){
            if (grid[x][y+1]==team){
                return team;
            }
        }
        if (grid[x][y+1]==team){
            if (grid[x][y-1]==team){
                return team;
            }
        }
        if (grid[x+1][y]==team){
            if (grid[x+2][y]==team){
                return team;
            }
        }
    }
    if (x==1 && y==1){
        if (grid[x][y-1]==team){
            if (grid[x][y+1]==team){
                return team;
            }
        }
        if (grid[x][y+1]==team){
            if (grid[x][y-1]==team){
                return team;
            }
        }
        if (grid[x+1][y]==team){
            if (grid[x-1][y]==team){
                return team;
            }
        }
        if (grid[x-1][y]==team){
            if (grid[x+1][y]==team){
                return team;
            }
        }
        if (grid[x-1][y-1]==team){
            if (grid[x+1][y+1]==team){
                return team;
            }
        }
        if (grid[x+1][y+1]==team){
            if (grid[x-1][y-1]==team){
                return team;
            }
        }
        if (grid[x-1][y+1]==team){
            if (grid[x+1][y-1]==team){
                return team;
            }
        }
        if (grid[x+1][y-1]==team){
            if (grid[x-1][y+1]==team){
                return team;
            }
        }
    }
    if (x==2 && y==1){
        if (grid[x][y-1]==team){
            if (grid[x][y+1]==team){
                return team;
            }
        }
        if (grid[x][y+1]==team){
            if (grid[x][y-1]==team){
                return team;
            }
        }
        if (grid[x-1][y]==team){
            if (grid[x-2][y]==team){
                return team;
            }
        }
    }
    if (x==0 && y==2){
        if (grid[x][y-1]==team){
            if (grid[x][y-2]==team){
                return team;
            }
        }
        if (grid[x+1][y]==team){
            if (grid[x+2][y]==team){
                return team;
            }
        }
        if (grid[x+1][y-1]==team){
            if (grid[x+2][y-2]==team){
                return team;
            }
        }
    }
    if (x==1 && y==2){
        if (grid[x][y-1]==team){
            if (grid[x][y-2]==team){
                return team;
            }
        }
        if (grid[x+1][y]==team){
            if (grid[x-1][y]==team){
                return team;
            }
        }
        if (grid[x-1][y]==team){
            if (grid[x+1][y]==team){
                return team;
            }
        }
    }
    if (x==2 && y==2){
        if (grid[x][y-1]==team){
            if (grid[x][y-2]==team){
                return team;
            }
        }
        if (grid[x-1][y]==team){
            if (grid[x-2][y]==team){
                return team;
            }
        }
        if (grid[x-1][y-1]==team){
            if (grid[x-2][y-2]==team){
                return team;
            }
        }
    }
    return 0;
}
void clearscreen(){
    std::cout << "\033[6;1H";
    std::cout<<"\x1b[0J";

}
void clearendscreen(){
    std::cout << "\033[12;1H";
    std::cout<<"\x1b[0J";

}
short checkdraw(int grid[3][3]){
    short drawmeter=0;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (grid[i][j]!=0){
                drawmeter++;
            }
        }
    }
    if (drawmeter==9){
        return 1;
    }
    return 0;
}
short verify(int grid[3][3]){
    short win=0;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            if (grid[i][j]!=0){
                win=checkwin(grid,i,j);
                if (win!=0){
                    return win;
                }
                else {
                    if (checkdraw(grid)==1){
                        return 3;
                    }
                }
            }
        }
    }
    return 0;
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
void player1(int grid[3][3],short x,short y){
    short cell=10,toomucherror=0;
    touchPosition touch;
    bool showonscreen=false;
    while (aptMainLoop()){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        hidTouchRead(&touch);
        C2D_SceneBegin(bottom);
        hidScanInput();
        u32 kDown = hidKeysDown();
        C2D_TargetClear(bottom, C2D_Color32(0,255,255,255));
        drawFullScreenGrid(3, 3);
		if (kDown&KEY_START){
            C2D_Fini();
            C3D_Fini();
            gfxExit();
            exit(0);
        }
        if (showonscreen==false)std::cout<<"please touch were you want to put the cross\n";
        showonscreen=true;
        if (touch.px!=0||touch.py!=0){
        cell=checkinputtouch(touch);
        interpretinput(x,y,cell);
        if (grid[x][y]==0 && x<3 && y<3){
            grid[x][y]=1;
            //std::cout<<"\033[6;1H"<<"\033[2K";
            C3D_FrameEnd(0);
            break;
        }
        else{
            if (toomucherror==3){
                std::cout << "\033[14;0H";
                std::cout<<"\x1b[0J";
                toomucherror=0;
            } 
            std::cout<<"invalid input\nlocation pointed to already used or does not exist\n" ;
            toomucherror++;
            //<<"\033[6;1H"<<"\033[2K";;
        }
        }
        C3D_FrameEnd(0);
    }
}
void player2(int grid[3][3],short x,short y){
    short cell=10,toomucherror=0;
    touchPosition touch;
    
    bool showonscreen=false;
    while (aptMainLoop()){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        hidTouchRead(&touch);
        C2D_SceneBegin(bottom);
        hidScanInput();
        u32 kDown = hidKeysDown();
        C2D_TargetClear(bottom, C2D_Color32(255,0,255,255));
        drawFullScreenGrid(3, 3);
		if (kDown&KEY_START){
            C2D_Fini();
            C3D_Fini();
            gfxExit();
            exit(0);
        }
        hidTouchRead(&touch);
        if (showonscreen==false)std::cout<<"please touch were you want to put the circle\n";
        showonscreen=true;
        if (touch.px!=0||touch.py!=0){
            cell=checkinputtouch(touch);
            interpretinput(x,y,cell);
            if (grid[x][y]==0 && x<3 && y<3){
                grid[x][y]=2;
                //std::cout<<"\033[6;1H"<<"\033[2K";
                C3D_FrameEnd(0);
                break;
            }
            else{
                if (toomucherror==3){
                std::cout << "\033[14;0H";
                std::cout<<"\x1b[0J";
                toomucherror=0;
            } 
                std::cout<<"invalid input\nlocation pointed to already used or does not exist\n" ;
                toomucherror++;//<<"\033[6;1H"<<"\033[2K";;
            }
        }
        C3D_FrameEnd(0);
    }
}
void calculate_circle(int centerx,int centery, int radius, short cell){
    double x1,y1,x2,y2;
    int xstart,ystart,xend,yend;
    double angleInRadians;
    const double PI = 3.14159265358979323846,changestep=PI/180/2;
    if (circles.size() <= static_cast<size_t>(cell))
    circles.resize(cell + 1);
    circles[cell].points.resize(720);
    circles[cell].centerX = centerx;
    circles[cell].centerY = centery;
    circles[cell].radius = radius;
    for (int angle=0;angle<720;angle++){
        angleInRadians = angle * changestep;
        x1 = centerx + radius * cos(angleInRadians);
        y1 = centery + radius * sin(angleInRadians);
        x2 = centerx + radius * cos(angleInRadians + changestep);
        y2 = centery + radius * sin(angleInRadians + changestep);
        xstart = std::round(x1);
        xend = std::round(x2);
        ystart = std::round(y1);
        yend = std::round(y2);
        circles[cell].points[angle].xstart = xstart;
        circles[cell].points[angle].xend = xend;
        circles[cell].points[angle].ystart = ystart;
        circles[cell].points[angle].yend = yend;
        //C2D_DrawLine(xstart,ystart,C2D_Color32(0,0,255,255),xend,yend,C2D_Color32(0,0,255,255),6,0);
    }
}
void keepcircle(short cell){
    for (size_t i=0;i<circles[cell].points.size();i+=1){
        C2D_DrawLine(circles[cell].points[i].xstart,circles[cell].points[i].ystart,C2D_Color32(0,0,255,255)
        ,circles[cell].points[i].xend,circles[cell].points[i].yend,C2D_Color32(0,0,255,255),10,0);
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
	int grid[3][3];
	short y=4,x=4,winner=0;
    bool turn=false;//false mean p1 true mean p2
    bool game_end, quitmessage=false;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
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
        C2D_TargetClear(bottom, C2D_Color32(255,255,255,255)); // clear to white
        showgrid(grid);
        if (turn==false){
            player1(grid,x,y);
            turn=true;
        }
        else {
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

    //C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    //C2D_SceneBegin(bottom);
    C2D_TargetClear(bottom, C2D_Color32(255,255,255,255));
    calculate_circle(20,190,20,0);
    calculate_circle(50,40,20,1);
    calculate_circle(80,70,20,2);
    calculate_circle(150,100,20,3);
    calculate_circle(100,190,20,4);
    //C3D_FrameEnd(0);

    while (aptMainLoop()){
        hidScanInput();
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_SceneBegin(bottom);
        //C2D_TargetClear(bottom, C2D_Color32(255,255,255,255)); // clear to white
        keepcircle(0);
        keepcircle(1);
        keepcircle(2);
        keepcircle(3);
        keepcircle(4);
        u32 kDown = hidKeysDown();
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

/*
int main(){
	gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, true);  // top and bottom both BGR8, with double buffering
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	//PrintConsole bottomScreen;
	//consoleInit(GFX_TOP, &bottomScreen);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	//C2D_Font font = C2D_FontLoad("romfs:/gfx/font.ttf");
	short cellselected=10 , circleposy = 0,circleposx =0,errorcicleposx=0;
    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/images/gros_crane.t3x");
    C2D_Image myImage = C2D_SpriteSheetGetImage(spriteSheet, 0);
	while (!spriteSheet){
		if (errorcicleposx>=300)errorcicleposx=0;
		spriteSheet = C2D_SpriteSheetLoad("romfs:/images/gros_crane.t3x");
		hidScanInput();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);   // begin frame
        C2D_TargetClear(top, C2D_Color32(255,255,255,255)); // clear to color chosen
        C2D_SceneBegin(top);
        C2D_DrawEllipseSolid(errorcicleposx, circleposy, 0.0f, 50, 50, C2D_Color32(255,0,0,255));
		C2D_TargetClear(bottom, C2D_Color32(255,255,255,255)); // clear to white
        C2D_SceneBegin(bottom);
		u32 kDown = hidKeysDown();
		if (kDown&KEY_START)break;
		errorcicleposx++;
		C3D_FrameEnd(0);
	}
	if (spriteSheet) {
    C2D_Image myImage = C2D_SpriteSheetGetImage(spriteSheet, 0);
    // Now you can use myImage safely
	}
	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);   // begin frame
        C2D_TargetClear(top, C2D_Color32(255,255,255,255)); // clear to color chosen
        C2D_SceneBegin(top);
        C2D_DrawImageAt(myImage, 100.0f, 50.0f, 0.0f);
		C2D_TargetClear(bottom, C2D_Color32(255,255,255,255)); // clear to white
        C2D_SceneBegin(bottom);
		u32 kDown = hidKeysDown();
		if (kDown&KEY_START)break;
		touchPosition touch;
		hidTouchRead(&touch);
		if (touch.px!=0 ||touch.py!=0){
			circleposx=touch.px-25;
			circleposy=touch.py-25;
			C2D_DrawEllipseSolid(circleposx, circleposy, 0.0f, 50, 50, C2D_Color32(255,255,0,255));	
    	}
		C3D_FrameEnd(0); 
	}
    C2D_SpriteSheetFree(spriteSheet);
	C2D_Fini();
    C3D_Fini();
	gfxExit();
	return 0;
}*/