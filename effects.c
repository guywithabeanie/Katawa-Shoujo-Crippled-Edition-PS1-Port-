#include "effects.h"

int SnowSystem_Init( SnowSystem* snowSystem, Image* snowSprite ) {
    snowSystem->snowSprite = snowSprite;

    for( int i = 0; i < SNOW_COUNT; i++ ) {
        SnowFlake* snowFlake = &snowSystem->snow[i];

        snowFlake->x = itoF( rand() % SCREEN_WIDTH );
        snowFlake->y = itoF( rand() % SCREEN_HEIGHT );

        snowFlake->velocityX = ( rand() % 10 ) + 10;
        snowFlake->velocityY = ( rand() % 30 ) + 20;
    }
}

int SnowSystem_Update( SnowSystem* snowSystem ) {
    for( int i = 0; i < SNOW_COUNT; i++ ) {
        SnowFlake* snowFlake = &snowSystem->snow[i];

        snowFlake->x += SyncToRefresh( snowFlake->velocityX );
        snowFlake->y += SyncToRefresh( snowFlake->velocityY );

        //Wrap around the screen.
        if( snowFlake->x > itoF( SCREEN_WIDTH ) ) snowFlake->x = itoF( -snowSystem->snowSprite->prect.w );
        if( snowFlake->y > itoF( SCREEN_HEIGHT ) ) snowFlake->y = itoF( -snowSystem->snowSprite->prect.h );
    }
}

int Renderer_DrawSnow( Renderer* renderer, SnowSystem* snowSystem ) {
    for( int i = 0; i < SNOW_COUNT; i++ ) {
        SnowFlake* snowFlake = &snowSystem->snow[i];
        Renderer_DrawImage ( 
            renderer, snowSystem->snowSprite, 
            Ftoi( snowFlake->x ), Ftoi( snowFlake->y ), 
            DEFAULT, Additive 
        );
    }
}