/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Game.h"
Global GAME;

void Mob_create(int id, int x,int y, int dx, int dy, int health, int cooldown, int mapLoc, int mobIndex){
    GAME.objMap[mapLoc].arrMobs[mobIndex].id = id;
    GAME.objMap[mapLoc].arrMobs[mobIndex].coolDown = cooldown; 
    GAME.objMap[mapLoc].arrMobs[mobIndex].x = x;
    GAME.objMap[mapLoc].arrMobs[mobIndex].y = y;
    GAME.objMap[mapLoc].arrMobs[mobIndex].dx = dx;
    GAME.objMap[mapLoc].arrMobs[mobIndex].dy = dy;
    GAME.objMap[mapLoc].arrMobs[mobIndex].health = 1;
    GAME.objMap[mapLoc].arrMobs[mobIndex].killed = false;
    GAME.objMap[mapLoc].arrMobs[mobIndex].onScreen = false;
    GAME.objMap[mapLoc].arrMobs[mobIndex].projectile = false;
    //A function that takes id and sets all these accordingly
    GAME.objMap[mapLoc].arrMobs[mobIndex].spritePtr = &GAME.spritesArr[id];
}

void Terrain_create(int id, int x,int y,  int mapLoc, int terrainIndex){
    GAME.objMap[mapLoc].arrTers[terrainIndex].id = id;
    GAME.objMap[mapLoc].arrTers[terrainIndex].x = x;
    GAME.objMap[mapLoc].arrTers[terrainIndex].y = y;
    //A function that takes id and sets all these accordingly
    GAME.objMap[mapLoc].arrTers[terrainIndex].spritePtr = &GAME.spritesArr[id];
}

void createMap(){
    GAME.objMap[31].arrMobs = (Mob *)malloc(1/*# of mobs*/ * sizeof(Mob));
    GAME.objMap[31].numOfMobs = 1;
    GAME.objMap[30].arrTers =  (Terrain *)malloc(1/*# of terrain objs*/* sizeof(Terrain));
    GAME.objMap[31].numOfTers = 1;
    Mob_create(0, 310, GAME_FLOOR - ZERO_BASE, -1, 0, 1, 0, 31, 0 );
    Terrain_create(0, 305, GAME_FLOOR - 50, 30, 0);
}

void Visible_update(Point topLeft, unsigned * leftIndex, unsigned * rightIndex){
    *leftIndex = topLeft.x/MAP_STEP;
    *rightIndex = (topLeft.x+SCREEN_X)/MAP_STEP;
}

void initializeGame(){
    declareSprites();
    GAME.spritesArr = allSprites;  
    GAME.Zero.health = 10;
    GAME.Zero.x = SCREEN_X/2+1 - (GAME.spritesArr[OBJECT_ID_ZERO].b+1)/2;
    GAME.Zero.y = GAME_FLOOR - GAME.spritesArr[OBJECT_ID_ZERO].h;
    GAME.Zero.dx = 0;
    GAME.Zero.dy = 0;
    GAME.Zero.jumpHeight = 20;
    GAME.Zero.hasAbility1 = false;
    GAME.Zero.hasAbility2 = false; 
    GAME.Zero.spritePtr = &GAME.spritesArr[OBJECT_ID_ZERO];  
    
    GAME.objMap = (Map *)malloc(GAME_SIZE_X/MAP_STEP *sizeof(Map));
    GAME.mapSize = GAME_SIZE_X/MAP_STEP;
    createMap();
    
    //function to populate the MAP
}


void Visible_draw (unsigned leftIndex, unsigned rightIndex){
    
    if (rightIndex >= GAME.mapSize) rightIndex = GAME.mapSize-1;
    Sprites_draw_main (GAME.Zero);
    for (; leftIndex <= rightIndex; leftIndex++){
        int i;
        for (i = 0; i < GAME.objMap[leftIndex].numOfMobs; i++){
            Sprites_draw_mob(GAME.objMap[leftIndex].arrMobs[i]);
        }
        for (i = 0; i < GAME.objMap[leftIndex].numOfTers; i++){
            Sprites_draw_terrain(GAME.objMap[leftIndex].arrTers[i]);
        }
    }
    
}