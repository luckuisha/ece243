#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

volatile int pixel_buffer_front; // global variable
volatile int pixel_buffer_back; // global variable

void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void draw_line(int x1, int y1, int x2, int y2, short int color);
void swap (int* a, int* b);

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_front = *pixel_ctrl_ptr;
    pixel_buffer_back = *(pixel_ctrl_ptr + 1);

    clear_screen();
    draw_line(0, 0, 150, 150, 0x001F);   // this line is blue
    draw_line(150, 150, 319, 0, 0x07E0); // this line is green
    draw_line(0, 220, 319, 220, 0xF800); // this line is red
    draw_line(319, 0, 0, 239, 0xF81F);   // this line is a pink color
}

// code not shown for clear_screen() and draw_line() subroutines

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_front + (y << 10) + (x << 1)) = line_color;
}

void clear_screen(){
	int x, y;
    for (x = 0; x < 320; x++){
        for (y = 0; y < 240; y++){
            plot_pixel(x, y, 0);
        }
    }
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

void swap (int* a, int* b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}