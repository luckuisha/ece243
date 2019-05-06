/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Rectangle.h
 * Author: kimkyu9
 *
 * Created on March 25, 2019, 2:53 PM
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "VGAdraw.h"

#define HIT_WALL_TOP 1
#define HIT_WALL_RIGHT 2  //        -1  1 -2
#define HIT_WALL_BOT 3    //         4  0  2  
#define HIT_WALL_LEFT 4   //        -4  3 -3
#define HIT_WALL_TOPLEFT -1
#define HIT_WALL_TOPRIGHT -2
#define HIT_WALL_BOTRIGHT -3
#define HIT_WALL_BOTLEFT -4
#define HIT_WALL_NONE 0


typedef struct rec{
    int x; //topleft
    int y;  //corner
    int b; //height
    int h;  //base
    int dx;
    int dy;
    short int color;
}Rec;

typedef struct point{
    int x;
    int y;
} Point;


Point Rec_center(Rec r);
void Rec_draw (Rec r);
int Rec_hitWall(Rec r);
void Rec_update(Rec* r);

#endif /* RECTANGLE_H */

