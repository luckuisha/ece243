/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Game.h
 * Author: kimkyu9
 *
 * Created on March 25, 2019, 3:25 PM
 */

#ifndef GAME_H
#define GAME_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Objects.h"
#include "Rectangle.h"


#define SCREEN_CENTRE_X (SCREEN_X-Zero.spritePtr->b + 1)/2

#define GAME_FLOOR 200
#define MAP_STEP 10
#define GAME_SIZE_X 1500
#define GAME_SIZE_Y SCREEN_Y


typedef struct map{
	Mob* arrMobs;
	unsigned numOfMobs;
	Terrain* arrTers;
	unsigned numOfTers;
}
Map;

#define MAX_NUMBER_MOBS_VISIBLE 10
#define MAX_NUMBER_TERRAINS_VISIBLE 30

typedef struct visible{
	Mob* arrMobsPtr[MAX_NUMBER_MOBS_VISIBLE]; 
	unsigned maxNumOfMobs;
	Terrain* arrTersPtr[MAX_NUMBER_TERRAINS_VISIBLE];
	unsigned maxNumOfTers;
}
Visible;

#define MAX_NUMBER_MAINCHARACTER_BIN 1
#define MAX_NUMBER_MOBS_BIN 5
#define MAX_NUMBER_TERRAINS_BIN 5

typedef struct spaceBin{
	Mob* arrMobsPtr[MAX_NUMBER_MOBS_BIN]; //NULL terminated
        //unsigned numOfMobs;
	Terrain* arrTersPtr[MAX_NUMBER_TERRAINS_BIN]; //NULL terminated
        //unsigned numOfTers;
        MainChar* arrMainCharsPtr[MAX_NUMBER_MAINCHARACTER_BIN]; //NULL terminated
        //unsigned numOfMainChars;
        unsigned updatedFrameMobsID;
        unsigned updatedFrameTersID;
        unsigned updatedFrameMainCharsID;
}
Bin;

typedef struct spaceMap{
	Bin** bins;
        unsigned base;
	unsigned height;
        unsigned numOfMobsInBin;
        unsigned numOfTersInBin;
        unsigned numOfMainCharsInBin;
}
Space;

typedef struct global{
    MainChar Zero;
    
    Point topLeft;
    
    Map* objMap;    //Used to figure out what to spawn
    unsigned mapSize;
    unsigned mapLeftIdx;
    unsigned mapRightIdx;
    
    Visible objVisible;
    
    Space   objSpace;
    unsigned frameID;
            
    Sprite* spritesArr;

}
Global;

extern Global GAME;

void MainChar_move(MainChar*);
void Main_grounded(MainChar* ZeroPtr);


void Mob_create(int id, int x,int y, int dx, int dy, int health, int cooldown, int mapLoc, int mobIndex);
void Mob_move(Mob* troop);
void Mob_grounded(Mob* troopPtr);

void Terrain_create(int id, int x,int y,  int mapLoc, int terrainIndex);

void Map_create();

void Space_create();

void initializeGame();

void Visible_update();
void Space_update();
void MainChar_updateBins(MainChar*);
void Mob_updateBins(Mob*);
void Terrain_updateBins(Terrain*);



void screen_move ();

void update_objects();

void Visible_draw ();

void move();
#endif /* GAME_H */

