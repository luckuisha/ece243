/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VGAdraw.c
 * Author: kimkyu9
 *
 * Created on March 25, 2019, 2:49 PM
 */


#include "VGAdraw.h"

volatile int pixel_buffer_start; // global variable

void swap (int* a, int* b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void draw_line(int x1, int y1, int x2, int y2, short int color){
    
    bool is_steep = abs(y2-y1) > abs(x2-x1);
    if (is_steep){
        swap (&x1, &y1);
        swap (&x2, &y2);
    }
    if (x2<x1){
        swap (&x1, &x2);
        swap (&y1, &y2);
    }
    
    int dx = x2-x1;
    int dy = abs(y2-y1);
    int error = -dx/2;
    int x;
    int y = y1;
    int y_step;
    if (y1 < y2){
        y_step = 1;
    }
    else y_step = -1;

    for (x = x1; x <= x2; x++){
        if (is_steep){
            plot_pixel(y, x, color);
        }
        else plot_pixel(x, y, color);
        error += dy;
        if (error >= 0){
            y += y_step;
            error -= dx;
        }
    }
}

void clear_screen(){
	int x,y;
    for ( x = 0; x < 320; x++){
        for ( y = 0; y < 240; y++){
            plot_pixel(x, y, 0);
        }
    }
}




void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = 0xFF203020;
    register int status;
    * pixel_ctrl_ptr = 1;
    
    status = *(pixel_ctrl_ptr +3);
    while ((status & 0x1) != 0){
        status = *(pixel_ctrl_ptr +3);
    }

}