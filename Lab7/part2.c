#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

volatile int pixel_buffer_start; // global variable

void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void draw_line(int x1, int y1, int x2, int y2, short int color);
void swap (int* a, int* b);
void wait_for_vsync();

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    int line1_x1 = 0;
    int line1_x2 = 150;
    int line1_y1 = 0;
    int line1_y2 = 0;
    int line1_dy = -1;
    short int line1_color = 0x001F;
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    

    while (1){
		clear_screen();
        if ( line1_y1 == 0 && line1_dy < 0){
            line1_dy = -line1_dy;
        }
        else if ( line1_y2 == 239 && line1_dy > 0){
            line1_dy = -line1_dy;
        }
        if (line1_y1 + line1_dy < 0 ){
            line1_y1 = 0;
            line1_y2 = 0;
        }
        else if (line1_y1 + line1_dy > 239 ){
            line1_y1 = 239;
            line1_y2 = 239;
        }
		else{
			line1_y1 += line1_dy;
			line1_y2 += line1_dy;
		}
        draw_line (line1_x1, line1_y1, line1_x2, line1_y2, line1_color);
        wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
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

// code not shown for clear_screen() and draw_line() subroutines

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void clear_screen(){
	register int x,y;
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