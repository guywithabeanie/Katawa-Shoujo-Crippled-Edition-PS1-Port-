#ifndef SCENE_H
#define SCENE_H

#include "image.h"
#include "specialEffects.h"
#include "texLut.h"

#define MAX_ACTORS 4

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

typedef struct SceneInfo {
    int background;
    int actorCount;
    int specialEffect;
} SceneInfo;

typedef struct Scene {
    void* specialEffectProperties;
    SceneInfo sceneInfo;
    Actor actors[MAX_ACTORS];

    Image background;
    Image specialEffect;
} Scene;

int Actor_Init( Actor* actor, ActorInfo* actorInfo );

int Actor_Draw( Actor* actor );

int Scene_Init( Scene* scene, SceneInfo* sceneInfo, ActorInfo* actorInfo );

int Scene_AddActor( Scene* scene, ActorInfo* actorInfo );

int Scene_RemoveActor( Scene* scene, int character );

int Scene_Update( Scene* scene );

#endif