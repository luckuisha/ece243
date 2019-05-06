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

typedef struct map{
	Mob* arrMobs;
        unsigned numOfMobs;
	Terrain* arrTers;
        unsigned numOfTers;
}
Map;

typedef struct global{
    MainChar Zero;
    Map* objMap;
    unsigned mapSize;
    Sprite* spritesArr;
}
Global;

extern Global GAME;

void Mob_create(int id, int x,int y, int dx, int dy, int health, int cooldown, int mapLoc, int mobIndex);

void Terrain_create(int id, int x,int y,  int mapLoc, int terrainIndex);

void createMap();
void initializeGame();

void Visible_update(Point topLeft, unsigned * leftIndex, unsigned * rightIndex);

void Visible_draw (unsigned leftIndex, unsigned rightIndex);

#endif /* GAME_H */

