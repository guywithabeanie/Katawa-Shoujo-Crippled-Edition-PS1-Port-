#ifndef EFFECTS_H
#define EFFECTS_H

#include <stdlib.h>

#include "renderer.h"
#include "image.h"
#include "fixed.h"

//Effects will always get drawn in this VRAM position.
#define FX_X 960
#define FX_Y 256
#define FX_CLUTX 0
#define FX_CLUTY 511

#define SNOW_COUNT 30

typedef struct SnowFlake {
    fixed x, y;
    int velocityX, velocityY;
} SnowFlake;

typedef struct SnowSystem {
    Image* snowSprite;
    SnowFlake snow[SNOW_COUNT];
} SnowSystem;

int SnowSystem_Init( SnowSystem* snowSystem, Image* snowSprite );
int SnowSystem_Update( SnowSystem* snowSystem );
int Renderer_DrawSnow( Renderer* renderer, SnowSystem* snowSystem );

#endif