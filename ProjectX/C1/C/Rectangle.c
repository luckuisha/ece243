/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Rectangle.h"

void Rec_draw (Rec r){
    int x;
    int y;
    for(y = r.y; y  < r.y + r.h; y++){
        for(x = r.x; x < r.x + r.b; x++){
            plot_pixel(x,y,/*rand()%65536*/r.color);
        }
    }
}

Point Rec_center(Rec r){
    Point center;
    center.x = (2*r.x+r.b+1)/2;
    center.y = (2*r.y+r.h+1)/2;
    return center;
}

int Rec_hitWall(Rec r){
    if(r.x+r.dx>=SCREEN_X){
        if(r.y+r.dy<0)
            return HIT_WALL_TOPRIGHT;
        if(r.y+r.dy>=SCREEN_Y)
            return HIT_WALL_BOTRIGHT;
        return HIT_WALL_RIGHT;
    }
    if(r.x+r.dx<0){
        if(r.y+r.dy<0)
            return HIT_WALL_TOPLEFT;
        if(r.y+r.dy>=SCREEN_Y)
            return HIT_WALL_BOTLEFT;
        return HIT_WALL_LEFT;
    }
	if(r.y+r.dy<0)
		return HIT_WALL_TOP;
	if(r.y+r.dy>=SCREEN_Y)
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
            r->x = SCREEN_X - r->b;
            break;
        }
        case HIT_WALL_BOT:{
            r->dy = -r->dy;
            r->y = SCREEN_Y - r->h;
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
            r->x = SCREEN_X - r->b;
            r->y = 0;
            break;
        }
        case HIT_WALL_BOTRIGHT:{
            r->dx = -r->dx;
            r->dy = -r->dy;
            r->x = SCREEN_X - r->b;
            r->y = SCREEN_Y - r->h;
            break;
        }
        case HIT_WALL_BOTLEFT:{
            r->dx = -r->dx;
            r->dy = -r->dy;
            r->x = 0;
            r->y = SCREEN_Y - r->h;
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
