#ifndef RENDERER_H
#define RENDERER_H

#include <sys/types.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <string.h>

#include "image.h"
#include "scene.h"
#include "fixed.h"

#define MARGINX 0                // margins for text display
#define MARGINY 32
#define SCREENXRES 320          // Screen width
#define SCREENYRES 240          // Screen height
#define FONTSIZE 8 * 7           // Text Field Height

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

#define OT_SIZE 128
#define SPRTT_SIZE 128

#define SyncToRefresh(n) ( Fixed_MultIntF(n, syncReciprocal) )

typedef struct Actor Actor;
typedef struct Scene Scene;

//The reciprocal of the refresh rate (60Hz for NTSC, 50Hz for PAL).
extern fixed syncReciprocal;

typedef struct Sprite {
    SPRT sprt;
    DR_TPAGE tPage;
} Sprite;

typedef struct Renderer {
    int spriteTableIndex;

    DISPENV dispEnv;
    DRAWENV drawEnv;

    u_long orderingTable[OT_SIZE];
    Sprite spriteTable[SPRTT_SIZE];
} Renderer;

typedef struct Color {
    u_char red, green, blue;
} Color;

Sprite* SpriteTable_Fetch( Renderer* renderer );

void SpriteTable_Refresh( Renderer* renderer );

void Renderer_Init( Renderer* renderer, Color backgroundColor );

int Renderer_DrawImage( Renderer* renderer, Image* image, int x, int y );

int Renderer_DrawActor( Renderer* renderer, Actor* actor );

int Renderer_DrawScene( Renderer* renderer, Scene* scene );

void Renderer_Refresh( Renderer* renderer );

#endif