/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Game.h"
Global GAME;

#define LEFT 4
#define RIGHT 2
#define TOP 1
#define BOT 3
#define TOPLEFT -1
#define TOPRIGHT -2
#define BOTRIGHT -3
#define BOTLEFT -4

void Mob_create(int id, int x,int y, int dx, int dy, int health, int cooldown, int mapLoc, int mobIndex){
    GAME.objMap[mapLoc].arrMobs[mobIndex].id = id;
    GAME.objMap[mapLoc].arrMobs[mobIndex].coolDown = cooldown; 
    GAME.objMap[mapLoc].arrMobs[mobIndex].x = x;
    GAME.objMap[mapLoc].arrMobs[mobIndex].y = y;
    GAME.objMap[mapLoc].arrMobs[mobIndex].dx = dx;
    GAME.objMap[mapLoc].arrMobs[mobIndex].dy = dy;
    GAME.objMap[mapLoc].arrMobs[mobIndex].health = 1;
    GAME.objMap[mapLoc].arrMobs[mobIndex].killed = false;
    GAME.objMap[mapLoc].arrMobs[mobIndex].onScreen = false;
    GAME.objMap[mapLoc].arrMobs[mobIndex].inVisibleVector = false;
    GAME.objMap[mapLoc].arrMobs[mobIndex].spawned = false;
    GAME.objMap[mapLoc].arrMobs[mobIndex].projectile = false;
	GAME.objMap[mapLoc].arrMobs[mobIndex].grounded = false;
    //A function that takes id and sets all these accordingly
    GAME.objMap[mapLoc].arrMobs[mobIndex].spritePtr = &GAME.spritesArr[id];
}

void Terrain_create(int id, int x,int y,  int mapLoc, int terrainIndex){
    GAME.objMap[mapLoc].arrTers[terrainIndex].id = id;
    GAME.objMap[mapLoc].arrTers[terrainIndex].x = x;
    GAME.objMap[mapLoc].arrTers[terrainIndex].y = y;
    GAME.objMap[mapLoc].arrTers[terrainIndex].onScreen = false;
    GAME.objMap[mapLoc].arrTers[terrainIndex].inVisibleVector = false;
    //A function that takes id and sets all these accordingly
    GAME.objMap[mapLoc].arrTers[terrainIndex].spritePtr = &GAME.spritesArr[id];
}

void Map_create(){
    GAME.objMap[1].arrTers =  (Terrain *)malloc(1/*# of terrain objs*/* sizeof(Terrain));
    GAME.objMap[1].numOfTers = 1;
    GAME.objMap[31].arrMobs = (Mob *)malloc(1/*# of mobs*/ * sizeof(Mob));
    GAME.objMap[31].numOfMobs = 1;
    GAME.objMap[30].arrTers =  (Terrain *)malloc(1/*# of terrain objs*/* sizeof(Terrain));
    GAME.objMap[30].numOfTers = 1;
    Mob_create(0, 310, GAME_FLOOR - ZERO_HEIGHT, -1, 0, 1, 0, 31, 0 );
    Terrain_create(0, 305, GAME_FLOOR - 50, 30, 0);
    Terrain_create(1, 10, GAME_FLOOR +5, 1, 0);
}

void Space_create(){
    GAME.objSpace.bins = (Bin **)malloc(GAME.objSpace.height * sizeof(Bin *));
    int i,j;
    for (i=0; i<GAME.objSpace.height; i++) 
        GAME.objSpace.bins[i] = (Bin *)malloc(GAME.objSpace.base * sizeof(Bin));
    
    for(i=0; i<GAME.objSpace.height; i++)
        for(j=0; j<GAME.objSpace.base; j++){
            GAME.objSpace.bins[i][j].updatedFrameMobsID = GAME.frameID;
            GAME.objSpace.bins[i][j].updatedFrameTersID = GAME.frameID;
            GAME.objSpace.bins[i][j].updatedFrameMainCharsID = GAME.frameID;
        }
}

