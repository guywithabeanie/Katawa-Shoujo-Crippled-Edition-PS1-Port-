#ifndef IMAGE_H
#define IMAGE_H

#include <sys/types.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#include "file.h"

#define BITMODE4  0
#define BITMODE8  1
#define BITMODE16 2
#define BITMODE24 3

#define AUTO -1

#define IsIndexed( n ) ( (n)->tim.mode & 0x08 )
#define Is4Bit( n ) ( (n)->tim.mode == 0x08 )
#define Is8Bit( n ) ( (n)->tim.mode == 0x09 )

#define GetR( n ) ( (n) & 0x1F )
#define GetG( n ) ( ( (n) >> 5 ) * 0x1F )
#define GetB( n ) ( ( (n) >> 10 ) * 0x1F )

typedef struct Image {
    int alpha;
    TIM_IMAGE tim;
    //Save the prect and crect since TIM_IMAGE only keeps pointers of them, which can screw up 
    //some things when loading multiple images.
    RECT prect;
    RECT crect;
} Image;

int Image_Load( Image* image, char* path, int imageVX, int imageVY, int clutVX, int clutVY );

#endif
