#ifndef SCENE_H
#define SCENE_H

#include "effects.h"
#include "texLut.h"
#include "actor.h"
#include "renderer.h"
#include "font.h"
#include "fontData.h"

#define MAX_ACTORS 4

typedef struct SceneInfo {
    char* text;
    int actorCount;

    Background background;
    Effect effect;
} SceneInfo;

typedef struct Scene {
    void* effectProperties;
    SceneInfo sceneInfo;
    Actor actors[MAX_ACTORS];

    Image background;
    Image effectTexture;
    Font font;
} Scene;

Status Scene_Init( Scene* scene, SceneInfo* sceneInfo, ActorInfo* actorInfo );

int Scene_AddActor( Scene* scene, ActorInfo* actorInfo );

int Scene_RemoveActor( Scene* scene, int character );

int Scene_Update( Scene* scene );

int Scene_Destroy( Scene* scene );

int Renderer_DrawScene( Renderer* renderer, Scene* scene );

#endif