void MainChar_updateBins(MainChar* ZeroPtr){
    unsigned i,j,k;
    if(ZeroPtr->x+ZeroPtr->spritePtr->b>GAME.objSpace.base || ZeroPtr->x<0 || ZeroPtr->y+ZeroPtr->spritePtr->h>GAME.objSpace.height || ZeroPtr->y<0 )
        return;
        
    for(i=ZeroPtr->y; i<ZeroPtr->y+ZeroPtr->spritePtr->h; i++){
        for(j=ZeroPtr->x; j<ZeroPtr->x+ZeroPtr->spritePtr->b; j++){
            bool foundSpot = true;
            k=0;
            if(GAME.frameID == GAME.objSpace.bins[i][j].updatedFrameMainCharsID){
                foundSpot = false;
                for(; k<GAME.objSpace.numOfMainCharsInBin; k++){
                    if(GAME.objSpace.bins[i][j].arrMainCharsPtr[k] == NULL){
                        foundSpot = true;
                        break;
                    } 
                }
            }
            if(foundSpot){
                GAME.objSpace.bins[i][j].arrMainCharsPtr[k] = ZeroPtr;
                k++;
                if(k<GAME.objSpace.numOfMainCharsInBin)
                    GAME.objSpace.bins[i][j].arrMainCharsPtr[k] = NULL;
            }
            else{
                printf("Space too crowded to add new character. \n");
            }
            GAME.objSpace.bins[i][j].updatedFrameMainCharsID = GAME.frameID;
        }
    }
}

void Mobs_updateBins(Mob* troopPtr){
    unsigned i,j,k;
    if(troopPtr->x+troopPtr->spritePtr->b>GAME.objSpace.base || troopPtr->x<0 || troopPtr->y+troopPtr->spritePtr->h>GAME.objSpace.height || troopPtr->y<0 )
        return;
        
    for(i=troopPtr->y; i<troopPtr->y+troopPtr->spritePtr->h; i++){
        for(j=troopPtr->x; j<troopPtr->x+troopPtr->spritePtr->b; j++){
            bool foundSpot = true;
            k=0;
            if(GAME.frameID == GAME.objSpace.bins[i][j].updatedFrameMobsID){
                foundSpot = false;
                for(; k<GAME.objSpace.numOfMobsInBin; k++){
                    if(GAME.objSpace.bins[i][j].arrMobsPtr[k] == NULL){
                        foundSpot = true;
                        break;
                    } 
                }
            }
            if(foundSpot){
                GAME.objSpace.bins[i][j].arrMobsPtr[k] = troopPtr;
                k++;
                if(k<GAME.objSpace.numOfMobsInBin)
                    GAME.objSpace.bins[i][j].arrMobsPtr[k] = NULL;
            }
            else{
                printf("Space too crowded to add new mob. \n");
            }
            GAME.objSpace.bins[i][j].updatedFrameMobsID = GAME.frameID;
        }
    }
    
}

void Terrain_updateBins(Terrain* skulePtr){
    unsigned i,j,k;
    if(skulePtr->x+skulePtr->spritePtr->b>GAME.objSpace.base || skulePtr->x<0 || skulePtr->y+skulePtr->spritePtr->h>GAME.objSpace.height || skulePtr->y<0 )
        return;
        
    for(i=skulePtr->y; i<skulePtr->y+skulePtr->spritePtr->h; i++){
        for(j=skulePtr->x; j<skulePtr->x+skulePtr->spritePtr->b; j++){
            bool foundSpot = true;
            k=0;
            if(GAME.frameID == GAME.objSpace.bins[i][j].updatedFrameTersID){
                foundSpot = false;
                for(; k<GAME.objSpace.numOfTersInBin; k++){
                    if(GAME.objSpace.bins[i][j].arrTersPtr[k] == NULL){
                        foundSpot = true;
                        break;
                    } 
                }
            }
            if(foundSpot){
                GAME.objSpace.bins[i][j].arrTersPtr[k] = skulePtr;
                k++;
                if(k<GAME.objSpace.numOfTersInBin)
                    GAME.objSpace.bins[i][j].arrTersPtr[k] = NULL;
            }
            else{
                printf("Space too crowded to add new terrain. \n");
            }
            GAME.objSpace.bins[i][j].updatedFrameTersID = GAME.frameID;
        }
    }
    
}

void Main_grounded(MainChar* ZeroPtr){
    int i, j;
    for(i=ZeroPtr->x; i<ZeroPtr->x+ZeroPtr->spritePtr->b; i++){
        if(GAME.frameID == GAME.objSpace.bins[ZeroPtr->y+ZeroPtr->spritePtr->h][i].updatedFrameTersID){
            for(j = 0; j<GAME.objSpace.numOfMainCharsInBin; j++){
                if(GAME.objSpace.bins[ZeroPtr->y+ZeroPtr->spritePtr->h][i].arrMainCharsPtr[j] != NULL){
                    ZeroPtr->grounded = true;
                    break;
                }
                else{
                    ZeroPtr->grounded = false;
                    break;
                }
            }
        }
        if (ZeroPtr->grounded == true) break;
    }
}

