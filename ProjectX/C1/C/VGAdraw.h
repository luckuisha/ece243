/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VGAdraw.h
 * Author: kimkyu9
 *
 * Created on March 25, 2019, 2:48 PM
 */

#ifndef VGADRAW_H
#define VGADRAW_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//#include "address_map_arm.h"

typedef struct key {
	bool pressed;
	bool released;
}Key;

extern Key leftKey;
extern Key rightKey;
extern Key upKey;
extern Key downKey;
extern Key spaceKey;

extern volatile char byte1, byte2,
    byte3; /* modified by PS/2 interrupt service routine */

#define SCREEN_X 320
#define SCREEN_Y 240

extern volatile int pixel_buffer_start; // global variable

void swap (int* a, int* b);
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x1, int y1, int x2, int y2, short int color);
void clear_screen();
void HEX_PS2(char b1, char b2, char b3);
void wait_for_vsync();

#endif /* VGADRAW_H */

