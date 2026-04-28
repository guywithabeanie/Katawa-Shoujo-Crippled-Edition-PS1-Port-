#include "scene.h"

int Actor_Init( Actor* actor, ActorInfo* actorInfo ) {
    actor->actorInfo = *actorInfo;   actor->activeBuffer = 0;

    switch( actorInfo->character ) {
        case Rin: {
            Image* image = &actor->texture[ actor->activeBuffer ];
            Image_Load( image, RinPoses[ actorInfo->pose ], AUTO, AUTO, AUTO, AUTO );
            break;
        }
        default:
            return -1;
            break;
    };
}

int Scene_Init( Scene* scene, SceneInfo* sceneInfo, ActorInfo* actorInfo ) {
    //Add stuff to add / remove actors and shi without manually doing shi.
    scene->sceneInfo = *sceneInfo;

    Image_Load( &scene->background, Backgrounds[ sceneInfo->background ], AUTO, AUTO, AUTO, AUTO );

    switch( sceneInfo->specialEffect ) {
        case SnowFall:
            scene->specialEffectProperties = malloc( sizeof( SnowSystem ) );
            Image_Load( &scene->specialEffect, SNOW_PATH, FX_X, FX_Y, FX_CLUTX, FX_CLUTY );
            SnowSystem_Init( scene->specialEffectProperties );
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
    switch( scene->sceneInfo.specialEffect ) {
        case SnowFall:
            SnowSystem_Update( scene->specialEffectProperties );
            break;

        default:
            break;
    }
}