#ifndef RENDERER_H
#define RENDERER_H

#include <sys/types.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <string.h>

#include "fixed.h"

#define MARGINX 0                // margins for text display
#define MARGINY 32
#define FONTSIZE 8 * 7           // Text Field Height

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

#define DEFAULT (Color){ 128, 128, 128 }

#define OT_SIZE 128
#define TEXTABLE_SIZE 128

#define SyncToRefresh(n) ( Fixed_MultIntF(n, syncReciprocal) )

//The reciprocal of the refresh rate (60Hz for NTSC, 50Hz for PAL).
extern fixed syncReciprocal;

typedef struct Texture {
    SPRT sprt;
    DR_TPAGE tPage;
} Texture;

typedef struct Renderer {
    int spriteTableIndex;

    DISPENV dispEnv;
    DRAWENV drawEnv;

    u_long orderingTable[OT_SIZE];
    Texture textureTable[TEXTABLE_SIZE];
} Renderer;

typedef struct Color {
    u_char red, green, blue;
} Color;

//The transparency modes that the PS1 supports.
typedef enum TransparencyMode {
    Opaque = 0,
    Translucent,                //50% Background  + 50% Foreground
    Additive,                   //100% Background + 100% Foreground
    Substractive,               //100% Background - 100% Foreground
    Additive25                  //100% Background + 25% Foreground
} TransparencyMode;

Texture* TextureTable_Fetch( Renderer* renderer );

void TextureTable_Refresh( Renderer* renderer );

void Renderer_Init( Renderer* renderer, Color backgroundColor );

void Renderer_Refresh( Renderer* renderer );

#endif