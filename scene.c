#include "scene.h"
#include "font.h"
#include "image.h"
#include "renderer.h"
#include "timer.h"

Status Scene_Init(Scene *scene, SceneInfo *sceneInfo, ActorInfo *actorInfo) {
  // Add stuff to add / remove actors and shi without manually doing shi.
  scene->sceneInfo = *sceneInfo;
  scene->stringCounter = 0;
  scene->continueAlpha = 0;

  Status loadStatus =
      Image_Load(&scene->backgroundTexture, Backgrounds[sceneInfo->background],
                 AUTO, AUTO, Left);
  if (loadStatus != Okay)
    return loadStatus;

  loadStatus = Font_Load(&scene->font, PLAYTIME_PATH, &playtimeFontData);
  if (loadStatus != Okay)
    return loadStatus;

  switch (sceneInfo->barType) {
  case NormalBar:
    loadStatus =
        Image_Load(&scene->barTexture, NORMALBAR_PATH, AUTO, AUTO, Right);
    break;

  case CharBar:
    loadStatus =
        Image_Load(&scene->barTexture, CHARBAR_PATH, AUTO, AUTO, Right);

    Image_Load(&scene->continueTexture, CONTINUE_PATH, CONTINUE_VRAMX,
               CONTINUE_VRAMY, None);

  default:
    break;
  }

  switch (sceneInfo->effect) {
  case SnowFall:
    scene->effectProperties = malloc(sizeof(SnowSystem));
    if (scene->effectProperties == NULL)
      return MallocError;

    Status loadStatus =
        Image_Load(&scene->effectTexture, Effects[sceneInfo->effect - 1], FX_X,
                   FX_Y, None);
    if (loadStatus != Okay)
      return loadStatus;

    SnowSystem_Init(scene->effectProperties, &scene->effectTexture);
    break;

  default:
    break;
  }

  if (sceneInfo->actorCount == 0)
    return 0;
  else if (sceneInfo->actorCount == NULL)
    return -1;

  for (int i = 0; i < sceneInfo->actorCount; i++)
    Actor_Init(&scene->actors[i], &actorInfo[i]);

  return Okay;
}

int Scene_AddActor(Scene *scene, ActorInfo *actorInfo) {
  Actor_Init(&scene->actors[scene->sceneInfo.actorCount++], actorInfo);
  return 0;
}

int Scene_Update(Scene *scene) {
  switch (scene->sceneInfo.effect) {
  case SnowFall:
    SnowSystem_Update(scene->effectProperties);
    break;

  default:
    break;
  }
  return 0;
}

int Scene_Destroy(Scene *scene) {
  if (scene->effectProperties != NULL)
    free(scene->effectProperties);

  return 0;
}

int Renderer_DrawScene(Renderer *renderer, Scene *scene) {
  int barX = 0;
  int barY = SCREEN_HEIGHT - scene->barTexture.prect.h - 10;

  int textX, textY;

  if (scene->sceneInfo.barType != CharBar) {
    textX = barX + 5;
    textY = barY + 9;
  } else {
    textX = barX + 9;
    textY = barY + 42;
  }

  int textFinished =
      Renderer_DrawText(renderer, &scene->font, scene->sceneInfo.text, textX,
                        textY, DEFAULT, TYPEWRITER, &scene->stringCounter);

  // Draw the continue icon if the text is done writing.
  static int continueAlphaIncrement = 2;
  static Timer continueTimer;

  if (textFinished) {
    // Clamp the alpha.
    if (scene->continueAlpha >= 128)
      scene->continueAlpha = 128;
    else if (scene->continueAlpha <= 0)
      scene->continueAlpha = 0;

    Color alpha = (Color){scene->continueAlpha, scene->continueAlpha,
                          scene->continueAlpha};

    Renderer_DrawImage(renderer, &scene->continueTexture, CONTINUE_X,
                       CONTINUE_Y, alpha, Additive);

    scene->continueAlpha += continueAlphaIncrement;

    if (scene->continueAlpha <= 0)
      continueAlphaIncrement = 2;

    // Pause the blinking for half a second.
    else if (scene->continueAlpha >= 128 && !continueTimer.counting) {
      continueAlphaIncrement = 0;
      Timer_Start(&continueTimer, tickPerSecond >> 2);
    }

    TimerStatus timerStatus = Timer_Update(&continueTimer);
    if (timerStatus == Finished) {
      continueAlphaIncrement = -2;
    }
  }

  // Reset.
  else {
    scene->continueAlpha = 0;
    continueAlphaIncrement = 2;
  }

  Renderer_DrawImage(renderer, &scene->barTexture, barX, barY, DEFAULT,
                     Average);

  // Draw effects.
  switch (scene->sceneInfo.effect) {
  case SnowFall:
    Renderer_DrawSnow(renderer, scene->effectProperties);
    break;

  default:
    break;
  };

  // Draw actors and background.
  for (int i = 0; i < scene->sceneInfo.actorCount; i++) {
    Renderer_DrawActor(renderer, &scene->actors[i]);
  }
  Renderer_DrawImage(renderer, &scene->backgroundTexture, 0, 0, DEFAULT,
                     Opaque);

  return 0;
}
