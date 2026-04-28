#ifndef SPECIAL_EFFECTS_H
#define SPECIAL_EFFECTS_H

#include <stdlib.h>

#include "renderer.h"
#include "scene.h"
#include "fixed.h"

#define FX_X 960
#define FX_Y 256
#define FX_CLUTX 0
#define FX_CLUTY 511

#define SnowFall 0
#define SNOW_PATH "\\GFX\\VFX\\SNOW.TIM;1"
#define SNOW_COUNT 30

typedef struct Renderer Renderer;

typedef struct SnowFlake {
    fixed x, y;
    int velocityX, velocityY;
} SnowFlake;

typedef struct SnowSystem {
    SnowFlake snow[SNOW_COUNT];
} SnowSystem;

int SnowSystem_Init( SnowSystem* snowSystem );
int SnowSystem_Update( SnowSystem* snowSystem );
int Renderer_DrawSnow( Renderer* renderer, Image* snowSprite, SnowSystem* snowSystem );

#endif