#include "renderer.h"

//1/60 and 1/50 in 20.12 fixed point representation.
fixed syncReciprocal;

Sprite* SpriteTable_Fetch( Renderer* renderer ) {
    if( renderer->spriteTableIndex >= SPRTT_SIZE ) return NULL;
    return &renderer->spriteTable[ renderer->spriteTableIndex++ ];
}

void SpriteTable_Refresh( Renderer* renderer ) {
    //No need to clean the original one, we'll just overwrite it.
    renderer->spriteTableIndex = 0;
}

void Renderer_Init( Renderer* renderer, Color backgroundColor ) {
    renderer->spriteTableIndex = 0;

    ResetGraph( 0 );
    //Initialize two display environments (what should the GPU show).
    SetDefDispEnv( &renderer->dispEnv, 0, 0     , SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize two draw environements (where should the GPU draw).
    SetDefDrawEnv( &renderer->drawEnv, 0, 0     , SCREEN_WIDTH, SCREEN_HEIGHT );

    //Allow us to draw.
    SetDispMask( 1 );

    //Set background color.
    setRGB0( &renderer->drawEnv, backgroundColor.red, backgroundColor.green, backgroundColor.blue );
    renderer->drawEnv.isbg = 1;

    //Set the active display / draw environements.
    PutDispEnv( &renderer->dispEnv );
    PutDrawEnv( &renderer->drawEnv );

    syncReciprocal = GetVideoMode() == MODE_NTSC ? 68 : 82;

    ClearOTagR( (u_long*) &renderer->orderingTable, OT_SIZE );

    //Load the font into (960, 0) in VRAM.
    FntLoad( 960, 0 );
    FntOpen(MARGINX, SCREENYRES - MARGINY - FONTSIZE, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280 );
}

int Renderer_DrawImage( Renderer* renderer, Image* image, int x, int y ) {
    TIM_IMAGE* tim = &image->tim;
    int imageDepth = tim->mode & 0x03;

    //Rendering works with DWords (16 bits), not with pixels.
    //So when you have a 4 bit picture, a single DWord holds 4 pixels, and with 8 bit it's 2 pixels.
    int pixelPerWord = Is4Bit( image ) ? 4 : Is8Bit( image ) ? 2 : 1;
    int wordPerTPage = Is4Bit( image ) ? 64 : Is8Bit( image ) ? 128 : 256;

    //Used for bitshifting (optimization).
    int bitsPerTPage = Is4Bit( image ) ? 6 : Is8Bit( image ) ? 7 : 8;

    //Find out the TPages where the image spans in VRam on both axis.
    int startPageX = tim->prect->x >> bitsPerTPage; 
    int endPageX = ( tim->prect->x + tim->prect->w - 1 ) >> bitsPerTPage;
    int startPageY = tim->prect->y >> 8;
    int endPageY = ( tim->prect->y + tim->prect->h - 1 ) >> 8;

    //Use the TPages that the image spans to find out the amount of TPages on both axis.
    int tpageCountX = endPageX - startPageX + 1;
    int tpageCountY = endPageY - startPageY + 1;

    int drawY = y;
    int currentVramY = tim->prect->y;
    int remainingHeight = tim->prect->h;

    for( int j = 0; j < tpageCountY; j++) {
        int drawX = x;
        int currentVramX = tim->prect->x;
        int remainingWidth = tim->prect->w;

        int verticalOffset = currentVramY & 0xFF;
        int sliceHeight = 256 - verticalOffset;
        if( sliceHeight > remainingHeight ) sliceHeight = remainingHeight;
        
        for( int i = 0; i < tpageCountX; i++ ) {
            Sprite* spriteObj = SpriteTable_Fetch( renderer );
            if( spriteObj == NULL ) return -1;

            int horizontalOffset = currentVramX & ( wordPerTPage - 1 );
            int sliceWidth = wordPerTPage - horizontalOffset;
            if( sliceWidth > remainingWidth ) sliceWidth = remainingWidth;

            SPRT* sprite = &spriteObj->sprt;
            DR_TPAGE* TPage = &spriteObj->tPage;

            SetSprt( sprite );
            setXY0( sprite, drawX, drawY );
            setWH( sprite, sliceWidth * pixelPerWord, sliceHeight );
            setUV0( sprite, ( currentVramX & ( wordPerTPage - 1 ) ) * pixelPerWord, currentVramY & 0xFF );
            setRGB0( sprite, 128, 128, 128 );

            //Retrieve the CLUTs if 4-bit / 8-bit.
            if( IsIndexed( image ) ) {
                setClut( sprite, image->tim.crect->x, tim->crect->y );
            }
            
            int TPageX = ( currentVramX >> bitsPerTPage ) << bitsPerTPage;
            int TpageY = ( currentVramY >> 8 ) << 8;

            setDrawTPage( TPage, 0, 1, getTPage( imageDepth, 0, TPageX, TpageY ) );

            addPrim( renderer->orderingTable, sprite );
            addPrim( renderer->orderingTable, TPage );

            currentVramX += sliceWidth;
            remainingWidth -= sliceWidth;
            drawX += sliceWidth * pixelPerWord;
        }

        currentVramY += sliceHeight;
        remainingHeight -= sliceHeight;
        drawY += sliceHeight;
    }
} 

int Renderer_DrawActor( Renderer* renderer, Actor* actor ) {
    Renderer_DrawImage( renderer, &actor->texture[ actor->activeBuffer ], actor->actorInfo.x, 0 );
}

int Renderer_DrawScene( Renderer* renderer, Scene* scene ) {
    switch( scene->sceneInfo.specialEffect ) {
        case SnowFall:
            Renderer_DrawSnow( renderer, &scene->specialEffect, scene->specialEffectProperties );
            break;

        default:
            break;
    };

    for( int i = 0; i < scene->sceneInfo.actorCount; i++ ) {
        Renderer_DrawActor( renderer, &scene->actors[i] );
    }
    Renderer_DrawImage( renderer, &scene->background, 0, 0 );
}

void Renderer_Refresh( Renderer* renderer ) {
    //Draw everything on the ordering table.
    DrawOTag( &renderer->orderingTable[OT_SIZE - 1] );
    //Output the text printed with FntPrint.
    FntFlush( -1 );

    //Wait till everything has been drawn.
    DrawSync( 0 );
    //Wait for vertical blank.
    VSync( 0 );

    //Set the active display / draw environements.
    PutDispEnv( &renderer->dispEnv );
    PutDrawEnv( &renderer->drawEnv );

    //Clear the ordering table.
    ClearOTagR( (u_long*) &renderer->orderingTable, OT_SIZE );

    SpriteTable_Refresh( renderer );
}