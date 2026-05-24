#ifndef IMAGE_H
#define IMAGE_H

#include <sys/types.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#include "error.h"
#include "renderer.h"
#include "file.h"

#define BITMODE4 0
#define BITMODE8 1
#define BITMODE16 2
#define BITMODE24 3

#define AUTO -1

#define IsIndexed(n) ((n)->mode & 0x08)
#define Is4Bit(n) ((n)->mode == 0x08)
#define Is8Bit(n) ((n)->mode == 0x09)

typedef struct Image {
  u_long mode;
  RECT prect;
  RECT crect;
} Image;

typedef struct VRAM_Position {
  short x, y;
} VRAM_Position;

typedef struct VRAM_Allocator {
  short start, end;
} VRAM_Allocator;

typedef enum Direction { None = 0, Left, Right } Direction;

VRAM_Position VRAM_Alloc(TIM_IMAGE *image, Direction direction);
Status Image_Load(Image *image, char *path, int imageVX, int imageVY,
                  Direction direction);

int Renderer_DrawImage(Renderer *renderer, Image *image, int x, int y,
                       Color tint, TransparencyMode transparencyMode);

#endif
