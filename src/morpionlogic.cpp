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
void showgrid(short grid[3][3]){
    std::cout<<" y";
    for (short i=0;i<3;i++){
        if (i==0){
            std::cout<<"    "<<i;
        }
    else {
    std::cout<<"     "<<i;
    }
    }
    std::cout<<"\nx\n";
    for (short i=0;i<3;i++){
        std::cout<<i<<"\n";
    }
    std::cout << "\033[8;7H";
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
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

short checkwin(short grid[3][3],short x,short y){
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

short checkdraw(short grid[3][3]){
    short drawmeter=0;
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
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

short verify(short grid[3][3]){
    short win=0;
    for (short i=0;i<3;i++){
        for (short j=0;j<3;j++){
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