#ifndef ACTOR_H
#define ACTOR_H

#include "renderer.h"
#include "image.h"
#include "texLut.h"

typedef struct ActorInfo {
    //Actors don't need to move through the Y axis all that much.
    int x;
    int character;
    int pose;
} ActorInfo;

typedef struct Actor {
    //Use two images for when switching between two states.
    int activeBuffer;

    ActorInfo actorInfo;
    Image texture[2];
} Actor;

int Actor_Init( Actor* actor, ActorInfo* actorInfo );

int Renderer_DrawActor( Renderer* renderer, Actor* actor );

#endif