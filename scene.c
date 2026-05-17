#include "scene.h"

Status Scene_Init( Scene* scene, SceneInfo* sceneInfo, ActorInfo* actorInfo ) {
    //Add stuff to add / remove actors and shi without manually doing shi.
    scene->sceneInfo = *sceneInfo;

    Status loadStatus = Image_Load ( 
        &scene->backgroundTexture, Backgrounds[ sceneInfo->background ], AUTO, AUTO, AUTO, AUTO 
    );
    if( loadStatus != Okay ) return loadStatus;

    loadStatus = Font_Load( &scene->font, PLAYTIME_PATH, &playtimeFontData );
    if( loadStatus != Okay ) return loadStatus;

    switch( sceneInfo->barType ) {
        case NormalBar:
            loadStatus = Image_Load( &scene->barTexture, NORMALBAR_PATH, AUTO, AUTO, AUTO, AUTO );
            break;
        
        case CharBar:
            loadStatus = Image_Load( &scene->barTexture, CHARBAR_PATH, AUTO, AUTO, AUTO, AUTO );

        default:
            break;
    }

    switch( sceneInfo->effect ) {
        case SnowFall:
            scene->effectProperties = malloc( sizeof( SnowSystem ) );
            if( scene->effectProperties == NULL ) return MallocError;

            Status loadStatus = Image_Load ( 
                &scene->effectTexture, Effects[ sceneInfo->effect - 1 ], FX_X, FX_Y, FX_CLUTX, FX_CLUTY 
            );
            if( loadStatus != Okay ) return loadStatus;

            SnowSystem_Init( scene->effectProperties, &scene->effectTexture );
            break;
        
        default:
            break;
    }

    if( sceneInfo->actorCount == 0 ) return 0;
    else if( sceneInfo->actorCount == NULL ) return -1;

    for( int i = 0; i < sceneInfo->actorCount; i++ ) Actor_Init( &scene->actors[i], &actorInfo[i] );
}

int Scene_AddActor( Scene* scene, ActorInfo* actorInfo ) {
    Actor_Init( &scene->actors[ scene->sceneInfo.actorCount++ ], actorInfo );
}

int Scene_Update( Scene* scene ) {
    switch( scene->sceneInfo.effect ) {
        case SnowFall:
            SnowSystem_Update( scene->effectProperties );
            break;

        default:
            break;
    }
}

int Scene_Destroy( Scene* scene ) {
    if( scene->effectProperties != NULL ) free( scene->effectProperties );
}

int Renderer_DrawScene( Renderer* renderer, Scene* scene ) {
    int barX = 0;
    int barY = SCREEN_HEIGHT - scene->barTexture.prect.h - 10;

    int textX, textY;

    if( scene->sceneInfo.barType != CharBar ) {
        textX = barX + 5;
        textY = barY + 9;
    }
    else {
        textX = barX + 9;
        textY = barY + 37;
    }

    Renderer_DrawText( renderer, &scene->font, scene->sceneInfo.text, textX, textY, DEFAULT, TYPEWRITER );
    Renderer_DrawImage( renderer, &scene->barTexture, barX, barY, DEFAULT, Average );

    switch( scene->sceneInfo.effect ) {
        case SnowFall:
            Renderer_DrawSnow( renderer, scene->effectProperties );
            break;

        default:
            break;
    };

    for( int i = 0; i < scene->sceneInfo.actorCount; i++ ) {
        Renderer_DrawActor( renderer, &scene->actors[i] );
    }
    Renderer_DrawImage( renderer, &scene->backgroundTexture, 0, 0, DEFAULT, Opaque );
}