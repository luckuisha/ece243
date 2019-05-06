/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Sprites.h"
#include "Objects.h"
Sprite allSprites[NUM_OF_SPRITES];

void declareSprites(){
    allSprites[OBJECT_ID_ZERO].id = OBJECT_ID_ZERO;
    allSprites[OBJECT_ID_ZERO].b = ZERO_BASE;
    allSprites[OBJECT_ID_ZERO].h = ZERO_HEIGHT;
    int height = allSprites[OBJECT_ID_ZERO].h;
    int base = allSprites[OBJECT_ID_ZERO].b;
    allSprites[OBJECT_ID_ZERO].colorArray = (short int **)malloc(height * sizeof(short int *));
    int i;
    for (i=0; i<height; i++) 
        allSprites[OBJECT_ID_ZERO].colorArray[i] = (short int *)malloc(base * sizeof(short int)); 
    int j;
    for(i=0; i<height; i++){
        for(j=0; j<base; j++){
            allSprites[OBJECT_ID_ZERO].colorArray[i][j] = rand()%65565;
        }
    }
    
    allSprites[OBJECT_ID_FLOOR].id = OBJECT_ID_FLOOR;
    allSprites[OBJECT_ID_FLOOR].b = FLOOR_BASE;
    allSprites[OBJECT_ID_FLOOR].h = FLOOR_HEIGHT;
    height = allSprites[OBJECT_ID_FLOOR].h;
    base = allSprites[OBJECT_ID_FLOOR].b;
    allSprites[OBJECT_ID_FLOOR].colorArray = (short int **)malloc(height * sizeof(short int *));
    for (i=0; i<height; i++) 
        allSprites[OBJECT_ID_FLOOR].colorArray[i] = (short int *)malloc(base * sizeof(short int)); 
    for(i=0; i<height; i++){
        for(j=0; j<base; j++){
            allSprites[OBJECT_ID_FLOOR].colorArray[i][j] = rand()%65565;
        }
    }
}

