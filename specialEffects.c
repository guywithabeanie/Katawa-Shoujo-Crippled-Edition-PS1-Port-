#include "specialEffects.h"

//Make snow texture normal lookin.
//Make a git repo.
int SnowSystem_Init( SnowSystem* snowSystem ) {
    for( int i = 0; i < SNOW_COUNT; i++ ) {
        SnowFlake* snowFlake = &snowSystem->snow[i];

        snowFlake->x = Fixed( rand() % SCREEN_WIDTH );
        snowFlake->y = Fixed( rand() % SCREEN_HEIGHT );

        snowFlake->velocityX = ( rand() % 10 ) + 10;
        snowFlake->velocityY = ( rand() % 30 ) + 20;
    }
}

int SnowSystem_Update( SnowSystem* snowSystem ) {
    for( int i = 0; i < SNOW_COUNT; i++ ) {
        SnowFlake* snowFlake = &snowSystem->snow[i];

        snowFlake->x += SyncToRefresh( snowFlake->velocityX );
        snowFlake->y += SyncToRefresh( snowFlake->velocityY );

        if( snowFlake->x > Fixed( SCREEN_WIDTH ) ) snowFlake->x = 0;
        if( snowFlake->y > Fixed( SCREEN_HEIGHT ) ) snowFlake->y = 0;
    }
}

int Renderer_DrawSnow( Renderer* renderer, Image* snowSprite, SnowSystem* snowSystem ) {
    for( int i = 0; i < SNOW_COUNT; i++ ) {
        SnowFlake* snowFlake = &snowSystem->snow[i];
        Renderer_DrawImage( renderer, snowSprite, Ftoi( snowFlake->x ), Ftoi( snowFlake->y ) );
    }
}