void Mob_grounded(Mob* troopPtr){
    int i, j;
    for(i=troopPtr->x; i<troopPtr->x+troopPtr->spritePtr->b; i++){
        if(GAME.frameID == GAME.objSpace.bins[troopPtr->y+troopPtr->spritePtr->h][i].updatedFrameTersID){
            for(j = 0; j<GAME.objSpace.numOfMobsInBin; j++){
                if(GAME.objSpace.bins[troopPtr->y+troopPtr->spritePtr->h][i].arrMobsPtr[j] != NULL){
                    troopPtr->grounded = true;
                    break;
                }
                else{
                    troopPtr->grounded = false;
                    break;
                }
            }
        }
        if (troopPtr->grounded == true) break;
    }
}

void MainChar_move(MainChar* ZeroPtr){
    Main_grounded(ZeroPtr);
    if (!ZeroPtr->grounded) ZeroPtr->dy += 2;
    int i,j;
    int mainDx = ZeroPtr->dx;
    int mainDy = ZeroPtr->dy;
    int endY = ZeroPtr->y + mainDy; 
    int endX = ZeroPtr->x + mainDx;
    int startY = ZeroPtr->y; 
    int startX = ZeroPtr->x;
    int size = abs(mainDy)+1;

    if (abs(mainDx) > abs(mainDy)){
        size = abs(mainDx)+1;
    }
    Point * nextPointValues = (Point *)malloc(size * sizeof(Point));

    bool is_steep = abs(endY-startY) > abs(endX-startX);
    if (is_steep){
        swap (&startX, &startY);
        swap (&endX, &endY);
    }
    if (endX<startX){
        swap (&startX, &endX);
        swap (&startY, &endY);
    }
    
    int dx = endX-startX;
    int dy = abs(endY-startY);
    int error = -dx/2;
    int x;
    int y = startY;
    int y_step;
    if (startY < endY){
        y_step = 1;
    }
    else y_step = -1;

    int arrayInc = 0;
    for (x = startX; x <= endX; x++){
        if (is_steep){
            nextPointValues[arrayInc].x = y;
            nextPointValues[arrayInc].y = x;
        }
        else{
             nextPointValues[arrayInc].x = x;
            nextPointValues[arrayInc].y = y;
        }
        error += dy;
        if (error >= 0){
            y += y_step;
            error -= dx;
        }
        arrayInc++;
    }

    // int mainDx = ZeroPtr->dx;
    // int mainDy = ZeroPtr->dy;
    // int endY = ZeroPtr->y + mainDy; 
    // int endX = ZeroPtr->x + mainDx;
    // int startY = ZeroPtr->y; 
    // int startX = ZeroPtr->x;

    Mob * hitMobPtr = NULL;
    Terrain * hitTerPtr = NULL;
    bool hitMob = false;
    bool hitTer = false;
    int locationTer = 0;
    int locationMob = 0;

    

    for (arrayInc = 0; arrayInc < size; arrayInc ++){
        int constantX = nextPointValues[arrayInc].x + ZeroPtr->spritePtr->b;
        int constantY = nextPointValues[arrayInc].y;
        if (mainDx < 0){
            constantX = nextPointValues[arrayInc].x;
        }
        if (mainDy > 0){
            constantY = nextPointValues[arrayInc].y + ZeroPtr->spritePtr->h;
        }
        for (i = nextPointValues[arrayInc].x; i < nextPointValues[arrayInc].x + ZeroPtr->spritePtr->b ; i++){
            if(GAME.frameID == GAME.objSpace.bins[constantY][i].updatedFrameTersID){
                for(j=0; j<GAME.objSpace.numOfTersInBin; j++){
                    if(GAME.objSpace.bins[constantY][i].arrTersPtr[j] == NULL){
                        break;
                    }
                    else{
                        hitTer = true;
                        hitTerPtr = GAME.objSpace.bins[constantY][i].arrTersPtr[j];
                        if (mainDy > 0)
                            locationTer = BOT;
                        else locationTer = TOP;
                        break;
                    }
                }
            }
           if(GAME.frameID == GAME.objSpace.bins[constantY][i].updatedFrameMobsID){
                for(j=0; j<GAME.objSpace.numOfMobsInBin; j++){
                    if(GAME.objSpace.bins[constantY][i].arrMobsPtr[j] == NULL){
                        break;
                    }
                    else{
                        hitMob = true;
                        hitMobPtr = GAME.objSpace.bins[constantY][i].arrMobsPtr[j];
                        if (mainDy > 0)
                            locationMob = BOT;
                        else locationMob = TOP;
                        break;
                    }
                }
            }

        }

        
        for (i = nextPointValues[arrayInc].y; i < nextPointValues[arrayInc].y + ZeroPtr->spritePtr->h; i++){
            if(GAME.frameID == GAME.objSpace.bins[i][constantX].updatedFrameTersID){
                for(j=0; j<GAME.objSpace.numOfTersInBin; j++){
                    if(GAME.objSpace.bins[i][constantX].arrTersPtr[j] == NULL){
                        break;
                    }
                    else{
                        hitTer = true;
                        hitTerPtr = GAME.objSpace.bins[i][constantX].arrTersPtr[j];
                       
                        if (mainDx > 0){
                             if (locationTer == TOP){
                                 locationTer = TOPRIGHT;
                             }
                             else if (locationTer == BOT){
                                 locationTer = BOTRIGHT;
                             }
                             locationTer = RIGHT;
                        }
                        else{
                            if (locationTer == TOP){
                                locationTer = TOPLEFT;
                            }
                            else if (locationTer == BOT){
                                 locationTer = BOTLEFT;
                             }
                            locationTer = LEFT;
                        }
                        break;
                    }
                }
            }
            if(GAME.frameID == GAME.objSpace.bins[i][constantX].updatedFrameMobsID){
                for(j=0; j<GAME.objSpace.numOfMobsInBin; j++){
                    if(GAME.objSpace.bins[i][constantX].arrMobsPtr[j] == NULL){
                        break;
                    }
                    else{
                        hitMob = true;
                        hitMobPtr = GAME.objSpace.bins[i][constantX].arrMobsPtr[j];
                         if (mainDx > 0){
                             if (locationMob == TOP){
                                 locationMob = TOPRIGHT;
                             }
                             else if (locationMob == BOT){
                                 locationMob = BOTRIGHT;
                             }
                             locationMob = RIGHT;
                        }
                        else{
                            if (locationMob == TOP){
                                locationMob = TOPLEFT;
                            }
                            else if (locationMob == BOT){
                                 locationMob = BOTLEFT;
                             }
                            locationMob = LEFT;
                        }
                        break;
                    }
                }
            }
        }

        if (hitMob || hitTer) break;
    }

    if (hitTer){
        switch (locationTer){
            case TOP:{
                ZeroPtr->y = nextPointValues[arrayInc].y;
                ZeroPtr->dy = 0;
                break;
            }
            case RIGHT:{
                ZeroPtr->x = nextPointValues[arrayInc].x ;
                ZeroPtr->dx = 0;
                break;
            }
            case BOT:{
                ZeroPtr->y = nextPointValues[arrayInc].y ;
                ZeroPtr->dy = 0;
                ZeroPtr->grounded = true;
                break;
            }
            case LEFT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->dx = 0;
                break;
            }
            case TOPLEFT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->y = nextPointValues[arrayInc].y;
                ZeroPtr->dx = 0;
                ZeroPtr->dy= 0;
                break;
            }
            case TOPRIGHT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->dx = 0;
                ZeroPtr->y = nextPointValues[arrayInc].y;
                ZeroPtr->dy = 0;
                break;
            }
            case BOTRIGHT:{
               ZeroPtr->y = nextPointValues[arrayInc].y ;
                ZeroPtr->dy = 0;
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->dx = 0;
                ZeroPtr->grounded = true;
                break;
            }
            case BOTLEFT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->dx = 0;
                ZeroPtr->y = nextPointValues[arrayInc].y ;
                ZeroPtr->dy = 0;
                ZeroPtr->grounded = true;
                break;
            }
            default:{
                ZeroPtr->x += ZeroPtr->dx;
                ZeroPtr->y += ZeroPtr->dy;
                break;
            }
        }
    }

    if (hitMob){
        switch (locationMob){
            case TOP:{
                ZeroPtr->y = nextPointValues[arrayInc].y;
                ZeroPtr->dy = 0;
                ZeroPtr->health -= 1;
                break;
            }
            case RIGHT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->dx = -ZeroPtr->dx-5;
                ZeroPtr->health -= 1;
                break;
            }
            case BOT:{
                ZeroPtr->y = nextPointValues[arrayInc].y ;
                ZeroPtr->dy = 0;
                hitMobPtr->health -= 1;
                break;
            }
            case LEFT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->dx = -ZeroPtr->dx+5;
                ZeroPtr->health -= 1;
                break;
            }
            case TOPLEFT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                ZeroPtr->y = nextPointValues[arrayInc].y;
                ZeroPtr->dx = -ZeroPtr->dx+5;
                //ZeroPtr->dy= 0;
                ZeroPtr->health -= 1;
                break;
            }
            case TOPRIGHT:{
                ZeroPtr->x = nextPointValues[arrayInc].x ;
                ZeroPtr->dx = -ZeroPtr->dx-5;
                ZeroPtr->y = nextPointValues[arrayInc].y;
                //ZeroPtr->dy = 0;
                ZeroPtr->health -= 1;
                break;
            }
            case BOTRIGHT:{
               ZeroPtr->y = nextPointValues[arrayInc].y;
                ZeroPtr->dy = 0;
                ZeroPtr->x = nextPointValues[arrayInc].x ;
                //ZeroPtr->dx = 0;
                hitMobPtr->health -= 1;
                break;
            }
            case BOTLEFT:{
                ZeroPtr->x = nextPointValues[arrayInc].x;
                //ZeroPtr->dx = 0;
                ZeroPtr->y = nextPointValues[arrayInc].y ;
                ZeroPtr->dy = 0;
                hitMobPtr->health -= 1;
                break;
            }
            default:{
                if (!hitTer){
                    ZeroPtr->x += ZeroPtr->dx;
                    ZeroPtr->y += ZeroPtr->dy;
                }
                break;
            }
        }
    }

    if(!hitMob && !hitTer){
        ZeroPtr->x += ZeroPtr->dx;
        ZeroPtr->y += ZeroPtr->dy;
    }

}

