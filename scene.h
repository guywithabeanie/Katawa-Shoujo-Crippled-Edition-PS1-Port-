#ifndef SCENE_H
#define SCENE_H

#include "image.h"
#include "effects.h"
#include "texLut.h"
#include "actor.h"
#include "renderer.h"

#define MAX_ACTORS 4

typedef struct Image Image;
typedef struct Actor Actor;
typedef struct ActorInfo ActorInfo;

typedef struct SceneInfo {
    int background;
    int actorCount;
    Effect effect;
} SceneInfo;

typedef struct Scene {
    void* effectProperties;
    SceneInfo sceneInfo;
    Actor actors[MAX_ACTORS];

    Image background;
    Image specialEffect;
} Scene;

int Scene_Init( Scene* scene, SceneInfo* sceneInfo, ActorInfo* actorInfo );

int Scene_AddActor( Scene* scene, ActorInfo* actorInfo );

int Scene_RemoveActor( Scene* scene, int character );

int Scene_Update( Scene* scene );

int Renderer_DrawScene( Renderer* renderer, Scene* scene );

#endif