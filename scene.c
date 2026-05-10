#include "scene.h"

Status Scene_Init( Scene* scene, SceneInfo* sceneInfo, ActorInfo* actorInfo ) {
    //Add stuff to add / remove actors and shi without manually doing shi.
    scene->sceneInfo = *sceneInfo;

    Status loadStatus = Image_Load ( 
        &scene->background, Backgrounds[ sceneInfo->background ], AUTO, AUTO, AUTO, AUTO 
    );
    if( loadStatus != Okay ) return loadStatus;

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
    Renderer_DrawImage( renderer, &scene->background, 0, 0, DEFAULT, Opaque );
}