void Mob_move(Mob * troopPtr){
	if (troopPtr->health == 0){
		troopPtr->killed = true;
		printf("mobDied. \n");
		return;
	}
    Mob_grounded(troopPtr);
    if (!troopPtr->grounded) troopPtr->dy += 2;
    int i,j;
    int mainDx = troopPtr->dx;
    int mainDy = troopPtr->dy;
    int endY = troopPtr->y + mainDy; 
    int endX = troopPtr->x + mainDx;
    int startY = troopPtr->y; 
    int startX = troopPtr->x;
    int size = abs(mainDy) + 1;

    if (abs(mainDx) > abs(mainDy)){
        size = abs(mainDx)+1;
    }
    Point * nextPointValues = (Point *)malloc(size * sizeof(Point));

    bool is_steep = abs(endY-startY) > abs(endX-startX);
    if (is_steep){
        swap (&startX, &startY);
        swap (&endX, &endY);
    }
    if (endX<startX){
        swap (&startX, &endX);
        swap (&startY, &endY);
    }
    
    int dx = endX-startX;
    int dy = abs(endY-startY);
    int error = -dx/2;
    int x;
    int y = startY;
    int y_step;
    if (startY < endY){
        y_step = 1;
    }
    else y_step = -1;

    int arrayInc = 0;
    for (x = startX; x <= endX; x++){
        if (is_steep){
            nextPointValues[arrayInc].x = y;
            nextPointValues[arrayInc].y = x;
        }
        else{
             nextPointValues[arrayInc].x = x;
            nextPointValues[arrayInc].y = y;
        }
        error += dy;
        if (error >= 0){
            y += y_step;
            error -= dx;
        }
        arrayInc++;
    }

    // int mainDx = troopPtr->dx;
    // int mainDy = troopPtr->dy;
    // int endY = troopPtr->y + mainDy; 
    // int endX = troopPtr->x + mainDx;
//    int startY = troopPtr->y; 
//    int startX = troopPtr->x;

    Terrain * hitTerPtr = NULL;
    bool hitTer = false;
    int locationTer = 0;

    

    for (arrayInc = 0; arrayInc < size; arrayInc ++){
        
        int constantX = nextPointValues[arrayInc].x + troopPtr->spritePtr->b;
        int constantY = nextPointValues[arrayInc].y;
        if (mainDx < 0){
            constantX = nextPointValues[arrayInc].x;
        }
        if (mainDy > 0){
            constantY = nextPointValues[arrayInc].y + troopPtr->spritePtr->h;
        }
        
        for (i = nextPointValues[arrayInc].x; i < nextPointValues[arrayInc].x + troopPtr->spritePtr->b ; i++){
            if(GAME.frameID == GAME.objSpace.bins[constantY][i].updatedFrameTersID){
                for(j=0; j<GAME.objSpace.numOfTersInBin; j++){
                    if(GAME.objSpace.bins[constantY][i].arrTersPtr[j] == NULL){
                        break;
                    }
                    else{
                        hitTer = true;
                        hitTerPtr = GAME.objSpace.bins[constantY][i].arrTersPtr[j];
                        if (mainDy > 0)
                            locationTer = BOT;
                        else locationTer = TOP;
                        break;
                    }
                }
            }
        }

        
        for (i = nextPointValues[arrayInc].y; i < nextPointValues[arrayInc].y + troopPtr->spritePtr->h; i++){
            if(GAME.frameID == GAME.objSpace.bins[i][constantX].updatedFrameTersID){
                for(j=0; j<GAME.objSpace.numOfTersInBin; j++){
                    if(GAME.objSpace.bins[i][constantX].arrTersPtr[j] == NULL){
                        break;
                    }
                    else{
                        hitTer = true;
                        hitTerPtr = GAME.objSpace.bins[i][constantX].arrTersPtr[j];
                       
                        if (mainDx > 0){
                             if (locationTer == TOP){
                                 locationTer = TOPRIGHT;
                             }
                             else if (locationTer == BOT){
                                 locationTer = BOTRIGHT;
                             }
                             locationTer = RIGHT;
                        }
                        else{
                            if (locationTer == TOP){
                                locationTer = TOPLEFT;
                            }
                            else if (locationTer == BOT){
                                 locationTer = BOTLEFT;
                             }
                            locationTer = LEFT;
                        }
                        break;
                    }
                }
            }
        }

        if (hitTer) break;
    }


    switch (locationTer){
        case TOP:{
            troopPtr->y = nextPointValues[arrayInc].y;
            troopPtr->dy = 0;
            break;
        }
        case RIGHT:{
            troopPtr->x = nextPointValues[arrayInc].x ;
            troopPtr->dx = -troopPtr->dx;
            break;
        }
        case BOT:{
            troopPtr->y = nextPointValues[arrayInc].y ;
            troopPtr->dy = 0;
            troopPtr->grounded = true;
            break;
        }
        case LEFT:{
            troopPtr->x = nextPointValues[arrayInc].x;
            troopPtr->dx = -troopPtr->dx;
            break;
        }
        case TOPLEFT:{
            troopPtr->x = nextPointValues[arrayInc].x;
            troopPtr->y = nextPointValues[arrayInc].y;
            troopPtr->dx = -troopPtr->dx;
            troopPtr->dy= 0;
            break;
        }
        case TOPRIGHT:{
            troopPtr->x = nextPointValues[arrayInc].x ;
            troopPtr->dx = -troopPtr->dx;
            troopPtr->y = nextPointValues[arrayInc].y;
            troopPtr->dy = 0;
            break;
        }
        case BOTRIGHT:{
           troopPtr->y = nextPointValues[arrayInc].y ;
            troopPtr->dy = 0;
            troopPtr->x = nextPointValues[arrayInc].x ;
            troopPtr->dx = -troopPtr->dx;
            troopPtr->grounded = true;
            break;
        }
        case BOTLEFT:{
            troopPtr->x = nextPointValues[arrayInc].x;
            troopPtr->dx = -troopPtr->dx;
            troopPtr->y = nextPointValues[arrayInc].y ;
            troopPtr->dy = 0;
            troopPtr->grounded = true;
            break;
        }
        default:{
            troopPtr->x += troopPtr->dx;
            troopPtr->y += troopPtr->dy;
            break;
        }
    }
    free(nextPointValues);

}

