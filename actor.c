#include "actor.h"

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

int Renderer_DrawActor( Renderer* renderer, Actor* actor ) {
    Renderer_DrawImage( renderer, &actor->texture[ actor->activeBuffer ], actor->actorInfo.x, 0, DEFAULT, Opaque );
}
