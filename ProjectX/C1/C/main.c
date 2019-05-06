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
#include "address_map_arm.h"
#include "Rectangle.h"
#include "Sprites.h"
#include "Objects.h"
#include "Game.h"
#include "defines.h"
#include "interrupt_ID.h"
void set_A9_IRQ_stack(void);
void config_GIC(void);
void config_MPcore_private_timer(void);
void config_PS2(void);
void enable_A9_interrupts(void);

/* these globals are written by interrupt service routines; we declare them as
 * volatile to avoid the compiler caching their values, even in registers */

extern volatile int timeout; // used to synchronize with the timer

/* function prototypes */
void HEX_PS2(char, char, char);

/* setup private timer in the ARM A9 */
void config_MPcore_private_timer() {
    volatile int * MPcore_private_timer_ptr =
        (int *)MPCORE_PRIV_TIMER; // timer base address

    /* set the timer period */
    int counter = 20000000; // period = 1/(200 MHz) x 40x10^6 = 0.2 sec
    *(MPcore_private_timer_ptr) = counter; // write to timer load register

    /* write to control register to start timer, with interrupts */
    *(MPcore_private_timer_ptr + 2) = 0x7; // int mask = 1, mode = 1, enable = 1
}

/* setup the PS/2 interrupts */
void config_PS2() {
    volatile int * PS2_ptr = (int *)PS2_BASE; // PS/2 port address

    *(PS2_ptr) = 0xFF; /* reset */
    *(PS2_ptr + 1) =
        0x1; /* write to the PS/2 Control register to enable interrupts */
}

int main(void)
{
	set_A9_IRQ_stack();            // initialize the stack pointer for IRQ mode
    config_GIC();                  // configure the general interrupt controller
    config_MPcore_private_timer(); // configure ARM A9 private timer
    config_PS2();  // configure PS/2 port to generate interrupts

    enable_A9_interrupts(); // enable interrupts
	/* setup the PS/2 interrupts */
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    //volatile int * keyboard_ctrl_ptr = (int *)0xFF200030;

    //set front pixel buffer to start of FPGA On-chip memory
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    //now, swap the front/back buffers, to set the front buffer location
    wait_for_vsync();
    //initialize a pointer to the pixel buffer, used by drawing functions
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    //set back pixel buffer to start of SDRAM memory
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    
    initializeGame();
    while (1){
        clear_screen();
        update_objects();
        move();
        Visible_draw();
      
        
        wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }

}

