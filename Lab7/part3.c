#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define HIT_WALL_TOP 1
#define HIT_WALL_RIGHT 2  //        -1  1 -2
#define HIT_WALL_BOT 3    //         4  0  2  
#define HIT_WALL_LEFT 4   //        -4  3 -3
#define HIT_WALL_TOPLEFT -1
#define HIT_WALL_TOPRIGHT -2
#define HIT_WALL_BOTRIGHT -3
#define HIT_WALL_BOTLEFT -4
#define HIT_WALL_NONE 0
#define SCREEN_X 319
#define SCREEN_Y 239
#define REC_NUM 8



volatile int pixel_buffer_start; // global variable

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

void swap (int* a, int* b);
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x1, int y1, int x2, int y2, short int color);
void clear_screen();
Point Rec_center(Rec r);
void Rec_draw (Rec r);
void Rec_draw_line (Rec r1, Rec r2);
int Rec_hitWall(Rec r);
void Rec_update(Rec* r);
void wait_for_vsync();

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    Rec recArr[REC_NUM];
	int i;
    for (i = 0; i < REC_NUM; i++){
        recArr[i].b = 3;
        recArr[i].h = 5;
        recArr[i].x = rand()%(320 - recArr[i].b+1);
        recArr[i].y = rand()%(240 - recArr[i].h+1);
        recArr[i].dx = (rand()%2 *2-1)*rand()%10;
        recArr[i].dy = (rand()%2 *2-1)*rand()%10;
        recArr[i].color  = rand()%65536  ;
    }

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
		int i;
        for (i = 0; i < REC_NUM; i++){
            int nextRecIndex = i+1;
            if(nextRecIndex == REC_NUM) nextRecIndex=0;

            Rec_draw(recArr[i]);
            Rec_draw_line(recArr[i], recArr[nextRecIndex]);
            Rec_update(&recArr[i]);
        }
        wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }

}

void Rec_draw (Rec r){
    int x;
    int y;
    for(y = r.y; y  < r.y + r.h; y++){
        for(x = r.x; x < r.x + r.b; x++){
            plot_pixel(x,y,/*rand()%65536*/r.color);
        }
    }
}

void Rec_draw_line (Rec r1, Rec r2){
    Point c1 = Rec_center(r1);
    Point c2 = Rec_center(r2);
    draw_line(c1.x, c1.y, c2.x, c2.y, /*rand()%65536*/ r1.color);
}

Point Rec_center(Rec r){
    Point center;
    center.x = (2*r.x+r.b+1)/2;
    center.y = (2*r.y+r.h+1)/2;
    return center;
}

int Rec_hitWall(Rec r){
    if(r.x+r.dx>SCREEN_X){
        if(r.y+r.dy<0)
            return HIT_WALL_TOPRIGHT;
        if(r.y+r.dy>SCREEN_Y)
            return HIT_WALL_BOTRIGHT;
        return HIT_WALL_RIGHT;
    }
    if(r.x+r.dx<0){
        if(r.y+r.dy<0)
            return HIT_WALL_TOPLEFT;
        if(r.y+r.dy>SCREEN_Y)
            return HIT_WALL_BOTLEFT;
        return HIT_WALL_LEFT;
    }
	if(r.y+r.dy<0)
		return HIT_WALL_TOP;
	if(r.y+r.dy>SCREEN_Y)
		return HIT_WALL_BOT;
	return HIT_WALL_NONE;
}



void Rec_update(Rec* r){
    int wallCase = Rec_hitWall(*r);
    switch(wallCase){
        case HIT_WALL_TOP:{
            r->dy = -r->dy;
            r->y = 0;
            break;
        }
        case HIT_WALL_RIGHT:{
            r->dx = -r->dx;
            r->x = SCREEN_X - r->b +1;
            break;
        }
        case HIT_WALL_BOT:{
            r->dy = -r->dy;
            r->y = SCREEN_Y - r->h +1;
            break;
        }
        case HIT_WALL_LEFT:{
            r->dx = -r->dx;
            r->x = 0;
            break;
        }
        case HIT_WALL_TOPLEFT:{
            r->dx = -r->dx;
            r->dy = -r->dy;
            r->x = 0;
            r->y = 0;
            break;
        }
        case HIT_WALL_TOPRIGHT:{
            r->dx = -r->dx;
            r->dy = -r->dy;
            r->x = SCREEN_X - r->b +1;
            r->y = 0;
            break;
        }
        case HIT_WALL_BOTRIGHT:{
            r->dx = -r->dx;
            r->dy = -r->dy;
            r->x = SCREEN_X - r->b +1;
            r->y = SCREEN_Y - r->h +1;
            break;
        }
        case HIT_WALL_BOTLEFT:{
            r->dx = -r->dx;
            r->dy = -r->dy;
            r->x = 0;
            r->y = SCREEN_Y - r->h +1;
            break;
        }
        case HIT_WALL_NONE:{
            r->x += r->dx;
            r->y += r->dy;
            break;
        }
        default:
            break;
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
	int x,y;
    for ( x = 0; x < 320; x++){
        for ( y = 0; y < 240; y++){
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










//Legacy Not used:
void Rec_actWall(Rec r, int wallCode);
void Rec_actWall(Rec r, int wallCode){
    switch(wallCode){
        case HIT_WALL_TOP:{
            r.dy = -r.dy;
            r.y = 0;
            break;
        }
        case HIT_WALL_RIGHT:{
            r.dx = -r.dx;
            r.x = SCREEN_X - r.b +1;
            break;
        }
        case HIT_WALL_BOT:{
            r.dy = -r.dy;
            r.y = SCREEN_Y - r.h +1;
            break;
        }
        case HIT_WALL_LEFT:{
            r.dx = -r.dx;
            r.x = 0;
            break;
        }
        case HIT_WALL_TOPLEFT:{
            r.dx = -r.dx;
            r.dy = -r.dy;
            r.x = 0;
            r.y = 0;
            break;
        }
        case HIT_WALL_TOPRIGHT:{
            r.dx = -r.dx;
            r.dy = -r.dy;
            r.x = SCREEN_X - r.b +1;
            r.y = 0;
            break;
        }
        case HIT_WALL_BOTRIGHT:{
            r.dx = -r.dx;
            r.dy = -r.dy;
            r.x = SCREEN_X - r.b +1;
            r.y = SCREEN_Y - r.h +1;
            break;
        }
        case HIT_WALL_BOTLEFT:{
            r.dx = -r.dx;
            r.dy = -r.dy;
            r.x = 0;
            r.y = SCREEN_Y - r.h +1;
            break;
        }
        case HIT_WALL_NONE:{

            break;
        }
        default:
            break;
    }
}