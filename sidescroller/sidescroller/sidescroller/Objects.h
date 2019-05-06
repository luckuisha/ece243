/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Objects.h
 * Author: kimkyu9
 *
 * Created on March 25, 2019, 3:18 PM
 */

#ifndef OBJECTS_H
#define OBJECTS_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "Sprites.h"

typedef struct mainChar{
	int health;
	int x;
	int y;
	int dx;
	int dy;
        int jumpHeight;
	bool hasAbility1;
	bool hasAbility2;
	//Ability ability1;
	//Ability ability2;
	Sprite* spritePtr;
}
MainChar;

typedef struct mob{
	int id;
	bool killed;
	bool onScreen;
	int x;
	int y;
	int dx;
	int dy;
	bool projectile;
	int coolDown;
	int health;
	Sprite* spritePtr;
}
Mob;

typedef struct terrain{
	int id;
	int x;
	int y;
	Sprite* spritePtr;
}
Terrain;


void Sprites_draw_main(MainChar Zero);
void Sprites_draw_mob(Mob troop);
void Sprites_draw_terrain(Terrain Skule);
#endif /* OBJECTS_H */

