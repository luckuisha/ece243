/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: kimkyu9
 *
 * Created on March 25, 2019, 2:43 PM
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "VGAdraw.h"
#include "Rectangle.h"
#include "Sprites.h"
#include "Objects.h"
#include "Game.h"

/*typedef struct ability{
	int id;
	int coolDown;
	Sprite*  spritePtr;
}
Ability;*/

//MainChar Zero = { 10, 20, GAME_FLOOR - allSprites[OBJECT_ID_ZERO].h, 0, 0, false, false, &allSprites[OBJECT_ID_ZERO] };




int main(void)
{
    initializeGame();
    
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    //volatile int * keyboard_ctrl_ptr = (int *)0xFF200030;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

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
    
    Point topLeft = {0, 0};
    unsigned leftIndex = 0;
    unsigned rightIndex = 319;
    
    initializeGame();
    while (1){
        clear_screen();
        Visible_update (topLeft, &leftIndex, &rightIndex);
        Visible_draw (leftIndex, rightIndex);
      
        
        wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }

}

