#include "image.h"
#include "error.h"
#include "libgpu.h"

static VRAM_Allocator vramStack = {.start = 320, .end = 960};
static int clutStack = 480;

VRAM_Position VRAM_Alloc(TIM_IMAGE *image, Direction direction) {
  VRAM_Position output;

  switch (direction) {
  case Left: {
    if (vramStack.start + image->prect->w > vramStack.end)
      return (VRAM_Position){-1, -1};

    output = (VRAM_Position){vramStack.start, 0};
    vramStack.start += image->prect->w;
    break;
  }
  case Right: {
    if (vramStack.end - image->prect->w < vramStack.start)
      return (VRAM_Position){-1, -1};

    vramStack.end -= image->prect->w;
    output = (VRAM_Position){vramStack.end, 0};
    break;
  }
  default:
    return (VRAM_Position){0, 0};
  }

  return output;
}

Status Image_Load(Image *image, char *path, int imageVX, int imageVY,
                  Direction direction) {
  // Open the TIM file.
  File fp;
  int status = File_Open(&fp, path);
  if (status == NULL) {
    return FileLoadError;
  }

  // Allocate space for the TIM file (must allocate by sector).
  int sectorCount = SectorToByte(ByteToSector(fp.size));
  char *buffer = malloc(sectorCount);
  if (buffer == NULL) {
    return MallocError;
  }

  // Read the file.
  Status readStatus = File_Read(&fp, buffer, WHOLE_FILE);
  if (readStatus != Okay) {
    return FileReadError;
  }

  TIM_IMAGE tim;
  // Get the information out of the TIM file.
  OpenTIM((u_long *)buffer);
  ReadTIM(&tim);

  if (imageVX == AUTO || imageVY == AUTO) {
    VRAM_Position vramPosition = VRAM_Alloc(&tim, direction);
    if (vramPosition.x == -1 && vramPosition.y == -1) {
      return VRAMError;
    }
    tim.prect->x = vramPosition.x;
    tim.prect->y = vramPosition.y;
  } else {
    tim.prect->x = imageVX;
    tim.prect->y = imageVY;
  }

  tim.crect->x = 0;
  tim.crect->y = clutStack++;

  image->mode = tim.mode;
  image->prect = *tim.prect;
  image->crect = *tim.crect;

  // Load the pixel data into VRAM.
  LoadImage(tim.prect, tim.paddr);
  DrawSync(0);

  // If it uses CLUTs, also load them to VRAM.
  if (IsIndexed(image)) {
    LoadImage(tim.crect, tim.caddr);
    DrawSync(0);
  }

  // clean
  free(buffer);

  return Okay;
}

int Renderer_DrawImage(Renderer *renderer, Image *image, int x, int y,
                       Color tint, TransparencyMode transparencyMode) {
  int imageDepth = image->mode & 0x03;

  // Rendering works with DWords (16 bits), not with pixels.
  // So when you have a 4 bit picture, a single DWord holds 4 pixels, and with
  // 8 bit it's 2 pixels.
  int pixelPerWord = Is4Bit(image) ? 4 : Is8Bit(image) ? 2 : 1;
  int wordPerTPage = Is4Bit(image) ? 64 : Is8Bit(image) ? 128 : 256;

  // Used for bitshifting (optimization).
  int bitsPerTPage = Is4Bit(image) ? 6 : Is8Bit(image) ? 7 : 8;

  // Find out the TPages where the image spans in VRam on both axis.
  int startPageX = image->prect.x >> bitsPerTPage;
  int endPageX = (image->prect.x + image->prect.w - 1) >> bitsPerTPage;
  int startPageY = image->prect.y >> 8;
  int endPageY = (image->prect.y + image->prect.h - 1) >> 8;

  // Use the TPages that the image spans to find out the amount of TPages on
  // both axis.
  int tpageCountX = endPageX - startPageX + 1;
  int tpageCountY = endPageY - startPageY + 1;

  int drawY = y;
  int currentVramY = image->prect.y;
  int remainingHeight = image->prect.h;

  for (int j = 0; j < tpageCountY; j++) {
    int drawX = x;
    int currentVramX = image->prect.x;
    int remainingWidth = image->prect.w;

    int verticalOffset = currentVramY & 0xFF;
    int sliceHeight = 256 - verticalOffset;
    if (sliceHeight > remainingHeight)
      sliceHeight = remainingHeight;

    for (int i = 0; i < tpageCountX; i++) {
      Texture *TextureObj = TextureTable_Fetch(renderer);
      if (TextureObj == NULL)
        return -1;

      int horizontalOffset = currentVramX & (wordPerTPage - 1);
      int sliceWidth = wordPerTPage - horizontalOffset;
      if (sliceWidth > remainingWidth)
        sliceWidth = remainingWidth;

      SPRT *Texture = &TextureObj->sprt;
      DR_TPAGE *TPage = &TextureObj->tPage;

      SetSprt(Texture);
      setXY0(Texture, drawX, drawY);
      setWH(Texture, sliceWidth * pixelPerWord, sliceHeight);
      setUV0(Texture, (currentVramX & (wordPerTPage - 1)) * pixelPerWord,
             currentVramY & 0xFF);
      setRGB0(Texture, tint.red, tint.green, tint.blue);
      if (transparencyMode != Opaque)
        setSemiTrans(Texture, 1);

      // Retrieve the CLUTs if 4-bit / 8-bit.
      if (IsIndexed(image)) {
        setClut(Texture, image->crect.x, image->crect.y);
      }

      int TPageX = (currentVramX >> bitsPerTPage) << bitsPerTPage;
      int TpageY = (currentVramY >> 8) << 8;

      setDrawTPage(TPage, 0, 1,
                   getTPage(imageDepth, transparencyMode, TPageX, TpageY));

      addPrim(renderer->orderingTable, Texture);
      addPrim(renderer->orderingTable, TPage);

      currentVramX += sliceWidth;
      remainingWidth -= sliceWidth;
      drawX += sliceWidth * pixelPerWord;
    }

    currentVramY += sliceHeight;
    remainingHeight -= sliceHeight;
    drawY += sliceHeight;
  }

  return 0;
}
