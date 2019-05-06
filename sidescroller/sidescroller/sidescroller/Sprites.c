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
}