void Space_update(){
    MainChar_updateBins(&GAME.Zero);
    unsigned i,j,k;
    for(i=0; i<GAME.objVisible.maxNumOfMobs; i++){
        if(GAME.objVisible.arrMobsPtr[i] == NULL) continue;
        Mobs_updateBins(GAME.objVisible.arrMobsPtr[i]);
    }

    for(i = 0; i<GAME.objVisible.maxNumOfTers; i++){
        Terrain* curTerPtr =  GAME.objVisible.arrTersPtr[i];
        if(curTerPtr == NULL) continue;
        
        Terrain_updateBins(curTerPtr);
    }
}

void Visible_update(){
    
    unsigned i,j,k;
    //Remove mobs not visible anymore:
    for(i = 0; i<GAME.objVisible.maxNumOfMobs; i++){
        if(GAME.objVisible.arrMobsPtr[i] == NULL) continue;
        //Remove from visible:
        if(/*GAME.objVisible.arrMobsPtr[i]->killed ||*/ !GAME.objVisible.arrMobsPtr[i]->onScreen){
            GAME.objVisible.arrMobsPtr[i]->inVisibleVector = false;
            GAME.objVisible.arrMobsPtr[i] = NULL;
            continue;
        }
    }
    //Remove ters not visible anymore:
    for(i = 0; i<GAME.objVisible.maxNumOfTers; i++){
        if(GAME.objVisible.arrTersPtr[i] == NULL) continue;
        //Remove from visible:
        if(!GAME.objVisible.arrTersPtr[i]->onScreen){
            GAME.objVisible.arrTersPtr[i]->inVisibleVector = false;
            GAME.objVisible.arrTersPtr[i] = NULL;
            continue;
        }
    }
    //Add Mobs that are visible now:
    for(k=GAME.mapLeftIdx; k<=GAME.mapRightIdx; k++){//Loop through map
        for(j=0; j<GAME.objMap[k].numOfMobs; j++){//Loop through mobs in the map
            if(GAME.objMap[k].arrMobs[j].onScreen && !GAME.objMap[k].arrMobs[j].inVisibleVector){
                for(i = 0; i<GAME.objVisible.maxNumOfMobs; i++){ //Find a spot in visible vector
                    if(GAME.objVisible.arrMobsPtr[i] == NULL){
                        //Add the mob to the visible
                        GAME.objVisible.arrMobsPtr[i] = &GAME.objMap[k].arrMobs[j];
                        GAME.objVisible.arrMobsPtr[i]->inVisibleVector = true;
                        break;
                    }
                    //else continue;
                }
                if(GAME.objVisible.arrMobsPtr[i]->inVisibleVector == false)
                    printf("No space for a new mob.\n");
            }
        }
    }
    //Add ters that are visible now:
    for(k=GAME.mapLeftIdx; k<=GAME.mapRightIdx; k++){//Loop through map
        for(j=0; j<GAME.objMap[k].numOfTers; j++){//Loop through ters in the map
            if(GAME.objMap[k].arrTers[j].onScreen && !GAME.objMap[k].arrTers[j].inVisibleVector){
                for(i = 0; i<GAME.objVisible.maxNumOfTers; i++){ //Find a spot in visible vector
                    if(GAME.objVisible.arrTersPtr[i] == NULL){
                        //Add the ters to the visible
                        GAME.objVisible.arrTersPtr[i] = &GAME.objMap[k].arrTers[j];
                        GAME.objVisible.arrTersPtr[i]->inVisibleVector = true;
                        break;
                    }
                    //else continue;
                }
                if(GAME.objVisible.arrTersPtr[i]->inVisibleVector == false)
                    printf("No space for a new ter.\n");
            }
        }
    }
}

