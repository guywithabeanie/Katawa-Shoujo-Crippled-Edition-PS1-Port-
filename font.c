#include "font.h"

Status Font_Load(Font *font, char *fontTexturePath, FontData *fontData) {
  font->fontData = fontData;

  Status loadStatus = Image_Load(&font->fontImage, fontTexturePath, FONTVRAM_X,
                                 FONTVRAM_Y, None);

  return loadStatus;
}

// Add typewriter esque writing.
int Renderer_DrawText(Renderer *renderer, Font *font, char *text, int x, int y,
                      Color tint, int displayType) {
  static int currentStringLength = 1;

  int imageDepth = font->fontImage.mode & 0x03;

  // Rendering works with DWords (16 bits), not with pixels.
  // So when you have a 4 bit picture, a single DWord holds 4 pixels, and with 8
  // bit it's 2 pixels.
  int pixelPerWord = Is4Bit(&font->fontImage)   ? 4
                     : Is8Bit(&font->fontImage) ? 2
                                                : 1;
  int wordPerTPage = Is4Bit(&font->fontImage)   ? 64
                     : Is8Bit(&font->fontImage) ? 128
                                                : 256;

  // Used for bitshifting (optimization).
  int bitsPerTPage = Is4Bit(&font->fontImage)   ? 6
                     : Is8Bit(&font->fontImage) ? 7
                                                : 8;

  int stringLength = strlen(text);

  int drawX = x;
  int drawY = y;

  int imageOffsetX = FONTVRAM_X & bitsPerTPage;
  int imageOffsetY = FONTVRAM_Y & 0xFF;

  // Fetch a TPage and use it for all the subsequent draws.
  DR_TPAGE *tPage = &TextureTable_Fetch(renderer)->tPage;

  int tPageX = (font->fontImage.prect.x >> bitsPerTPage) << bitsPerTPage;
  int tPageY = (font->fontImage.prect.y >> 8) << 8;

  setDrawTPage(tPage, 0, 1, getTPage(imageDepth, Additive, tPageX, tPageY));

  if (displayType == TYPEWRITER) {
    if (currentStringLength < stringLength)
      currentStringLength++;
  } else
    currentStringLength = stringLength;

  for (int i = 0; i < currentStringLength; i++) {
    char currentCharacter = text[i];

    switch (currentCharacter) {
    case ' ':
      drawX += 3;
      if (drawX > SCREEN_WIDTH)
        drawX = x;
      continue;

    case '\n':
      drawX = x;
      drawY += font->fontData->charHeight + 2;
      continue;

    default:
      break;
    };

    int currentCharacterIndex = font->fontData->indices[currentCharacter - ' '];
    CharData *currentCharactersData =
        &font->fontData->charData[currentCharacterIndex];

    if (drawX >= SCREEN_WIDTH) {
      drawX = x;
      drawY += font->fontData->charHeight + 2;
    }

    Texture *texture = TextureTable_Fetch(renderer);

    SPRT *sprite = &texture->sprt;

    setSprt(sprite);
    setSemiTrans(sprite, 1);
    setXY0(sprite, drawX, drawY);
    setUV0(sprite, currentCharactersData->u + imageOffsetX,
           currentCharactersData->v + imageOffsetY);
    setWH(sprite, currentCharactersData->w, font->fontData->charHeight);
    setRGB0(sprite, tint.red, tint.green, tint.blue);

    if (IsIndexed(&font->fontImage))
      setClut(sprite, font->fontImage.crect.x, font->fontImage.crect.y);

    addPrim(renderer->orderingTable, sprite);

    drawX += currentCharactersData->w;
  }

  addPrim(renderer->orderingTable, tPage);
  return 0;
}
