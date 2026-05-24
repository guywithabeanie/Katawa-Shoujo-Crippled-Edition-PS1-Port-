#include "renderer.h"

fixed syncReciprocal;

Texture *TextureTable_Fetch(Renderer *renderer) {
  if (renderer->spriteTableIndex >= TEXTABLE_SIZE)
    return NULL;
  return &renderer->textureTable[renderer->spriteTableIndex++];
}

void TextureTable_Refresh(Renderer *renderer) {
  // No need to clean the original one, we'll just overwrite it.
  renderer->spriteTableIndex = 0;
}

void Renderer_Init(Renderer *renderer, Color backgroundColor) {
  renderer->spriteTableIndex = 0;

  ResetGraph(0);
  // Initialize two display environments (what should the GPU show).
  SetDefDispEnv(&renderer->dispEnv, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // Initialize two draw environements (where should the GPU draw).
  SetDefDrawEnv(&renderer->drawEnv, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // Allow us to draw.
  SetDispMask(1);

  // Set background color.
  setRGB0(&renderer->drawEnv, backgroundColor.red, backgroundColor.green,
          backgroundColor.blue);
  renderer->drawEnv.isbg = 1;

  // Set the active display / draw environements.
  PutDispEnv(&renderer->dispEnv);
  PutDrawEnv(&renderer->drawEnv);

  // 1/60 or 1/50 in 20.12 fixed point representation.
  syncReciprocal = GetVideoMode() == MODE_NTSC ? 68 : 82;

  ClearOTagR((u_long *)&renderer->orderingTable, OT_SIZE);

  // Load the font into (960, 0) in VRAM.
  FntLoad(960, 0);
  FntOpen(MARGINX, SCREEN_WIDTH - MARGINY - FONTSIZE,
          SCREEN_HEIGHT - MARGINX * 2, FONTSIZE, 0, 280);
}

void Renderer_Refresh(Renderer *renderer) {
  // Draw everything on the ordering table.
  DrawOTag(&renderer->orderingTable[OT_SIZE - 1]);
  // Output the text printed with FntPrint.
  FntFlush(-1);

  // Wait till everything has been drawn.
  DrawSync(0);
  // Wait for vertical blank.
  VSync(0);

  // Set the active display / draw environements.
  PutDispEnv(&renderer->dispEnv);
  PutDrawEnv(&renderer->drawEnv);

  // Clear the ordering table.
  ClearOTagR((u_long *)&renderer->orderingTable, OT_SIZE);

  TextureTable_Refresh(renderer);
}