void initializeGame(){
    declareSprites();
    GAME.spritesArr = allSprites;  
    GAME.Zero.health = 10;
    GAME.Zero.x = SCREEN_X/2+1 - (GAME.spritesArr[OBJECT_ID_ZERO].b+1)/2;
    GAME.Zero.y = GAME_FLOOR - GAME.spritesArr[OBJECT_ID_ZERO].h;
    GAME.Zero.dx = 0;
    GAME.Zero.dy = 0;
    GAME.Zero.jumpHeight = 20;
    GAME.Zero.hasAbility1 = false;
    GAME.Zero.hasAbility2 = false; 
    GAME.Zero.spritePtr = &GAME.spritesArr[OBJECT_ID_ZERO];  
	GAME.Zero.grounded = false;
    
    GAME.topLeft.x = 0;
    GAME.topLeft.y = 0;
    GAME.mapLeftIdx = 0/MAP_STEP;
    GAME.mapRightIdx = ( (0+(SCREEN_X-1))/MAP_STEP );
    
    GAME.mapSize = (GAME_SIZE_X-1)/MAP_STEP + 1;
    GAME.objMap = (Map *)malloc(GAME.mapSize *sizeof(Map));
    
    unsigned i;
    for (i = 0; i < GAME.mapSize; i++){
        GAME.objMap[i].numOfTers = 0;
        GAME.objMap[i].numOfMobs = 0;
    }
    
    //function to populate the MAP
    Map_create();
    //Visible:
    GAME.objVisible.maxNumOfMobs = MAX_NUMBER_MOBS_VISIBLE;
    GAME.objVisible.maxNumOfTers = MAX_NUMBER_TERRAINS_VISIBLE;
    for(i = 0; i<GAME.objVisible.maxNumOfMobs; i++){
        GAME.objVisible.arrMobsPtr[i] = NULL;
    }
    for(i = 0; i<GAME.objVisible.maxNumOfTers; i++){
        GAME.objVisible.arrTersPtr[i] = NULL;
    }
    
    GAME.frameID = 0;
    
    GAME.objSpace.base = GAME_SIZE_X;
    GAME.objSpace.height = GAME_SIZE_Y;
    GAME.objSpace.numOfMobsInBin = MAX_NUMBER_MOBS_BIN;
    GAME.objSpace.numOfTersInBin = MAX_NUMBER_TERRAINS_BIN;
    GAME.objSpace.numOfMainCharsInBin = MAX_NUMBER_MAINCHARACTER_BIN;
    Space_create();
    
    leftKey.pressed = false;
    rightKey.pressed = false;
    upKey.pressed = false;
    downKey.pressed = false;
    spaceKey.pressed = false;
    leftKey.released = false;
    rightKey.released = false;
    upKey.released = false;
    downKey.released = false;
    spaceKey.released = false;
}

