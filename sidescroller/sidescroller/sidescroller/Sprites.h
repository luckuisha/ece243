/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sprites.h
 * Author: kimkyu9
 *
 * Created on March 25, 2019, 2:56 PM
 */

#ifndef SPRITES_H
#define SPRITES_H


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "VGAdraw.h"
#include "Rectangle.h"

typedef struct sprite{
	int id;
	int b;
	int h;
	short int** colorArray;
}
Sprite;

#define NUM_OF_SPRITES 1
#define ZERO_BASE 6
#define ZERO_HEIGHT 10
#define OBJECT_ID_ZERO 0

extern Sprite allSprites[NUM_OF_SPRITES];
void declareSprites();


#endif /* SPRITES_H */

