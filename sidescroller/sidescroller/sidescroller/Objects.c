/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Objects.h"


void Sprites_draw_main(MainChar Zero){
    Sprite cur = *Zero.spritePtr;
    int i,j;
    for(i=0; i<cur.h; i++){
        for(j=0;j<cur.b;j++){
            //If(colorArray[i][j]==-1 don't draw)
            plot_pixel(Zero.x,Zero.y,cur.colorArray[i][j]);
        }
    }
}

void Sprites_draw_mob(Mob troop){
    Sprite cur = *troop.spritePtr;
    int i,j;
    for(i=0; i<cur.h; i++){
        for(j=0;j<cur.b;j++){
            plot_pixel(troop.x,troop.y,cur.colorArray[i][j]);
        }
    }
}

void Sprites_draw_terrain(Terrain Skule){
    Sprite cur = *Skule.spritePtr;
    int i,j;
    for(i=0; i<cur.h; i++){
        for(j=0;j<cur.b;j++){
            plot_pixel(Skule.x,Skule.y,cur.colorArray[i][j]);
        }
    }
}