void Visible_draw(){
    unsigned i;
    for(i = 0; i<GAME.objVisible.maxNumOfMobs; i++){
        Mob* curMobPtr =  GAME.objVisible.arrMobsPtr[i];
        if(curMobPtr == NULL) continue;
        
        Sprites_draw_mob(*curMobPtr, GAME.topLeft);
    }
    for(i = 0; i<GAME.objVisible.maxNumOfTers; i++){
        Terrain* curTerPtr =  GAME.objVisible.arrTersPtr[i];
        if(GAME.objVisible.arrTersPtr[i] == NULL) continue;
        
        Sprites_draw_terrain(*curTerPtr, GAME.topLeft);
    }
    Sprites_draw_main(GAME.Zero, GAME.topLeft);
}

void update_objects(){
    GAME.frameID++;
    screen_move();
    
    unsigned i,j,k;
    
    //Adds valid objects in the map to the visible vector:
    for(i=GAME.mapLeftIdx; i<=GAME.mapRightIdx; i++){
        for(j=0; j<GAME.mapRightIdx - GAME.mapLeftIdx + 1; j++){
            for(k=0; k<GAME.objMap[j].numOfMobs; k++){ //Loop through mobs that are supposed to be in the screen.
                Mob* curMobPtr =  &GAME.objMap[j].arrMobs[k];
                if(/*!curMobPtr->killed && */!curMobPtr->spawned){
                    curMobPtr->onScreen = true;
                    curMobPtr->spawned = true;
                }
                //else{
                //    curMobPtr->onScreen = false;
                //}
            }
            for(k=0; k<GAME.objMap[j].numOfTers; k++){
                Terrain* curTerPtr =  &GAME.objMap[j].arrTers[k];
                if(!curTerPtr->onScreen){
                    curTerPtr->onScreen = true;
                }
                //else{
                //    
                //}
            }
        }
    }
    
    //Updates what is visible:
    Visible_update();
    
    Space_update();
    
    //All visible will live:
        
    MainChar_move(&GAME.Zero);
    //GAME.Zero.x += GAME.Zero.dx;
    //GAME.Zero.y += GAME.Zero.dy;
    
    for(i = 0; i<GAME.objVisible.maxNumOfMobs; i++){
        Mob* curMobPtr =  GAME.objVisible.arrMobsPtr[i];
        if(curMobPtr == NULL) continue;
        Mob_move(curMobPtr);
        //Check if mob should remain in visible vector:
        if((curMobPtr->x+curMobPtr->spritePtr->b<GAME.topLeft.x || curMobPtr->x>GAME.topLeft.x+SCREEN_X -1) || curMobPtr->killed){ //If it left the screen:
            GAME.objVisible.arrMobsPtr[i]->onScreen = false;
            continue;
        }
    }
    for(i = 0; i<GAME.objVisible.maxNumOfTers; i++){
        Terrain* curTerPtr =  GAME.objVisible.arrTersPtr[i];
        if(GAME.objVisible.arrTersPtr[i] == NULL) continue;
        
        //Check if mob should remain in visible vector
        if((curTerPtr->x+curTerPtr->spritePtr->b<GAME.topLeft.x || curTerPtr->x>GAME.topLeft.x+SCREEN_X -1)){ //If it left the screen:
            GAME.objVisible.arrTersPtr[i]->onScreen = false;
            continue;
        }
    }

}

