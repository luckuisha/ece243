/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Objects.h"


void Sprites_draw_main(MainChar Zero, Point topLeft){
    Sprite cur = *Zero.spritePtr;
    int i,j;
    for(i=0; i<cur.h; i++){
        for(j=0;j<cur.b;j++){
            //If(colorArray[i][j]==-1 don't draw)
            int xToDraw = Zero.x-topLeft.x + j;
            int yToDraw = Zero.y-topLeft.y + i;
            if((xToDraw>=0 || xToDraw<SCREEN_X) && (yToDraw>=0 || yToDraw<SCREEN_Y))
                plot_pixel(xToDraw,yToDraw,cur.colorArray[i][j]);
        }
    }
}

void Sprites_draw_mob(Mob troop, Point topLeft){
    Sprite cur = *troop.spritePtr;
    int i,j;
    for(i=0; i<cur.h; i++){
        for(j=0;j<cur.b;j++){
            int xToDraw = troop.x-topLeft.x + j;
            int yToDraw = troop.y-topLeft.y + i;
            if((xToDraw>=0 || xToDraw<SCREEN_X) && (yToDraw>=0 || yToDraw<SCREEN_Y))
                plot_pixel(xToDraw,yToDraw,cur.colorArray[i][j]);
        }
    }
}

void Sprites_draw_terrain(Terrain Skule, Point topLeft){
    Sprite cur = *Skule.spritePtr;
    int i,j;
    for(i=0; i<cur.h; i++){
        for(j=0;j<cur.b;j++){
            int xToDraw = Skule.x-topLeft.x + j;
            int yToDraw = Skule.y-topLeft.y + i;
            if((xToDraw>=0 || xToDraw<SCREEN_X) && (yToDraw>=0 || yToDraw<SCREEN_Y))
                plot_pixel(xToDraw,yToDraw,cur.colorArray[i][j]);
        }
    }
}