void screen_move(){
    GAME.topLeft.x = GAME.Zero.x - SCREEN_X/2+1 + (GAME.spritesArr[OBJECT_ID_ZERO].b+1)/2;
    int signedLeftIdx = GAME.topLeft.x/MAP_STEP;
    int signedRightIdx = (GAME.topLeft.x+SCREEN_X-1)/MAP_STEP;
    
    if(signedLeftIdx<0)
        GAME.mapLeftIdx = 0;
    else if (signedLeftIdx>=GAME.mapSize)
         GAME.mapLeftIdx = GAME.mapSize-1;
    else
        GAME.mapLeftIdx = GAME.topLeft.x/MAP_STEP;

    if(signedRightIdx>=GAME.mapSize)
        GAME.mapRightIdx = GAME.mapSize-1;
    else if (signedRightIdx < 0)
         GAME.mapRightIdx = GAME.topLeft.x/MAP_STEP;
    else
        GAME.mapRightIdx = (GAME.topLeft.x+SCREEN_X-1)/MAP_STEP;
    
    
}

void move(){
	//HEX_PS2(byte1, byte2, byte3);
    GAME.Zero.dx = 0;
	if (leftKey.pressed && GAME.Zero.dx > -5){
        GAME.Zero.dx -= 3;
    }
    if (rightKey.pressed && GAME.Zero.dx < 5){
        GAME.Zero.dx += 3;
    }
    
    if (upKey.pressed && GAME.Zero.dy > -2){
        GAME.Zero.dy -= 6;
    }
    if (downKey.pressed && GAME.Zero.dy < +3){
        GAME.Zero.dy = +1;
    }
    if (spaceKey.pressed && GAME.Zero.dy> -2){
        GAME.Zero.dy -= 6;
    }
}